#include "defines.h"
#include "Keyboard.h"
#include "Display/Display.h"
#include "Hardware/Hardware.h"
#include "Hardware/CPU.h"
#include <stm32f4xx.h>
#include <stm32f4xx_hal.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static StructControl commands[10];
static int pointer = 0;


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
static const Control controls[5][6] = {{B_0, B_5, B_Dot,    B_Ch,   B_F1, B_LEFT},          // RL0
                                       {B_1, B_6, B_Minus,  B_WF,   B_F2, Control_None},    // RL1
                                       {B_2, B_7, B_ESC,    B_U,    B_F3, REG_BTN},         // RL2
                                       {B_3, B_8, B_M,      B_ON1,  B_F4, Control_None},    // RL3
                                       {B_4, B_9, B_LEFT,   B_ON2,  B_F5, B_RIGHT}};        // RL4

static uint16 sls[] =             {SL0,   SL1,   SL2,   SL3,   SL4,   SL5};
static GPIO_TypeDef* slsPorts[] = {GPIOB, GPIOB, GPIOB, GPIOB, GPIOD, GPIOD};
static char slsAsciiPorts[] =     {'B',   'B',   'B',   'B',   'D',   'D'};

static uint16 rls[] =             {RL0,   RL1,   RL2,   RL3,   RL4};
static GPIO_TypeDef* rlsPorts[] = {GPIOA, GPIOA, GPIOA, GPIOD, GPIOD};
static char rlsAsciiPorts[] =     {'A',   'A',   'A',   'D',   'D'};

#define SET_SL(n)       HAL_GPIO_WritePin(slsPorts[n], sls[n], GPIO_PIN_SET);
#define SET_ALL_SL      HAL_GPIO_WritePin(GPIOB, SL0 | SL1 | SL2 | SL3, GPIO_PIN_SET); HAL_GPIO_WritePin(GPIOD, SL4 | SL5, GPIO_PIN_SET);
#define RESET_SL(n)     HAL_GPIO_WritePin(slsPorts[n], sls[n], GPIO_PIN_RESET)
#define READ_RL(n)      HAL_GPIO_ReadPin(rlsPorts[n], rls[n])

static bool init = false;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Keyboard::Init(void)
{
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            timePress[i][j] = 0;
        }
    }

    pointer = 0;

    CPU::SetCallbackKeyboard(&Keyboard::Update);

    CPU::InitKeyboardInputs(sls, slsAsciiPorts, 6, rls, rlsAsciiPorts, 5);

    init = true;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Keyboard::Update(void)
{
    if(!init)
    {
        return;
    }
    
    uint time = HAL_GetTick();

    for (int sl = 0; sl < NUM_SL; sl++)
    {
        RESET_SL(sl);

        for (int rl = 0; rl < NUM_RL; rl++)
        {
            uint state = READ_RL(rl);

            Control control =  controls[rl][sl];

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
static void FillCommand(Control control, TypePress typePress)
{
    commands[pointer].control = control;
    commands[pointer++].typePress = typePress;
    Display::SetButton(typePress == TypePress_Press ? control : Control_None);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Keyboard_BufferIsEmpty(void)
{
    return pointer == 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
StructControl Keyboard_GetNextControl(void)
{
    StructControl retValue;

    if (Keyboard_BufferIsEmpty())
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
const char *ControlName(Control control)
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
