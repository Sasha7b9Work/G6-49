#include "defines.h"
#include "Display/Display.h"
#include "Hardware/Beeper.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/HAL/HAL_PIO.h"
#include "Keyboard/Keyboard.h"


static Control commands[10];
static int pointer = 0;


static void DetectRegulator();

#define SL0 (1 << 12)
#define SL1 (1 << 13)
#define SL2 (1 << 14)
#define SL3 (1 << 15)
#define SL4 (1 << 8)
#define SL5 (1 << 9)
#define NUM_SL 6

#define RL0 (1 << 8) 
#define RL1 (1 << 9)
#define RL2 (1 << 10)
#define RL3 (1 << 13)
#define RL4 (1 << 12)
#define NUM_RL 5

#define BUTTON_IS_PRESS(state)  ((state) == 0)

// При обнаружении нажатия кнопки сюда записывается время нажатия
static uint timePress[5][6];

//                                         SL0      SL1       SL2          SL3        S4          SL5
static const Key::E keys[5][6] =     {{Key::_0, Key::_5, Key::Dot,   Key::Esc,   Key::F1,   Key::None},    // RL0
                                      {Key::_1, Key::_6, Key::Minus, Key::Right, Key::F2,   Key::None},    // RL1
                                      {Key::_2, Key::_7, Key::None,  Key::Left,  Key::F3,   Key::None},    // RL2
                                      {Key::_3, Key::_8, Key::On1,   Key::None,  Key::F4,   Key::None},    // RL3
                                      {Key::_4, Key::_9, Key::On2,   Key::None,  Key::None, Key::None}};   // RL4

static uint16 sls[] =             {SL0,   SL1,   SL2,   SL3,   SL4,   SL5};
static char slsAsciiPorts[] =     {'B',   'B',   'B',   'B',   'D',   'D'};

static uint16 rls[] =             {RL0,   RL1,   RL2,   RL3,   RL4};
static char rlsAsciiPorts[] =     {'A',   'A',   'A',   'D',   'D'};

#define SET_SL(n)       HAL_PIO::WritePin(slsAsciiPorts[n], sls[n], true)
#define SET_ALL_SL      HAL_PIO::WritePin('B', SL0 | SL1 | SL2 | SL3, true); HAL_PIO::WritePin('D', SL4 | SL5, true);
#define RESET_SL(n)     HAL_PIO::WritePin(slsAsciiPorts[n], sls[n], false)
#define READ_RL(n)      HAL_PIO::ReadPin(rlsAsciiPorts[n], rls[n])

static bool init = false;


void Keyboard::Init()
{
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            timePress[i][j] = 0;
        }
    }

    pointer = 0;

    HAL_TIM4::Init(&Keyboard::Update);

    Keyboard::InitInputs(sls, slsAsciiPorts, 6, rls, rlsAsciiPorts, 5);

    init = true;
}


void Keyboard::Update()
{
    if(!init)
    {
        return;
    }
    
    uint time = TIME_MS;

    for (int sl = 0; sl < NUM_SL; sl++)
    {
        RESET_SL(sl);

        for (int rl = 0; rl < NUM_RL; rl++)
        {
            bool state = READ_RL(rl);

            Key::E key = keys[rl][sl];

            if (key != Key::None)
            {
                if ((timePress[rl][sl] != 0) && (timePress[rl][sl] != MAX_UINT))         // Если клавиша находится в нажатом положении
                {
                    uint delta = time - timePress[rl][sl];
                    if(delta > 500)                                             // Если прошло более 500 мс с момента нажатия -
                    {
                        timePress[rl][sl] = MAX_UINT;
                        AppendEvent(keys[rl][sl], Action::Long);   // это будет длинное нажатие
                    }
                    else if (delta > 100 &&                                     // Если прошло более 100 мс с момента нажатия
                        !BUTTON_IS_PRESS(state))                                // и сейчас кнопка находится в отжатом состоянии
                    {
                        timePress[rl][sl] = MAX_UINT;                           // То учитываем это в массиве
                        AppendEvent(keys[rl][sl], Action::Up);     // И сохраняем отпускание кнопки в буфере команд
                    }
                }
                else if (BUTTON_IS_PRESS(state) && timePress[rl][sl] != MAX_UINT)   // Если кнопка нажата
                {
                    timePress[rl][sl] = time;                                       // то сохраняем время её нажатия
                    AppendEvent(keys[rl][sl], Action::Down);
                }
                else if(!BUTTON_IS_PRESS(state) && timePress[rl][sl] == MAX_UINT)
                {
                    timePress[rl][sl] = 0;
                }
            }
        }
        
        SET_ALL_SL;
    }
    
    DetectRegulator();

    SET_ALL_SL;
}


static void DetectRegulator()
{
    // Детектируем кнопку
    static bool prevPressButton = false;
    static uint timePrevPress = 0;
    static bool needDetectButton = true;
    
    bool press = HAL_PIO::ReadPin('C', HPin::_2) ? false : true;

    if(!press)
    {
        needDetectButton = true;
    }

    if(needDetectButton)
    {
        uint time = TIME_MS;

        if(press && prevPressButton && time - timePrevPress > 500)          // Если нажатие длится более 0.5 сек
        {
            Keyboard::AppendEvent(Key::RegButton, Action::Long);                                     // посылаем длинное нажатие
            needDetectButton = false;
            prevPressButton = false;
            timePrevPress = 0;
        }

        if(timePrevPress == 0)                                              // Если кнопка находилась ранее в отжатом положении
        {
            if(press)                                                       // И теперь нажата
            {
                timePrevPress = time;
                prevPressButton = true;
                Keyboard::AppendEvent(Key::RegButton, Action::Down);
            }
        }
        else                                                                // Ексли копка была нажата ранее
        {
            if(time - timePrevPress > 50)                                   // то проверять в следующий раз будем не ранее чем через 50 мс
            {                                                               // во избежание дребезга контактов
                if(!press)
                {
                    Keyboard::AppendEvent(Key::RegButton, Action::Up);
                    timePrevPress = 0;
                    prevPressButton = false;
                }
            }
        }
    }


    // Детектируем поворот
    static bool prevStatesIsOne = false;

    bool stateLeft = HAL_PIO::ReadPin('C', HPin::_0);
    bool stateRight = HAL_PIO::ReadPin('C', HPin::_1);

    if (stateLeft && stateRight)
    {
        prevStatesIsOne = true;
    }
    else if (prevStatesIsOne && stateLeft && !stateRight)
    {
        Keyboard::AppendEvent(Key::RotateLeft, Action::Down);
        prevStatesIsOne = false;
    }
    else if (prevStatesIsOne && !stateLeft && stateRight)
    {
        Keyboard::AppendEvent(Key::RotateRight, Action::Down);
        prevStatesIsOne = false;
    }
}


void Keyboard::AppendEvent(Key::E key, Action::E action)
{
    commands[pointer].key = key;
    commands[pointer].action = action;
    pointer++;

    Beeper::Beep(action);
}


bool Keyboard::BufferIsEmpty()
{
    return pointer == 0;
}


Control Keyboard::GetNextControl()
{
    Control control;

    if (BufferIsEmpty())
    {
        control.key = Key::None;
    }
    else
    {
        control = commands[0];
        for (int i = 1; i < pointer; i++)
        {
            commands[i - 1] = commands[i];
        }
        --pointer;
    }

    return control;
}

#ifdef WIN32
void Keyboard::InitInputs(const uint16 *, const char *, int, const uint16 *, const char *, int) {}
#else
void Keyboard::InitInputs(const uint16 *sl, const char *portSL, int numSL, const uint16 *rl, const char *portRL, int numRL)
{
    for (int i = 0; i < numRL; i++)
    {
        HAL_PIO::Init(portRL[i], rl[i], HMode::Input, HPull::No);
    }

    for (int i = 0; i < numSL; i++)
    {
        HAL_PIO::Init(portSL[i], sl[i], HMode::Output_PP, HPull::No);
    }

    // Инициализируем ручку
    HAL_PIO::Init('C', HPin::_0, HMode::Input, HPull::Up);
    HAL_PIO::Init('C', HPin::_1, HMode::Input, HPull::Up);
    HAL_PIO::Init('C', HPin::_2, HMode::Input, HPull::Up);
}
#endif


const char *PanelControlName(const Key::E key)
{
    static pCHAR names[] =
    {
        "None",
        "F1",
        "F2",
        "F3",
        "F4",
        "F5",
        "0",
        "1",
        "2",
        "3",
        "4",
        "5",
        "6",
        "7",
        "8",
        "9",
        ".",
        "-",
        "Esc",
        "left",
        "right",
        "Ch",
        "WF",
        "M",
        "U",
        "ON1",
        "ON2",
        "Reg Left",
        "Reg right",
        "Reg кнопка"
    };

    return names[key];
}


void Keyboard::Draw()
{
} 


bool Keyboard::Decoder::Decode(const Control controls[20], const Control &control)
{
    static int step = 0;        // Номер следующего проверяемого элемента массива

    if(control == controls[step])
    {
        step++;
        if(controls[step].key == Key::Count)
        {
            step = 0;
            return true;
        }

        return false;
    }

    step = 0;

    return false;
}
