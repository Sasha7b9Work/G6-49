#include <stm32f4xx.h>
#include "CPU.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void STM32::EnablePeriphery()
{
    __HAL_RCC_SPI4_CLK_ENABLE();

    __HAL_RCC_TIM2_CLK_ENABLE();    // Для тиков
    __HAL_RCC_TIM3_CLK_ENABLE();    // Для таймеров
    __HAL_RCC_TIM5_CLK_ENABLE();    // Для миллисекунд

    __HAL_RCC_LTDC_CLK_ENABLE();
    __HAL_RCC_DMA2D_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void STM32::InitHardware()
{
    CPU::InitLTDC();

    CPU::InitFSMC();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CPU::InitLTDC()
{
    GPIO_InitTypeDef isGPIO =
    {
        //  R3         R6
        GPIO_PIN_0 | GPIO_PIN_1,
        GPIO_MODE_AF_PP,
        GPIO_NOPULL,
        GPIO_SPEED_FREQ_LOW,
        GPIO_AF9_LTDC
    };
    HAL_GPIO_Init(GPIOB, &isGPIO);

    //              B5          VSYNC         G2            R4          R5
    isGPIO.Pin = GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_6 | GPIO_PIN_11 | GPIO_PIN_12;
    isGPIO.Alternate = GPIO_AF14_LTDC;
    HAL_GPIO_Init(GPIOA, &isGPIO);

    //              G4             G5            B6          B7
    isGPIO.Pin = GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_8 | GPIO_PIN_9;
    HAL_GPIO_Init(GPIOB, &isGPIO);

    //              HSYNC         G6           R2
    isGPIO.Pin = GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_10;
    HAL_GPIO_Init(GPIOC, &isGPIO);

    //                B3          G7          B2
    isGPIO.Pin = GPIO_PIN_10 | GPIO_PIN_3 | GPIO_PIN_6;
    HAL_GPIO_Init(GPIOD, &isGPIO);

    //               G3             B4           DE            CLK           R7
    isGPIO.Pin = GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
    HAL_GPIO_Init(GPIOE, &isGPIO);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CPU::InitFSMC(void)
{
    /// \todo временно изменил - возможно, на флешку не пишет из-за неправильных таймингов
    static const FMC_NORSRAM_TimingTypeDef sramTiming =
    {
        6,                  ///< FSMC_AddressSetupTime
        8,                  ///< FSMC_AddressHoldTime
        14,                 ///< FSMC_DataSetupTime
        12,                 ///< FSMC_BusTurnAroundDuration
        3,                  ///< FSMC_CLKDivision
        0,                  ///< FSMC_DataLatency
        FMC_ACCESS_MODE_B   ///< FSMC_AccessMode
    };

    static const SRAM_HandleTypeDef sramHandle =
    {
        FMC_NORSRAM_DEVICE,
        FMC_NORSRAM_EXTENDED_DEVICE,
    {
        FMC_NORSRAM_BANK1,                 ///< Init.NSBank
        FMC_DATA_ADDRESS_MUX_ENABLE,       ///< Init.DataAddressMux
        FMC_MEMORY_TYPE_NOR,               ///< Init.MemoryType
        FMC_NORSRAM_MEM_BUS_WIDTH_8,       ///< Init.MemoryDataWidth
        FMC_BURST_ACCESS_MODE_DISABLE,     ///< Init.BurstAccessMode
        FMC_WAIT_SIGNAL_POLARITY_LOW,      ///< Init.WaitSignalPolarity
        FMC_WRAP_MODE_DISABLE,             ///< Init.WrapMode
        FMC_WAIT_TIMING_BEFORE_WS,         ///< Init.WaitSignalActive
        FMC_WRITE_OPERATION_ENABLE,        ///< Init.WriteOperation
        FMC_WAIT_SIGNAL_DISABLE,           ///< Init.WaitSignal
        FMC_EXTENDED_MODE_DISABLE,         ///< Init.ExtendedMode
        FMC_ASYNCHRONOUS_WAIT_DISABLE,     ///< Init.AsynchronousWait
        FMC_WRITE_BURST_DISABLE,           ///< Init.WriteBurst
        0, 0, 0
    },
        HAL_UNLOCKED, HAL_SRAM_STATE_RESET, 0
    };

    if (HAL_SRAM_Init((SRAM_HandleTypeDef*)(&sramHandle), (FMC_NORSRAM_TimingTypeDef*)(&sramTiming),
        (FMC_NORSRAM_TimingTypeDef*)(&sramTiming)) != HAL_OK)
    {
        ERROR_HANDLER;
    };
}
