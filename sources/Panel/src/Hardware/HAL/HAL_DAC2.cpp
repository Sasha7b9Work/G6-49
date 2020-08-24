#include "defines.h"
#include "Hardware/CPU.h"
#include "Hardware/HAL/HAL.h"
#include <stm32f4xx_hal.h>


static DAC_HandleTypeDef handleDAC = { DAC };


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
}


void HAL_DAC2::StartDMA(uint prescaler)
{
	ConfigTIM7(prescaler);

	DAC_ChannelConfTypeDef config =
	{
		DAC_TRIGGER_T7_TRGO,
		DAC_OUTPUTBUFFER_ENABLE
	};

	/*##-1- Initialize the DAC peripheral ######################################*/
	if(HAL_DAC_Init(&handleDAC) != HAL_OK)
	{
		ERROR_HANDLER();
	}

	/*##-2- DAC channel2 Configuration #########################################*/
	config.DAC_Trigger = DAC_TRIGGER_T7_TRGO;
	config.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;

	if(HAL_DAC_ConfigChannel(&handleDAC, &config, DAC_CHANNEL_2) != HAL_OK)
	{
		/* Channel configuration Error */
		ERROR_HANDLER();
	}

	/*##-3- DAC channel2 Triangle Wave generation configuration ################*/
	if(HAL_DACEx_TriangleWaveGenerate(&handleDAC, DAC_CHANNEL_2, DAC_TRIANGLEAMPLITUDE_4095) != HAL_OK)
	{
		/* Triangle wave generation Error */
		ERROR_HANDLER();
	}

	/*##-4- Enable DAC Channel1 ################################################*/
	if(HAL_DAC_Start(&handleDAC, DAC_CHANNEL_2) != HAL_OK)
	{
		/* Start Error */
		ERROR_HANDLER();
	}

	/*##-5- Set DAC channel1 DHR12RD register ##################################*/
	if(HAL_DAC_SetValue(&handleDAC, DAC_CHANNEL_2, DAC_ALIGN_12B_R, (1 << 12) - 1) != HAL_OK)
	{
		/* Setting value Error */
		ERROR_HANDLER();
	}
}


void HAL_DAC2::StopDMA()
{
	HAL_DAC_Stop_DMA(&handleDAC, DAC_CHANNEL_2);
}


void HAL_DAC2::ConfigTIM7(uint period)
{
	static TIM_HandleTypeDef htim;
	TIM_MasterConfigTypeDef  sMasterConfig;

	/*##-1- Configure the TIM peripheral #######################################*/
	/* Time base configuration */
	htim.Instance = TIM7;

	htim.Init.Period = 0x1;
	htim.Init.Prescaler = 0;
	htim.Init.ClockDivision = 0;
	htim.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim.Init.RepetitionCounter = 10;
	HAL_TIM_Base_Init(&htim);

	/* TIM6 TRGO selection */
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

	HAL_TIMEx_MasterConfigSynchronization(&htim, &sMasterConfig);

	/*##-2- Enable TIM peripheral counter ######################################*/
	HAL_TIM_Base_Start(&htim);
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
