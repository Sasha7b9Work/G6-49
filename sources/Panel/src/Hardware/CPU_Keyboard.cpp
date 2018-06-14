#ifdef OPEN
#include <stm32f7xx.h>
#else
#include <stm32f4xx.h>
#endif

#include "defines.h"
#include "Display/Display.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define TIME_UPDATE 2   ///< Время между опросами клавиатуры
static StructControl commands[10];
static int pointer = 0;
static GPIO_TypeDef * const ports[] = {GPIOA, GPIOB, GPIOC, GPIOD, GPIOE};
/// Таймер для опроса клавиатуры
static TIM_HandleTypeDef handleTIM4;
static void(*callbackKeyboard)() = 0;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void FillCommand(Control control, TypePress typePressm);
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

                                           // SL0  SL1  SL2       SL3     SL4   SL5
static const Control controls[5][6] = {{B_0, B_5, B_Dot,   B_ESC,   B_F1, B_None},          // RL0
                                            {B_1, B_6, B_Minus, B_LEFT,  B_F2, B_None},    // RL1
                                            {B_2, B_7, B_None,  B_RIGHT, B_F3, B_None},         // RL2
                                            {B_3, B_8, B_ON1,   B_None,  B_F4, B_None},    // RL3
                                            {B_4, B_9, B_ON2,   B_None,  B_F5, B_None}};        // RL4

static uint16 sls[] =             {SL0,   SL1,   SL2,   SL3,   SL4,   SL5};
static char slsAsciiPorts[] =     {'B',   'B',   'B',   'B',   'D',   'D'};

static uint16 rls[] =             {RL0,   RL1,   RL2,   RL3,   RL4};
static char rlsAsciiPorts[] =     {'A',   'A',   'A',   'D',   'D'};

#define SET_SL(n)       CPU::GPIO_::WritePin(slsAsciiPorts[n], sls[n], true)
#define SET_ALL_SL      CPU::GPIO_::WritePin('B', SL0 | SL1 | SL2 | SL3, true); CPU::GPIO_::WritePin('D', SL4 | SL5, true);
#define RESET_SL(n)     CPU::GPIO_::WritePin(slsAsciiPorts[n], sls[n], false)
#define READ_RL(n)      CPU::GPIO_::ReadPin(rlsAsciiPorts[n], rls[n])

static bool init = false;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CPU::Keyboard::Init(void)
{
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            timePress[i][j] = 0;
        }
    }

    pointer = 0;

    CPU::Keyboard::SetCallback(&Keyboard::Update);

    CPU::Keyboard::InitInputs(sls, slsAsciiPorts, 6, rls, rlsAsciiPorts, 5);

    init = true;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CPU::Keyboard::Update(void)
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

            Control control =  controls[rl][sl];

            if (control != B_None)
            {
                if (timePress[rl][sl] && timePress[rl][sl] != MAX_UINT)     // Если клавиша находится в нажатом положении
                {
                    uint delta = time - timePress[rl][sl];
                    if(delta > 500)                                         // Если прошло более 500 мс с момента нажатия -
                    {
                        timePress[rl][sl] = MAX_UINT;
                        FillCommand(controls[rl][sl], Long);                // это будет длинное нажатие
                    }
                    else if (delta > 100 &&                                 // Если прошло более 100 мс с момента нажатия
                        !BUTTON_IS_PRESS(state))                            // и сейчас кнопка находится в отжатом состоянии
                    {
                        timePress[rl][sl] = 0;                              // То учитываем это в массиве
                        FillCommand(controls[rl][sl], Up);                  // И сохраняем отпускание кнопки в буфере команд
                    }
                }
                else if (BUTTON_IS_PRESS(state) && timePress[rl][sl] != MAX_UINT) // Если кнопка нажата
                {
                    timePress[rl][sl] = time;                               // то сохраняем время её нажатия
                    FillCommand(controls[rl][sl], Down);
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

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DetectRegulator(void)
{
    // Детектируем кнопку
    static bool prevPressButton = false;
    static uint timePrevPress = 0;
    static bool needDetectButton = true;
    
    bool press = CPU::GPIO_::ReadPin('C', GPIO_PIN_2) ? false : true;

    if(!press)
    {
        needDetectButton = true;
    }

    if(needDetectButton)
    {
        uint time = TIME_MS;

        if(press && prevPressButton && time - timePrevPress > 500)          // Если нажатие длится более 0.5 сек
        {
            FillCommand(REG_BTN, Long);                                     // посылаем длинное нажатие
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
                FillCommand(REG_BTN, Down);
            }
        }
        else                                                                // Ексли копка была нажата ранее
        {
            if(time - timePrevPress > 50)                                   // то проверять в следующий раз будем не ранее чем через 50 мс
            {                                                               // во избежание дребезга контактов
                if(!press)
                {
                    FillCommand(REG_BTN, Up);
                    timePrevPress = 0;
                    prevPressButton = false;
                }
            }
        }
    }


    // Детектируем поворот
    static bool prevStatesIsOne = false;

    bool stateLeft = CPU::GPIO_::ReadPin('C', GPIO_PIN_0);
    bool stateRight = CPU::GPIO_::ReadPin('C', GPIO_PIN_1);

    if (stateLeft && stateRight)
    {
        prevStatesIsOne = true;
    }
    else if (prevStatesIsOne && stateLeft && !stateRight)
    {
        FillCommand(REG_LEFT, Down);
        prevStatesIsOne = false;
    }
    else if (prevStatesIsOne && !stateLeft && stateRight)
    {
        FillCommand(REG_RIGHT, Down);
        prevStatesIsOne = false;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void FillCommand(Control control, TypePress typePress)
{
    commands[pointer].key = control;
    commands[pointer++].typePress = typePress;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool CPU::Keyboard::BufferIsEmpty(void)
{
    return pointer == 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
StructControl CPU::Keyboard::GetNextControl(void)
{
    StructControl retValue;

    if (BufferIsEmpty())
    {
        retValue.key = B_None;
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

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CPU::Keyboard::InitInputs(uint16 sl[], char portSL[], int numSL, uint16 rl[], char portRL[], int numRL)
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

    // Инициализируем таймер, по прерываниям которого будем опрашивать клавиатуру
    HAL_NVIC_SetPriority(TIM4_IRQn, 0, 1);

    HAL_NVIC_EnableIRQ(TIM4_IRQn);

    handleTIM4.Instance = TIM4;
    handleTIM4.Init.Period = TIME_UPDATE * 10 - 1;
    handleTIM4.Init.Prescaler = (uint)((SystemCoreClock / 2) / 10000) - 1;
    handleTIM4.Init.ClockDivision = 0;
    handleTIM4.Init.CounterMode = TIM_COUNTERMODE_UP;

    if (HAL_TIM_Base_Init(&handleTIM4) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    if (HAL_TIM_Base_Start_IT(&handleTIM4) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    // Инициализируем ручку
    isGPIO.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2;
    isGPIO.Mode = GPIO_MODE_INPUT;
    isGPIO.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOC, &isGPIO);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
const char *PanelControlName(Control control)
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

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CPU::Keyboard::TIM4_::ElapsedCallback(void *htim)
{
    if ((TIM_HandleTypeDef *)htim == &handleTIM4 && callbackKeyboard)
    {
        callbackKeyboard();
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CPU::Keyboard::SetCallback(void(*func)())
{
    callbackKeyboard = func;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CPU::Keyboard::TIM4_::Start(uint timeStopMS)
{
    Stop();

    if (timeStopMS == MAX_UINT)
    {
        return;
    }

    uint dT = timeStopMS - TIME_MS;

    handleTIM4.Init.Period = (dT * 2) - 1;  // 10 соответствует 0.1мс. Т.е. если нам нужна 1мс, нужно засылать (100 - 1)

    HAL_TIM_Base_Init(&handleTIM4);
    HAL_TIM_Base_Start_IT(&handleTIM4);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CPU::Keyboard::TIM4_::Stop()
{
    HAL_TIM_Base_Stop_IT(&handleTIM4);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CPU::Keyboard::Draw()
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
                CPU::Keyboard::TIM4_::ElapsedCallback(&handleTIM4);
            }
        }
    }

#ifdef __cplusplus
}
#endif
