#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include <stm32f4xx_hal.h>


static GPIO_TypeDef *const ports[] = { GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG, GPIOH, GPIOI, GPIOJ, GPIOK };


void HAL_PIO::WritePin(char port, uint16 maskPin, bool state)
{
    HAL_GPIO_WritePin(ports[port - 'A'], maskPin, state ? GPIO_PIN_SET : GPIO_PIN_RESET);
}


bool HAL_PIO::ReadPin(char port, uint16 maskPin)
{
    return HAL_GPIO_ReadPin(ports[port - 'A'], maskPin) == GPIO_PIN_SET;
}
