#include "defines.h"
#include "Hardware/CPU.h"
#include "Hardware/HAL/HAL.h"
#include <stm32f4xx_hal.h>


static DAC_HandleTypeDef handleDAC = { DAC };


void *HAL_DAC2::handle = &handleDAC;


void HAL_DAC2::Init()
{
	__DMA1_CLK_ENABLE();
	__TIM7_CLK_ENABLE();
	__DAC_CLK_ENABLE();

    GPIO_InitTypeDef structGPIO =
    {
        GPIO_PIN_5,
        GPIO_MODE_ANALOG,
        GPIO_NOPULL,
        0, 0
    };

    HAL_GPIO_Init(GPIOA, &structGPIO);

	DAC_ChannelConfTypeDef config =
	{
		DAC_TRIGGER_T7_TRGO,
		DAC_OUTPUTBUFFER_ENABLE
	};

	HAL_DAC_DeInit(&handleDAC);

	HAL_DAC_Init(&handleDAC);

	HAL_DAC_ConfigChannel(&handleDAC, &config, DAC_CHANNEL_1);

    static DMA_HandleTypeDef hdmaDAC1 =
    {
        DMA1_Stream5,
        {
            DMA_CHANNEL_7,
            DMA_MEMORY_TO_PERIPH,
            DMA_PINC_DISABLE,
            DMA_MINC_ENABLE,
            DMA_PDATAALIGN_BYTE,
            DMA_MDATAALIGN_BYTE,
            DMA_CIRCULAR,
            DMA_PRIORITY_HIGH,
            DMA_FIFOMODE_DISABLE,
            DMA_FIFO_THRESHOLD_HALFFULL,
            DMA_MBURST_SINGLE,
            DMA_PBURST_SINGLE
        }
    };

    HAL_DMA_Init(&hdmaDAC1);

    __HAL_LINKDMA(&handleDAC, DMA_Handle1, hdmaDAC1);

    HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 7, 0);
    HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
}


#ifdef __cplusplus
extern "C" {
#endif


void DMA1_Stream5_IRQHandler()
{
	HAL_DMA_IRQHandler(handleDAC.DMA_Handle1);
}

#ifdef __cplusplus
}
#endif
