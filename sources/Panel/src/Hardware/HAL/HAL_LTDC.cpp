#include "defines.h"
#include "LTDC.h"
#include "CPU.h"
#include "Display/Painter.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Settings/Settings.h"
#include <string.h>


static LTDC_HandleTypeDef handleLTDC;
static DMA2D_HandleTypeDef hDMA2D;

static uint frontBuffer = 0;
static uint backBuffer = 0;



static void SetBuffers(uint frontBuffer, uint backBuffer);


void HAL_LTDC::Init(uint front, uint back)
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

    GPIO_InitTypeDef initStr;
    initStr.Pin = GPIO_PIN_6;
    initStr.Mode = GPIO_MODE_OUTPUT_PP;
    initStr.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &initStr);

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);         // Включение подсветки

    SetBuffers(front, back);

    hDMA2D.Init.ColorMode = DMA2D_INPUT_L8;
    hDMA2D.Init.OutputOffset = 0;

    hDMA2D.XferCpltCallback = NULL;
    
    hDMA2D.LayerCfg[0].AlphaMode = DMA2D_NO_MODIF_ALPHA;
    hDMA2D.LayerCfg[0].InputAlpha = 0xff;
    hDMA2D.LayerCfg[0].InputColorMode = DMA2D_INPUT_L8;
    hDMA2D.LayerCfg[0].InputOffset = 0;
    
    hDMA2D.Instance = DMA2D;
}


static void SetBuffers(uint front, uint back)
{
    frontBuffer = front;
    backBuffer = back;

    LTDC_LayerCfgTypeDef pLayerCfg;

    pLayerCfg.WindowX0 = 0;
    pLayerCfg.WindowX1 = SCREEN_WIDTH;
    pLayerCfg.WindowY0 = 0;
    pLayerCfg.WindowY1 = SCREEN_HEIGHT;
    pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_L8;
    pLayerCfg.Alpha = 0xff;
    pLayerCfg.Alpha0 = 0xff;
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


void HAL_LTDC::SetColors(uint *clut, uint8 numColors)
{
    HAL_LTDC_ConfigCLUT(&handleLTDC, clut, numColors, 0);
    
    HAL_LTDC_EnableCLUT(&handleLTDC, 0);
}


void HAL_LTDC::ToggleBuffers()
{
    hDMA2D.Init.Mode = DMA2D_M2M;

    if (HAL_DMA2D_Init(&hDMA2D) == HAL_OK)
    {
        if (HAL_DMA2D_ConfigLayer(&hDMA2D, 1) == HAL_OK)
        {
            if (HAL_DMA2D_Start(&hDMA2D, backBuffer, frontBuffer, SCREEN_WIDTH, SCREEN_HEIGHT) == HAL_OK)
            {
                HAL_DMA2D_PollForTransfer(&hDMA2D, 1);
            }
        }
    }
}


void HAL_LTDC::FillRegion(int, int, int, int, Color color)
{
    /// \todo Не работает функция

    DMA2D_HandleTypeDef handle;

    handle.Init.Mode = DMA2D_R2M;
    handle.Init.ColorMode = DMA2D_INPUT_L8;
    handle.Init.OutputOffset = 0;

    handle.XferCpltCallback = NULL;

    handle.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
    handle.LayerCfg[1].InputAlpha = 0xff;
    handle.LayerCfg[1].InputColorMode = DMA2D_INPUT_L8;
    handle.LayerCfg[1].InputOffset = 0;

    handle.Instance = DMA2D;

    if (HAL_DMA2D_Init(&handle) == HAL_OK)
    {
        if (HAL_DMA2D_ConfigLayer(&handle, 1) == HAL_OK)
        {
            if (HAL_DMA2D_Start(&handle, color.value, backBuffer, BUFFER_WIDTH, BUFFER_HEIGHT) == HAL_OK)
            {
                HAL_DMA2D_PollForTransfer(&handle, 200);
            }
        }
    }
}
