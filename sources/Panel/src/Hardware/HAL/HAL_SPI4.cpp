#include "defines.h"
#include "Display/Console.h"
#include "Display/Painter.h"
#include "Hardware/Timer.h"
#include "log.h"
#include "Hardware/HAL/HAL.h"
#include <stm32f4xx_hal.h>


// Для связи с основным процессором
static SPI_HandleTypeDef handleSPI4 =
{
	SPI4,
	{
		SPI_MODE_MASTER,
		SPI_DIRECTION_2LINES,
		SPI_DATASIZE_8BIT,
		SPI_POLARITY_HIGH,
		SPI_PHASE_2EDGE,
		SPI_NSS_SOFT,
		SPI_BAUDRATEPRESCALER_32,
		SPI_FIRSTBIT_MSB,
		SPI_TIMODE_DISABLED,
		SPI_CRCCALCULATION_DISABLED,
		7
	},
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, HAL_UNLOCKED, HAL_SPI_STATE_RESET, 0
};



void HAL_SPI4::Init()
{
	GPIO_InitTypeDef isGPIO =
	{   //  CLK         MI           MO
		GPIO_PIN_2 | GPIO_PIN_5 | GPIO_PIN_6,
		GPIO_MODE_AF_PP,
		GPIO_PULLUP,
		GPIO_SPEED_HIGH,
		GPIO_AF5_SPI4
	};

	HAL_GPIO_Init(GPIOE, &isGPIO);

	HAL_SPI_Init(&handleSPI4);

	// На этом пине будем читать занятость процессора генератора
	isGPIO.Pin = GPIO_PIN_4;
	isGPIO.Mode = GPIO_MODE_INPUT;
	isGPIO.Alternate = 0;
	HAL_GPIO_Init(GPIOE, &isGPIO);
}


bool HAL_SPI4::Transmit(const void *buffer, int size, int timeout)
{
	if (HAL_SPI_Transmit(&handleSPI4, static_cast<uint8 *>(const_cast<void *>(buffer)), static_cast<uint16>(size), static_cast<uint>(timeout)) != HAL_OK)
	{
		return false;
	}

	return true;
}


bool HAL_SPI4::Transmit(int value, int timeout)
{
    return Transmit(&value, 4, timeout);
}


bool HAL_SPI4::Receive(void *recv, int size, int timeout)
{
	if (HAL_SPI_Receive(&handleSPI4, static_cast<uint8 *>(recv), static_cast<uint16>(size), static_cast<uint>(timeout)) != HAL_OK)
	{
		return false;
	}

	return true;
}


uint HAL_SPI4::ReceiveAndCompare(const void *compared, int size)
{
    uint result = 0;

    uint8 byte = 0;

    uint8 *data = static_cast<uint8 *>(const_cast<void *>(compared));

    for (int i = 0; i < size; i++)
    {
        if (Receive(&byte, 1, 10) && data[i] != byte)
        {
            result++;
        }
    }

    return result;
}



void HAL_SPI4::WaitFreedom()
{
	while (!IsReady())
	{
	}
}


void HAL_SPI4::WaitFalling()
{
    while (IsReady())
    {
    };   // Если попали в время сигнала готовности, пропустим его, чтобы транзакция гарантированно начиналась после разрешающего фронта

    while (!IsReady())
    {
    };  // Теперь ожидаем, когда придёт сигнал готовности
}



bool HAL_SPI4::IsReady()
{
	return HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4) == GPIO_PIN_SET;
}
