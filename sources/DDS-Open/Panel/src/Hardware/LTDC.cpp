#include "defines.h"
#include "LTDC.h"
#include "CPU.h"
#include "Display/Painter.h"
#include "Hardware/Timer.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static LTDC_HandleTypeDef handleLTDC;
static uint frontBuffer = 0;
static uint backBuffer = 0;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void LTDC_::Init(uint front, uint back)
{
    
#ifdef STM32F429xx
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

    GPIO_InitTypeDef initStr;
    initStr.Pin = GPIO_PIN_6;
    initStr.Mode = GPIO_MODE_OUTPUT_PP;
    initStr.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &initStr);

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);         // Включение подсветки
#endif
    
#ifdef STM32F746xx
    GPIO_InitTypeDef GPIO_InitStruct;
    __LTDC_CLK_ENABLE();
  
    /**LTDC GPIO Configuration    
    PE4     ------> LTDC_B0
    PE5     ------> LTDC_G0
    PE6     ------> LTDC_G1
    PI9     ------> LTDC_VSYNC
    PI10     ------> LTDC_HSYNC
    PF10     ------> LTDC_DE
    PC0     ------> LTDC_R5
    PH2     ------> LTDC_R0
    PH3     ------> LTDC_R1
    PB1     ------> LTDC_R6
    PH8     ------> LTDC_R2
    PH9     ------> LTDC_R3
    PH10     ------> LTDC_R4
    PG6     ------> LTDC_R7
    PG7     ------> LTDC_CLK
    PH13     ------> LTDC_G2
    PH15     ------> LTDC_G4
    PI0     ------> LTDC_G5
    PI1     ------> LTDC_G6
    PI2     ------> LTDC_G7
    PD6     ------> LTDC_B2
    PG10     ------> LTDC_G3
    PG11     ------> LTDC_B3
    PG12     ------> LTDC_B1
    PI4     ------> LTDC_B4
    PI5     ------> LTDC_B5
    PI6     ------> LTDC_B6
    PI7     ------> LTDC_B7 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_0|GPIO_PIN_1 
                          |GPIO_PIN_2|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6 
                          |GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
    HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_8|GPIO_PIN_9 
                          |GPIO_PIN_10|GPIO_PIN_13|GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
    HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_LTDC;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_11|GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_LTDC;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

    /* Peripheral interrupt init*/
    HAL_NVIC_SetPriority(LTDC_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(LTDC_IRQn);
    
      LTDC_LayerCfgTypeDef pLayerCfg;
  LTDC_LayerCfgTypeDef pLayerCfg1;

  handleLTDC.Instance = LTDC;
  handleLTDC.Init.HSPolarity = LTDC_HSPOLARITY_AL;
  handleLTDC.Init.VSPolarity = LTDC_VSPOLARITY_AL;
  handleLTDC.Init.DEPolarity = LTDC_DEPOLARITY_AL;
  handleLTDC.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
  handleLTDC.Init.HorizontalSync = 19;
  handleLTDC.Init.VerticalSync = 2;
  handleLTDC.Init.AccumulatedHBP = 159;
  handleLTDC.Init.AccumulatedVBP = 22;
  handleLTDC.Init.AccumulatedActiveW = 1183;
  handleLTDC.Init.AccumulatedActiveH = 622;
  handleLTDC.Init.TotalWidth = 1343;
  handleLTDC.Init.TotalHeigh = 634;
  handleLTDC.Init.Backcolor.Blue = 0;
  handleLTDC.Init.Backcolor.Green = 0;
  handleLTDC.Init.Backcolor.Red = 0;
  HAL_LTDC_Init(&handleLTDC);

  pLayerCfg.WindowX0 = 0;
  pLayerCfg.WindowX1 = SCREEN_WIDTH;
  pLayerCfg.WindowY0 = 0;
  pLayerCfg.WindowY1 = SCREEN_HEIGHT;
  pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_RGB565;
  pLayerCfg.Alpha = 0xFF;
  pLayerCfg.Alpha0 = 0;
  pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
  pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
  pLayerCfg.FBStartAdress = 0xD0000000;
  pLayerCfg.ImageWidth = SCREEN_WIDTH;
  pLayerCfg.ImageHeight = SCREEN_HEIGHT;
  pLayerCfg.Backcolor.Blue = 0;
  pLayerCfg.Backcolor.Green = 0;
  pLayerCfg.Backcolor.Red = 0;
  HAL_LTDC_ConfigLayer(&handleLTDC, &pLayerCfg, 0);

  pLayerCfg1.WindowX0 = 0;
  pLayerCfg1.WindowX1 = SCREEN_WIDTH;
  pLayerCfg1.WindowY0 = 0;
  pLayerCfg1.WindowY1 = SCREEN_HEIGHT;
  pLayerCfg1.PixelFormat = LTDC_PIXEL_FORMAT_RGB565;
  pLayerCfg1.Alpha = 0;
  pLayerCfg1.Alpha0 = 0;
  pLayerCfg1.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
  pLayerCfg1.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
  pLayerCfg1.FBStartAdress = 0xD0200000;
  pLayerCfg1.ImageWidth = SCREEN_WIDTH;
  pLayerCfg1.ImageHeight = SCREEN_HEIGHT;
  pLayerCfg1.Backcolor.Blue = 0;
  pLayerCfg1.Backcolor.Green = 0;
  pLayerCfg1.Backcolor.Red = 0;
#endif

    SetBuffers(front, back);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void LTDC_::SetBuffers(uint front, uint back)
{
    frontBuffer = front;
    backBuffer = back;

    LTDC_LayerCfgTypeDef pLayerCfg;

    pLayerCfg.WindowX0 = 0;
    pLayerCfg.WindowX1 = SCREEN_WIDTH;
    pLayerCfg.WindowY0 = 0;
    pLayerCfg.WindowY1 = SCREEN_HEIGHT;
    pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_L8;
    pLayerCfg.Alpha = 255;
    pLayerCfg.Alpha0 = 255;
    pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_CA;
    pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_CA;
    pLayerCfg.FBStartAdress = frontBuffer;
    pLayerCfg.ImageWidth = SCREEN_WIDTH;
    pLayerCfg.ImageHeight = SCREEN_HEIGHT;
    pLayerCfg.Backcolor.Blue = 0;
    pLayerCfg.Backcolor.Green = 0;
    pLayerCfg.Backcolor.Red = 0;
    if (HAL_LTDC_ConfigLayer(&handleLTDC, &pLayerCfg, 0) != HAL_OK)
    {
        ERROR_HANDLER();
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void LTDC_::SetColors(uint clut[], uint8 numColors)
{
    HAL_LTDC_ConfigCLUT(&handleLTDC, clut, numColors, 0);
    
    HAL_LTDC_EnableCLUT(&handleLTDC, 0);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void LTDC_::ToggleBuffers()
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
            if (HAL_DMA2D_Start(&hDMA2D, backBuffer, frontBuffer, SCREEN_WIDTH, SCREEN_HEIGHT) == HAL_OK)
            {
                HAL_DMA2D_PollForTransfer(&hDMA2D, 100);
            }
        }
    }
}
