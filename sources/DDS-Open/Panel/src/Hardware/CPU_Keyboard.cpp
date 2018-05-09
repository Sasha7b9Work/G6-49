#include "CPU.h"
#include "Display/Painter.h"
#include "Display/Colors.h"
#include "Hardware/Timer.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint time = 0;
static void(*callbackKeyboard)() = 0;
static TIM_HandleTypeDef handleTIM4;
#define TIME_UPDATE 2


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CPU::Keyboard::Init()
{
    SetCallback(&Update);

    HAL_NVIC_SetPriority(TIM4_IRQn, 0, 1);

    HAL_NVIC_EnableIRQ(TIM4_IRQn);

    handleTIM4.Instance = TIM4;
    handleTIM4.Init.Period = TIME_UPDATE * 10 - 1;
    handleTIM4.Init.Prescaler = (uint)((SystemCoreClock / 2) / 10000) - 1;
    handleTIM4.Init.ClockDivision = 0;
    handleTIM4.Init.CounterMode = TIM_COUNTERMODE_UP;

    HAL_TIM_Base_Init(&handleTIM4);

    HAL_TIM_Base_Start_IT(&handleTIM4);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool CPU::Keyboard::BufferIsEmpty()
{
    return true;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
StructControl CPU::Keyboard::GetNextControl()
{
    StructControl retValue;
    retValue.typePress = TypePress_Press;
    return retValue;
}

#define WIDTH_BUTTON 150
#define HEIGHT_BUTTON 80
#define DELTA 10

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CPU::Keyboard::Draw()
{
    int x0 = 350;
    int y0 = 25;

    static char *titles[6][4] =
    {
        {"F1", "1", "2", "3"},
        {"F2", "4", "5", "6"},
        {"F3", "7", "8", "9"},
        {"F4", ".", "0", "-"},
        {"ESC", "REG LEFT", "BTN", "REG RIGHT"},
        {"ON1", "ON2", "LEFT", "RIGHT"}
    };

    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            char *title = titles[i][j];
            if (title[0])
            {
                DrawButton(x0 + j * (WIDTH_BUTTON + DELTA), y0 + i * (HEIGHT_BUTTON + DELTA), title);
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CPU::Keyboard::DrawButton(int x, int y, char *title)
{
    Painter::DrawRectangle(x, y, WIDTH_BUTTON, HEIGHT_BUTTON, Color::FILL);
    Painter::DrawStringInCenterRect(x, y, WIDTH_BUTTON, HEIGHT_BUTTON, title);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CPU::Keyboard::Update()
{
    time = TIME_MS;
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
void CPU::Keyboard::TIM4_::ElapsedCallback(void *htim)
{
    if ((TIM_HandleTypeDef *)htim == &handleTIM4 && callbackKeyboard)
    {
        callbackKeyboard();
    }
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
