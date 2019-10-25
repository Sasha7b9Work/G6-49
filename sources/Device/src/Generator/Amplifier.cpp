#include "defines.h"
#include "Generator/Amplifier.h"
#include "Settings/Settings.h"
#include <stm32f4xx.h>


struct Attenuation
{
    enum E
    {
        _0Db,
        _10Db,
        _20Db,
        _30Db,
        Count
    };
};


struct Attenuator
{
    static void SetAttenuation(Chan::E ch, Attenuation::E attenuation)
    {
        static GPIO_TypeDef *const gpio0[Chan::Count] = { GPIOE, GPIOF };
        static GPIO_TypeDef *const gpio1[Chan::Count] = { GPIOB, GPIOC };

        static const uint16 pin0[Chan::Count] = { GPIO_PIN_15, GPIO_PIN_5 };
        static const uint16 pin1[Chan::Count] = { GPIO_PIN_5, GPIO_PIN_13 };

        static const GPIO_PinState state0[Attenuation::Count] =
        {
            GPIO_PIN_RESET,
            GPIO_PIN_SET,
            GPIO_PIN_RESET,
            GPIO_PIN_SET
        };

        static const GPIO_PinState state1[Attenuation::Count] =
        {
            GPIO_PIN_RESET,
            GPIO_PIN_RESET,
            GPIO_PIN_SET,
            GPIO_PIN_SET
        };

        HAL_GPIO_WritePin(gpio0[ch], pin0[ch], state0[attenuation]);
        HAL_GPIO_WritePin(gpio1[ch], pin1[ch], state1[attenuation]);
    }

    static void Init()
    {
        GPIO_InitTypeDef isGPIO;
        isGPIO.Pin = GPIO_PIN_15;
        isGPIO.Mode = GPIO_MODE_OUTPUT_PP;
        isGPIO.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOE, &isGPIO);

        isGPIO.Pin = GPIO_PIN_10;
        HAL_GPIO_Init(GPIOB, &isGPIO);

        isGPIO.Pin = GPIO_PIN_5;
        HAL_GPIO_Init(GPIOF, &isGPIO);

        isGPIO.Pin = GPIO_PIN_13;
        HAL_GPIO_Init(GPIOC, &isGPIO);
    }
};


void Amplifier::Init()
{
    GPIO_InitTypeDef isGPIO;
    isGPIO.Pin = GPIO_PIN_0;
    isGPIO.Mode = GPIO_MODE_OUTPUT_PP;
    isGPIO.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOF, &isGPIO);

    isGPIO.Pin = GPIO_PIN_14;
    HAL_GPIO_Init(GPIOC, &isGPIO);

    Attenuator::Init();
}


void Amplifier::Tune(Chan::E ch)
{
    float amplitude = set.amplitude[ch].ToFloat();

    if (amplitude > 3.16F)              // 1 диапазон
    {
        SetState(ch, true);
        Attenuator::SetAttenuation(ch, Attenuation::_0Db);
    }
    else if (amplitude > 1.0F)          // 2 диапазон
    {
        SetState(ch, true);
        Attenuator::SetAttenuation(ch, Attenuation::_10Db);
    }
    else if (amplitude > 0.316F)        // 3,4 диапазоны
    {
        if (set.offset[ch].ToFloat() > 2.5F)
        {
            SetState(ch, true);
            Attenuator::SetAttenuation(ch, Attenuation::_20Db);
        }
        else
        {
            SetState(ch, false);
            Attenuator::SetAttenuation(ch, Attenuation::_0Db);
        }
    }
    else if (amplitude > 0.100F)        // 5 диапазон
    {
        SetState(ch, false);
        Attenuator::SetAttenuation(ch, Attenuation::_10Db);
    }
    else if(amplitude > 0.0316F)        // 6 диапазон
    {
        SetState(ch, false);
        Attenuator::SetAttenuation(ch, Attenuation::_20Db);
    }
    else                                // 7 диапазон
    {
        SetState(ch, false);
        Attenuator::SetAttenuation(ch, Attenuation::_30Db);
    }
}


void Amplifier::SetState(Chan::E ch, bool state)
{
    static GPIO_TypeDef * const gpio[Chan::Count] = { GPIOF, GPIOC };
    static const uint16         pin[Chan::Count] = { GPIO_PIN_0, GPIO_PIN_14 };

    HAL_GPIO_WritePin(gpio[ch], pin[ch], state ? GPIO_PIN_SET : GPIO_PIN_RESET);
}
