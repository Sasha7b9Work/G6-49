#include "defines.h"
#include "Display.h"
#include "Display/Painter.h"
#include "Display/Font/Font.h"
#include "InputWindow.h"
#include "InputWindowStruct.h"
#include "Keyboard/Keyboard.h"
#include "Menu/Menu.h"
#include "Menu/MenuItems.h"
#include "Settings/Settings.h"
#include "Settings/SettingsSignals.h"
#include "Settings/SettingsTypes.h"
#include <stm32f4xx_hal.h>
#include <math.h>
#include <string.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void DrawSignal(Channel chan);
static void DrawSignalUGO(Channel chan, int y0);
static void DrawSignalParameters(Channel chan, int y0);
static void DrawParameterValue(Channel chan, WaveParameter parameter, int x, int y);
static void DrawConsole();

static const char *text = 0;

#define STRING_IN_CONSOLE 29
#define SYMBOLS_IN_STRING 64
static char bufferConsole[STRING_IN_CONSOLE][SYMBOLS_IN_STRING] = {};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static LTDC_HandleTypeDef hltdc;
static uint8 frontBuffer[320 * 240];       // Это экран
static uint8 backBuffer[320 * 240];        // Зто задний буфер. В нём происходит отрисовка, и затем изображение копируется во frontBuffer


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Display_Init(void)
{
    LTDC_LayerCfgTypeDef pLayerCfg;

    hltdc.Instance = LTDC;
    hltdc.Init.HSPolarity = LTDC_HSPOLARITY_AL;
    hltdc.Init.VSPolarity = LTDC_VSPOLARITY_AL;
    hltdc.Init.DEPolarity = LTDC_DEPOLARITY_AH;
    hltdc.Init.PCPolarity = LTDC_PCPOLARITY_IIPC;
    hltdc.Init.HorizontalSync = 0;
    hltdc.Init.VerticalSync = 0;
    hltdc.Init.AccumulatedHBP = 70;
    hltdc.Init.AccumulatedVBP = 13;
    hltdc.Init.AccumulatedActiveW = 390;
    hltdc.Init.AccumulatedActiveH = 253;
    hltdc.Init.TotalWidth = 408;
    hltdc.Init.TotalHeigh = 263;
    hltdc.Init.Backcolor.Blue = 0;
    hltdc.Init.Backcolor.Green = 0;
    hltdc.Init.Backcolor.Red = 0;
    if (HAL_LTDC_Init(&hltdc) != HAL_OK)
    {
      _Error_Handler(__FILE__, __LINE__);
    }

    pLayerCfg.WindowX0 = 0;
    pLayerCfg.WindowX1 = 320;
    pLayerCfg.WindowY0 = 0;
    pLayerCfg.WindowY1 = 240;
    pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_L8;
    pLayerCfg.Alpha = 127;
    pLayerCfg.Alpha0 = 127;
    pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_CA;
    pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_CA;
    pLayerCfg.FBStartAdress = (uint32_t)frontBuffer;
    pLayerCfg.ImageWidth = 320;
    pLayerCfg.ImageHeight = 240;
    pLayerCfg.Backcolor.Blue = 0;
    pLayerCfg.Backcolor.Green = 0;
    pLayerCfg.Backcolor.Red = 0;
    if (HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg, 0) != HAL_OK)
    {
      _Error_Handler(__FILE__, __LINE__);
    }

    uint clut[10] =
    {
        0x00000000,
        0x00ffffff,
        0x00a0a0a0,
        0x000000ff
    };

    HAL_LTDC_ConfigCLUT(&hltdc, clut, 10, 0);

    HAL_LTDC_EnableCLUT(&hltdc, 0);

    GPIO_InitTypeDef initStr;
    initStr.Pin = GPIO_PIN_6;
    initStr.Mode = GPIO_MODE_OUTPUT_PP;
    initStr.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &initStr);

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);         // Включение подсветки

}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Display_SetColorBackground(void)
{
    if (set.serv_bacgroundBlack)
    {
        uint clut[10] =
        {
            0x00000000,
            0x00ffffff,
            0x00a0a0a0,
            0x000000ff
        };

        HAL_LTDC_ConfigCLUT(&hltdc, clut, 10, 0);
    }
    else
    {
        uint clut[10] =
        {
            0x00ffffff,
            0x00000000,
            0x00a0a0a0,
            0x000000ff
        };

        HAL_LTDC_ConfigCLUT(&hltdc, clut, 10, 0);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Display_ToggleBuffers(void)
{
    uint destination = (uint)frontBuffer;
    uint source = (uint)backBuffer;

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
            if (HAL_DMA2D_Start(&hDMA2D, source, destination, 320, 240) == HAL_OK)
            {
                HAL_DMA2D_PollForTransfer(&hDMA2D, 100);
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
uint8 *Display_GetBuffer(void)
{
    return backBuffer;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Display_Update(void)
{
    Painter_BeginScene(COLOR_BACK);

    Painter_SetColor(COLOR_FILL);

    DrawSignal(A);
    DrawSignal(B);
    
    gMenu.Draw();

    if (ADDITION_PAGE_IS_INPUT)
    {
        InputWindow_Draw();
    }

    Painter_DrawRectangleC(0, 0, 318, 238, COLOR_FILL);

    if (text)
    {
        Painter_DrawText(75, 4, text);
    }

    DrawConsole();

    Painter_EndScene();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Display_SetButton(Control control)
{
    text = ControlName(control);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawSignal(Channel ch)
{
    if (CHANNEL_ENABLED(ch))
    {
        int y0 = (ch == A) ? HEIGHT_TITLE : HEIGHT_TITLE + HEIGHT_SIGNAL;
        Painter_DrawRectangleC(0, y0, WIDTH_SIGNAL, HEIGHT_SIGNAL, COLOR_FILL);

        Painter_DrawBigText(5, y0 + 5, 2, (ch == A) ? "1" : "2");

        DrawSignalUGO(ch, y0);

        DrawSignalParameters(ch, y0);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawSignalUGO(Channel chan, int y0)
{
    y0 += 30;
    int height = 50;
    int width = 90;
    int aveY = y0 + 5 + height / 2;
    int minY = y0 + 5;
    int maxY = minY + height;
    int x0 = 10;

    Painter_DrawVLine(x0, minY, maxY);
    Painter_DrawHLine(aveY, x0, x0 + width);

    WaveForm form = WAVE_FORM_CH(chan);

    if (form == Form_Sine)
    {
        float speed = 0.1f;
        int delta = 1;

        for (int i = delta; i < width; i++)
        {
            int y1 = aveY - (int)(sinf((i - delta) * speed) * height / 2.0f);
            int y2 = aveY - (int)(sinf(i * speed) * height / 2.0f);

            Painter_DrawLine(x0 + i - delta, y1, x0 + i, y2);
            Painter_RunDisplay();
        }
    }
    else if (form == Form_Saw)
    {
        int dX = 30;
        for (int x = x0; x < x0 + 80; x += dX)
        {
            Painter_DrawLine(x, aveY, x + dX, minY);
            Painter_DrawLine(x + dX, aveY, x + dX, minY);
        }
    }
    else if (form == Form_Impulse)
    {
        int deltaX = 20;
        for (int i = 0; i < 5; i++)
        {
            Painter_DrawVLine(x0, minY, aveY);
            Painter_DrawHLine(minY, x0, x0 + 5);
            Painter_DrawVLine(x0 + 5, minY, aveY);
            x0 += deltaX;
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawSignalParameters(Channel ch, int y0)
{
    int x0 = 107;
    
    WaveForm form = WAVE_FORM_CH(ch);

    y0 += 5;

    Painter_DrawTextC(22, y0 + 3, NameWaveForm(form), COLOR_FILL);

    AllowableParameters allowParameters;
    IWS_FillAllowParameters(ch, form, &allowParameters);
    
    for (int i = 0; i < NumParameters; i++)
    {
        if (allowParameters.allow[i])
        {
            Color color = COLOR_FILL;
            if (ch == CHANNEL && strcmp(gMenu.NameCurrentParameter(), NameParameter((WaveParameter)i)) == 0)
            {
                Painter_FillRegionC(x0, y0, 139, 8, COLOR_FILL);
                color = COLOR_BACK;
            }
            Painter_DrawTextC(x0 + 1, y0, NameParameter((WaveParameter)i), color);

            DrawParameterValue(ch, (WaveParameter)i, x0 + 80, y0);

            y0 += 10;
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawParameterValue(Channel ch, WaveParameter param, int x, int y)
{
    WaveForm form = WAVE_FORM_CH(ch);

    x = Painter_DrawText(x, y, IWS_StringValue(&PARAMETER(ch, form, param)));

    char buffer[10];
    Painter_DrawText(x, y, NameUnit(buffer, PARAMETER_ORDER(ch, form, param), PARAMETER_UNIT(ch, form, param)));
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Display_AddStringToConsole(char *string)
{
    // Находим номер первой пустой строки
    int numEmptyString = 0;

    for (; numEmptyString < STRING_IN_CONSOLE; numEmptyString++)
    {
        if (!bufferConsole[numEmptyString][0])
        {
            break;
        }
    }


    // Если пустой строки нет
    if (numEmptyString == STRING_IN_CONSOLE)
    {
        // То сдвигаем все строки на одну
        for (int i = 1; i < STRING_IN_CONSOLE; i++)
        {
            strcpy(&bufferConsole[i - 1][0], &bufferConsole[i][0]);
        }
        numEmptyString = STRING_IN_CONSOLE - 1;
    }

    char tempBuffer[1000] = "";

    static int numLines = 0;

    sprintf(tempBuffer, "%d %s", numLines++, string);

    if (strlen(tempBuffer) > SYMBOLS_IN_STRING - 1)
    {
        tempBuffer[SYMBOLS_IN_STRING - 1] = 0;
    }
    
    strcpy(&bufferConsole[numEmptyString][0], tempBuffer);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawConsole(void)
{
    if (!CONSOLE_ENABLED)
    {
        return;
    }

    int y = 1;
    for (int i = 0; i < STRING_IN_CONSOLE; i++)
    {
        int length = Font_GetLengthText(&bufferConsole[i][0]);
        Painter_FillRegionC(2, y, length, 8, COLOR_BACK);
        Painter_DrawTextC(2, y, &bufferConsole[i][0], COLOR_FILL);
        y += 8;
    }
}
