#pragma clang diagnostic ignored "-Wpadded"
#include <stm32f4xx.h>
#pragma clang diagnostic warning "-Wpadded"
#include "CPU.h"
#include "Hardware/Timer.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static TIM_HandleTypeDef handleTIM2;

/// Для дисплея
static LTDC_HandleTypeDef handleLTDC;
/// Для связи с основным процессором
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
        SPI_BAUDRATEPRESCALER_2,
        SPI_FIRSTBIT_MSB,
        SPI_TIMODE_DISABLED,
        SPI_CRCCALCULATION_DISABLED,
        7
    },
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, HAL_UNLOCKED, HAL_SPI_STATE_RESET, 0
};

static uint frontBuffer = 0;
static uint backBuffer = 0;
static GPIO_TypeDef * const ports[] = {GPIOA, GPIOB, GPIOC, GPIOD, GPIOE};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CPU::Init()
{
    STM429::Init();
    
    EnablePeriphery();
    
    InitHardware();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CPU::EnablePeriphery()
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
void CPU::InitHardware()
{
    InitLTDC();

    InitFSMC();

    InitTIM2();

    InitSPI4();
    
    Timer::Init();

    Keyboard::Init();
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


    handleLTDC.Instance = LTDC;
    handleLTDC.Init.HSPolarity = LTDC_HSPOLARITY_AL;
    handleLTDC.Init.VSPolarity = LTDC_VSPOLARITY_AL;
    handleLTDC.Init.DEPolarity = LTDC_DEPOLARITY_AH;
    handleLTDC.Init.PCPolarity = LTDC_PCPOLARITY_IIPC;
    handleLTDC.Init.HorizontalSync = 0;
    handleLTDC.Init.VerticalSync = 0;
    handleLTDC.Init.AccumulatedHBP = 70;
    handleLTDC.Init.AccumulatedVBP = 13;
    handleLTDC.Init.AccumulatedActiveW = 390;
    handleLTDC.Init.AccumulatedActiveH = 253;
    handleLTDC.Init.TotalWidth = 408;
    handleLTDC.Init.TotalHeigh = 263;
    handleLTDC.Init.Backcolor.Blue = 0;
    handleLTDC.Init.Backcolor.Green = 0;
    handleLTDC.Init.Backcolor.Red = 0;
    if (HAL_LTDC_Init(&handleLTDC) != HAL_OK)
    {
        ERROR_HANDLER();
    }
    
    uint clut[10] =
    {
        0x00000000,
        0x00ffffff,
        0x00a0a0a0,
        0x000000ff
    };

    HAL_LTDC_ConfigCLUT(&handleLTDC, clut, 10, 0);

    HAL_LTDC_EnableCLUT(&handleLTDC, 0);

    GPIO_InitTypeDef initStr;
    initStr.Pin = GPIO_PIN_6;
    initStr.Mode = GPIO_MODE_OUTPUT_PP;
    initStr.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &initStr);

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);         // Включение подсветки
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CPU::LTDC_::SetBuffers(uint front, uint back)
{
    frontBuffer = front;
    backBuffer = back;

    LTDC_LayerCfgTypeDef pLayerCfg;

    pLayerCfg.WindowX0 = 0;
    pLayerCfg.WindowX1 = 320;
    pLayerCfg.WindowY0 = 0;
    pLayerCfg.WindowY1 = 240;
    pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_L8;
    pLayerCfg.Alpha = 127;
    pLayerCfg.Alpha0 = 127;
    pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_CA;
    pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_CA;
    pLayerCfg.FBStartAdress = frontBuffer;
    pLayerCfg.ImageWidth = 320;
    pLayerCfg.ImageHeight = 240;
    pLayerCfg.Backcolor.Blue = 0;
    pLayerCfg.Backcolor.Green = 0;
    pLayerCfg.Backcolor.Red = 0;
    if (HAL_LTDC_ConfigLayer(&handleLTDC, &pLayerCfg, 0) != HAL_OK)
    {
        ERROR_HANDLER();
    }
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
        ERROR_HANDLER();
    };
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CPU::InitTIM2()
{
    handleTIM2.Instance = TIM2;
    handleTIM2.Init.Prescaler = 0;
    handleTIM2.Init.CounterMode = TIM_COUNTERMODE_UP;
    handleTIM2.Init.Period = (uint)-1;
    handleTIM2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_Base_Init(&handleTIM2);
    HAL_TIM_Base_Start(&handleTIM2);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CPU::InitSPI4()
{
    GPIO_InitTypeDef isGPIO =
    {   //  CLK         MI           MO
        GPIO_PIN_2 | GPIO_PIN_5 | GPIO_PIN_6,
        GPIO_MODE_AF_PP,
        GPIO_NOPULL,
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

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CPU::LTDC_::SetColors(uint clut[], uint numColors)
{
    HAL_LTDC_ConfigCLUT(&handleLTDC, clut, numColors, 0);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CPU::LTDC_::ToggleBuffers()
{
    DMA2D_HandleTypeDef hDMA2D;

    hDMA2D.Init.Mode = DMA2D_M2M;
    hDMA2D.Init.ColorMode = DMA2D_INPUT_L8;
    hDMA2D.Init.OutputOffset = 0;

    hDMA2D.XferCpltCallback = NULL;

    hDMA2D.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
    hDMA2D.LayerCfg[1].InputAlpha = 0xFF;
    hDMA2D.LayerCfg[1].InputColorMode = DMA2D_INPUT_L8;
    hDMA2D.LayerCfg[1].InputOffset = 0;

    hDMA2D.Instance = DMA2D;

    if (HAL_DMA2D_Init(&hDMA2D) == HAL_OK)
    {
        if (HAL_DMA2D_ConfigLayer(&hDMA2D, 1) == HAL_OK)
        {
            if (HAL_DMA2D_Start(&hDMA2D, backBuffer, frontBuffer, 320, 240) == HAL_OK)
            {
                HAL_DMA2D_PollForTransfer(&hDMA2D, 100);
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CPU::SPI4_::TransmitReceive(uint8 *trans, uint8 *receiv, uint16 size, uint timeOut)
{
    HAL_SPI_TransmitReceive(&handleSPI4, trans, receiv, size, timeOut);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CPU::SPI4_::Transmit(uint8 *buffer, uint16 size, uint timeOut)
{
    HAL_SPI_Transmit(&handleSPI4, buffer, size, timeOut);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool CPU::SPI4_::IsBusy()
{
    return HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4) == GPIO_PIN_RESET;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CPU::GPIO_::WritePin(char port, uint16 maskPin, bool state)
{
    HAL_GPIO_WritePin(ports[port - 'A'], maskPin, state ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool CPU::GPIO_::ReadPin(char port, uint16 maskPin)
{
    return HAL_GPIO_ReadPin(ports[port - 'A'], maskPin) == GPIO_PIN_SET;
}
