#include "defines.h"
#include "Beeper.h"
#include "Timer.h"
#include "Utils/Math.h"
#include "Hardware/HAL/HAL.h"
#include <stm32f4xx_hal.h>
#include <cmath>


enum TypeWave
{
    TypeWave_Sine,
    TypeWave_Meandr,
    TypeWave_Triangle
};


static void Stop();

static void SetWave(void);

static uint16 CalculatePeriodForTIM();

static void TIM7_Config(uint16 prescaler, uint16 period);

static void CalculateSine();

static void CalculateMeandr();

static void CalculateTriangle();

// Звук нажатия на кнопку.
static void ButtonPress();

// Функция вызовет звук отпускаемой кнопки только если перед этим проигрывался звук нажатия кнопки.
static void ButtonRelease();


static bool SOUND_WARN_IS_BEEP = false;
static bool SOUND_ENABLED = true;
static bool SOUND_IS_BEEP = false;
static bool BUTTON_IS_PRESSED = false;

static float frequency = 0.0f;
static float amplitude = 0.0f;
static TypeWave typeWave = TypeWave_Sine;

static const int POINTS_IN_PERIOD = 10;
static uint8 points[POINTS_IN_PERIOD] = { 0 };


void Beeper::Init()
{
    HAL_DAC2::Init();
}


void Sound_Beep(TypeWave typeWave_, float frequency_, float amplitude_, uint duration)
{
    if (SOUND_WARN_IS_BEEP)
    {
        return;
    }
    if (!SOUND_ENABLED)
    {
        return;
    }
    if (frequency != frequency_ || amplitude != amplitude_ || typeWave != typeWave_)
    {
        frequency = frequency_;
        amplitude = amplitude_;
        typeWave = typeWave_;

        Stop();
        SetWave();
    }

    Stop();

    SOUND_IS_BEEP = true;

    HAL_DAC_Start_DMA(reinterpret_cast<DAC_HandleTypeDef *>(HAL_DAC2::handle), DAC_CHANNEL_1, (uint32_t *)points, POINTS_IN_PERIOD, DAC_ALIGN_8B_R);

    Timer::SetAndStartOnce(Timer::Type::StopSound, Stop, duration);
}


void SetWave(void)
{
    TIM7_Config(0, CalculatePeriodForTIM());

    if (typeWave == TypeWave_Sine)
    {
        CalculateSine();
    }
    else if (typeWave == TypeWave_Meandr)
    {
        CalculateMeandr();
    }
    else if (typeWave == TypeWave_Triangle)
    {
        CalculateTriangle();
    }
}

void TIM7_Config(uint16 prescaler, uint16 period)
{
    static TIM_HandleTypeDef htim =
    {
        TIM7,
        {
            0,
            TIM_COUNTERMODE_UP,
        }
    };

    htim.Init.Prescaler = prescaler;
    htim.Init.Period = period;
    htim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;

    HAL_TIM_Base_Init(&htim);

    TIM_MasterConfigTypeDef masterConfig =
    {
        TIM_TRGO_UPDATE,
        TIM_MASTERSLAVEMODE_DISABLE
    };

    HAL_TIMEx_MasterConfigSynchronization(&htim, &masterConfig);

    HAL_TIM_Base_Stop(&htim);
    HAL_TIM_Base_Start(&htim);
}


uint16 CalculatePeriodForTIM()
{
    return static_cast<uint16>(120e6F / frequency / POINTS_IN_PERIOD);
}


void CalculateSine()
{
    for (int i = 0; i < POINTS_IN_PERIOD; i++)
    {
        float step = 2.0f * 3.1415926f / (POINTS_IN_PERIOD - 1);
        float value = (std::sin(static_cast<float>(i) * step) + 1.0f) / 2.0f;
        points[i] = static_cast<uint8>(value * amplitude * 255.0F);
    }
}


void CalculateMeandr()
{
    for (int i = 0; i < POINTS_IN_PERIOD / 2; i++)
    {
        points[i] = static_cast<uint8>(255.0F * amplitude);
    }
    for (int i = POINTS_IN_PERIOD / 2; i < POINTS_IN_PERIOD; i++)
    {
        points[i] = 0;
    }
}


void CalculateTriangle()
{
    float k = 255.0 / POINTS_IN_PERIOD;
    for (int i = 0; i < POINTS_IN_PERIOD; i++)
    {
        points[i] = static_cast<uint8>(k * static_cast<float>(i) * amplitude);
    }
}


static void ButtonPress()
{
    Sound_Beep(TypeWave_Sine, 2000.0F, 1.0F, 50);
}


static void Stop()
{
    HAL_DAC_Stop_DMA(reinterpret_cast<DAC_HandleTypeDef *>(HAL_DAC2::handle), DAC_CHANNEL_1);
    SOUND_IS_BEEP = false;
    SOUND_WARN_IS_BEEP = false;
}


static void ButtonRelease()
{
    if (BUTTON_IS_PRESSED)
    {
        Sound_Beep(TypeWave_Sine, 1000.0f, 0.25f, 50);
        BUTTON_IS_PRESSED = 0;
    }
}


void Beeper::Beep(Action::E action)
{
    static const pFuncVV func[Action::Count] =
    {
        ButtonPress,
        ButtonRelease,
        ButtonRelease
    };

    if (action < Action::Count)
    {
        func[action]();
    }
}
