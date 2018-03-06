#pragma clang diagnostic ignored "-Wpadded"
#include <stm32f4xx.h>
#pragma clang diagnostic warning "-Wpadded"
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
static void FillCommand(PanelControl control, TypePress typePressm);
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
static const PanelControl controls[5][6] = {{B_0, B_5, B_Dot,    B_Ch,   B_F1, B_LEFT},          // RL0
                                            {B_1, B_6, B_Minus,  B_WF,   B_F2, Control_None},    // RL1
                                            {B_2, B_7, B_ESC,    B_U,    B_F3, REG_BTN},         // RL2
                                            {B_3, B_8, B_M,      B_ON1,  B_F4, Control_None},    // RL3
                                            {B_4, B_9, B_LEFT,   B_ON2,  B_F5, B_RIGHT}};        // RL4

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
            uint state = READ_RL(rl);

            PanelControl control =  controls[rl][sl];

            if (control != Control_None)
            {
                if (timePress[rl][sl])                                      // Если клавиша находится в нажатом положении
                {
                    if (time - timePress[rl][sl] > 100 &&                   // Если прошло более 100 мс с момента нажатия
                        !BUTTON_IS_PRESS(state))                            // и сейчас кнопка находится в отжатом состоянии
                    {
                        timePress[rl][sl] = 0;                              // То учитываем это в массиве
                        FillCommand(controls[rl][sl], TypePress_Release);   // И сохраняем отпускание кнопки в буфере команд
                    }
                }
                else if (BUTTON_IS_PRESS(state))                            // Если кнопка нажата
                {
                    timePress[rl][sl] = time;                               // то сохраняем время её нажатия
                    FillCommand(controls[rl][sl], TypePress_Press);
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
    RESET_SL(5);

    static uint prevStatesIsOne = false;

    uint stateLeft = READ_RL(1);
    uint stateRight = READ_RL(3);

    if (stateLeft && stateRight)
    {
        prevStatesIsOne = true;
    }
    else if (prevStatesIsOne && BUTTON_IS_PRESS(stateLeft) && !BUTTON_IS_PRESS(stateRight))
    {
        FillCommand(REG_A_LEFT, TypePress_Press);
        prevStatesIsOne = false;
    }
    else if (prevStatesIsOne && !BUTTON_IS_PRESS(stateLeft) && BUTTON_IS_PRESS(stateRight))
    {
        FillCommand(REG_B_RIGHT, TypePress_Press);
        prevStatesIsOne = false;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void FillCommand(PanelControl control, TypePress typePress)
{
    commands[pointer].control = control;
    commands[pointer++].typePress = typePress;
    Display::SetButton(typePress == TypePress_Press ? control : Control_None);
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
        retValue.control = Control_None;
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
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
const char *PanelControlName(PanelControl control)
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
