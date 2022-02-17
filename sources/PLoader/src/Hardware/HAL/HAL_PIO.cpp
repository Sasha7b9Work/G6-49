#include "defines.h"
#include "Hardware/HAL/HAL_PIO.h"
#include <stm32f4xx_hal.h>

const uint16 HPin::_0 = GPIO_PIN_0;
const uint16 HPin::_1 = GPIO_PIN_1;
const uint16 HPin::_2 = GPIO_PIN_2;
const uint16 HPin::_3 = GPIO_PIN_3;
const uint16 HPin::_4 = GPIO_PIN_4;
const uint16 HPin::_5 = GPIO_PIN_5;
const uint16 HPin::_6 = GPIO_PIN_6;
const uint16 HPin::_7 = GPIO_PIN_7;
const uint16 HPin::_8 = GPIO_PIN_8;
const uint16 HPin::_9 = GPIO_PIN_9;
const uint16 HPin::_10 = GPIO_PIN_10;
const uint16 HPin::_11 = GPIO_PIN_11;
const uint16 HPin::_12 = GPIO_PIN_12;
const uint16 HPin::_13 = GPIO_PIN_13;
const uint16 HPin::_14 = GPIO_PIN_14;
const uint16 HPin::_15 = GPIO_PIN_15;


static GPIO_TypeDef *const ports[] = { GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG, GPIOH, GPIOI, GPIOJ, GPIOK };

static const uint modes[HMode::Count] =
{
    GPIO_MODE_ANALOG,
    GPIO_MODE_OUTPUT_PP,
    GPIO_MODE_IT_RISING,
    GPIO_MODE_INPUT,
    GPIO_MODE_AF_PP
};

static const uint pulles[HPull::Count] =
{
    GPIO_NOPULL,
    GPIO_PULLDOWN,
    GPIO_PULLUP
};

static const uint speedes[HSpeed::Count] =
{
    GPIO_SPEED_FREQ_LOW,
    GPIO_SPEED_FREQ_HIGH,
    GPIO_SPEED_FREQ_VERY_HIGH
};

static const uint alternates[HAlternate::Count] =
{
    GPIO_AF0_MCO,
    GPIO_AF5_SPI1,
    GPIO_AF7_USART3,
    GPIO_AF10_OTG_FS,
    GPIO_AF12_OTG_HS_FS
};

#define MODE(m)      (modes[m])
#define PULL(p)      (pulles[p])
#define SPEED(s)     (speedes[s])
#define ALTERNATE(a) (alternates[a])
#define PORT(p)      (ports[(p) - 'A'])


void HAL_PIO::WritePin(char port, uint16 maskPin, bool state)
{
    HAL_GPIO_WritePin(ports[port - 'A'], maskPin, state ? GPIO_PIN_SET : GPIO_PIN_RESET);
}


bool HAL_PIO::ReadPin(char port, uint16 maskPin)
{
    return HAL_GPIO_ReadPin(ports[port - 'A'], maskPin) == GPIO_PIN_SET;
}


void HAL_PIO::Init(char port, uint pins, HMode::E mode, HPull::E pull, HSpeed::E speed, HAlternate::E alternate)
{
    GPIO_InitTypeDef isGPIO =
    {
        pins,
        MODE(mode),
        PULL(pull),
        SPEED(speed),
        ALTERNATE(alternate)
    };

    HAL_GPIO_Init(PORT(port), &isGPIO);
}
