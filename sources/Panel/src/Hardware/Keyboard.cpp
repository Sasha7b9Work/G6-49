#include <stm32f4xx.h>
#include "defines.h"
#include "Display/Display.h"
#include "Hardware/Beeper.h"
#include "Hardware/CPU.h"
#include "Hardware/Keyboard.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"


static KeyEvent commands[10];
static int pointer = 0;
static GPIO_TypeDef * const ports[] = {GPIOA, GPIOB, GPIOC, GPIOD, GPIOE};


static void FillCommand(KeyEvent::E control, KeyEvent::Action::E action);
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

/// При обнаружении нажатия кнопки сюда записывается время нажатия
static uint timePress[5][6];

                                     //          SL0          SL1          SL2             SL3             S4             SL5
static const KeyEvent::E controls[5][6] = {{KeyEvent::_0, KeyEvent::_5, KeyEvent::Dot,   KeyEvent::Esc,   KeyEvent::F1,   KeyEvent::None},    // RL0
                                          {KeyEvent::_1, KeyEvent::_6, KeyEvent::Minus, KeyEvent::Left,  KeyEvent::F2,   KeyEvent::None},    // RL1
                                          {KeyEvent::_2, KeyEvent::_7, KeyEvent::None,  KeyEvent::Right, KeyEvent::F3,   KeyEvent::None},    // RL2
                                          {KeyEvent::_3, KeyEvent::_8, KeyEvent::On1,   KeyEvent::None,  KeyEvent::F4,   KeyEvent::None},    // RL3
                                          {KeyEvent::_4, KeyEvent::_9, KeyEvent::On2,   KeyEvent::None,  KeyEvent::None, KeyEvent::None}};   // RL4

static uint16 sls[] =             {SL0,   SL1,   SL2,   SL3,   SL4,   SL5};
static char slsAsciiPorts[] =     {'B',   'B',   'B',   'B',   'D',   'D'};

static uint16 rls[] =             {RL0,   RL1,   RL2,   RL3,   RL4};
static char rlsAsciiPorts[] =     {'A',   'A',   'A',   'D',   'D'};

#define SET_SL(n)       HAL_GPIO::WritePin(slsAsciiPorts[n], sls[n], true)
#define SET_ALL_SL      HAL_GPIO::WritePin('B', SL0 | SL1 | SL2 | SL3, true); HAL_GPIO::WritePin('D', SL4 | SL5, true);
#define RESET_SL(n)     HAL_GPIO::WritePin(slsAsciiPorts[n], sls[n], false)
#define READ_RL(n)      HAL_GPIO::ReadPin(rlsAsciiPorts[n], rls[n])

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

            KeyEvent::E control =  controls[rl][sl];

            if (control != KeyEvent::None)
            {
                if (timePress[rl][sl] && timePress[rl][sl] != MAX_UINT)         // Если клавиша находится в нажатом положении
                {
                    uint delta = time - timePress[rl][sl];
                    if(delta > 500)                                             // Если прошло более 500 мс с момента нажатия -
                    {
                        timePress[rl][sl] = MAX_UINT;
                        FillCommand(controls[rl][sl], KeyEvent::Action::Long);   // это будет длинное нажатие
                    }
                    else if (delta > 100 &&                                     // Если прошло более 100 мс с момента нажатия
                        !BUTTON_IS_PRESS(state))                                // и сейчас кнопка находится в отжатом состоянии
                    {
                        timePress[rl][sl] = MAX_UINT;                           // То учитываем это в массиве
                        FillCommand(controls[rl][sl], KeyEvent::Action::Up);     // И сохраняем отпускание кнопки в буфере команд
                    }
                    else
                    {
                        // здесь ничего
                    }
                }
                else if (BUTTON_IS_PRESS(state) && timePress[rl][sl] != MAX_UINT)   // Если кнопка нажата
                {
                    timePress[rl][sl] = time;                                       // то сохраняем время её нажатия
                    FillCommand(controls[rl][sl], KeyEvent::Action::Down);
                }
                else if(!BUTTON_IS_PRESS(state) && timePress[rl][sl] == MAX_UINT)
                {
                    timePress[rl][sl] = 0;
                }
                else
                {
                    // здесь ничего
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
    
    bool press = HAL_GPIO::ReadPin('C', GPIO_PIN_2) ? false : true;

    if(!press)
    {
        needDetectButton = true;
    }

    if(needDetectButton)
    {
        uint time = TIME_MS;

        if(press && prevPressButton && time - timePrevPress > 500)          // Если нажатие длится более 0.5 сек
        {
            FillCommand(KeyEvent::RegButton, KeyEvent::Action::Long);                                     // посылаем длинное нажатие
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
                FillCommand(KeyEvent::RegButton, KeyEvent::Action::Down);
            }
        }
        else                                                                // Ексли копка была нажата ранее
        {
            if(time - timePrevPress > 50)                                   // то проверять в следующий раз будем не ранее чем через 50 мс
            {                                                               // во избежание дребезга контактов
                if(!press)
                {
                    FillCommand(KeyEvent::RegButton, KeyEvent::Action::Up);
                    timePrevPress = 0;
                    prevPressButton = false;
                }
            }
        }
    }


    // Детектируем поворот
    static bool prevStatesIsOne = false;

    bool stateLeft = HAL_GPIO::ReadPin('C', GPIO_PIN_0);
    bool stateRight = HAL_GPIO::ReadPin('C', GPIO_PIN_1);

    if (stateLeft && stateRight)
    {
        prevStatesIsOne = true;
    }
    else if (prevStatesIsOne && stateLeft && !stateRight)
    {
        FillCommand(KeyEvent::RegLeft, KeyEvent::Action::Down);
        prevStatesIsOne = false;
    }
    else if (prevStatesIsOne && !stateLeft && stateRight)
    {
        FillCommand(KeyEvent::RegRight, KeyEvent::Action::Down);
        prevStatesIsOne = false;
    }
    else
    {
        // здесь ничего
    }
}


static void FillCommand(KeyEvent::E key, KeyEvent::Action::E action)
{
    commands[pointer++] = KeyEvent(key, action);

    Beeper::Beep(action);
}


bool Keyboard::BufferIsEmpty()
{
    return pointer == 0;
}


KeyEvent Keyboard::GetNextControl()
{
    KeyEvent retValue;

    if (BufferIsEmpty())
    {
        retValue.value = KeyEvent::None;
    }
    else
    {
        retValue = commands[0];
        for (int i = 1; i < pointer; i++)
        {
            commands[i - 1] = commands[i];
        }
        --pointer;
    }

    return retValue;
}


void Keyboard::InitInputs(const uint16 *sl, const char *portSL, int numSL, const uint16 *rl, const char *portRL, int numRL)
{
    GPIO_InitTypeDef isGPIO;

    for (int i = 0; i < numRL; i++)
    {
        isGPIO.Pin = rl[i];
        isGPIO.Mode = GPIO_MODE_INPUT;
        HAL_GPIO_Init(ports[portRL[i] - 'A'], &isGPIO);
    }

    for (int i = 0; i < numSL; i++)
    {
        isGPIO.Pin = sl[i];
        isGPIO.Mode = GPIO_MODE_OUTPUT_PP;
        HAL_GPIO_Init(ports[portSL[i] - 'A'], &isGPIO);
    }

    // Инициализируем ручку
    isGPIO.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2;
    isGPIO.Mode = GPIO_MODE_INPUT;
    isGPIO.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOC, &isGPIO);
}


const char *PanelControlName(const KeyEvent &control)
{
    static const char *names[] =
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

    return names[control];
}


void Keyboard::Draw()
{
} 

#ifdef __cplusplus
extern "C" {
#endif

    //------------------------------------------------------------------------------------------------------------------------------------------------
    void TIM4_IRQHandler()
    {
        if ((TIM4->SR & TIM_SR_UIF) == TIM_SR_UIF)
        {
            if ((TIM4->DIER & TIM_DIER_UIE) == TIM_DIER_UIE)
            {
                TIM4->SR = ~TIM_DIER_UIE;
                HAL_TIM4::ElapsedCallback();
            }
        }
    }

#ifdef __cplusplus
}
#endif
