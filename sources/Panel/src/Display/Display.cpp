#include "defines.h"
#include "Display.h"
#include "DisplayTypes.h"
#include "Display/Painter.h"
#include "Display/Font/Font.h"
#include "Hardware/CPU.h"
#include "Hardware/LTDC.h"
#include "InputWindow.h"
#include "InputWindowStruct.h"
#include "Menu/Menu.h"
#include "Menu/MenuItems.h"
#include "Settings/Settings.h"
#include "Settings/SettingsDisplay.h"
#include "Settings/SettingsSignals.h"
#include "Settings/SettingsTypes.h"
#include <math.h>
#include <string.h>
#include <stdio.h>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const char *Display::text = 0;
char        Display::bufferConsole[STRING_IN_CONSOLE][SYMBOLS_IN_STRING] = {};
#ifdef STM32F429xx
uint8       *Display::frontBuffer = malloc(SCREEN_WIDTH * SCREEN_HEIGHT);
uint8       *Display::backBuffer = malloc(SCREEN_WIDTH * SCREEN_HEIGHT);
#endif
    
#ifdef STM32F746xx
uint8       *Display::frontBuffer = (uint8 *)SDRAM_DEVICE_ADDR;
uint8       *Display::backBuffer = (uint8 *)(SDRAM_DEVICE_ADDR + SCREEN_HEIGHT * SCREEN_WIDTH);
#endif


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Display::Init()
{
    LTDC_::Init((uint)frontBuffer, (uint)backBuffer);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
uint8 *Display::GetBuffer(void)
{
    return backBuffer;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Display::Update(void)
{
    Painter::BeginScene(Color::BACK);

    Painter::SetColor(Color::FILL);
    
    DrawSignal(A);

    DrawSignal(B);
    
    Menu::Draw();

    if (ADDITION_PAGE_IS_INPUT)
    {
        InputWindow::Draw();
    }

    Painter::DrawRectangle(0, 0, 318, 238, Color::FILL);

    if (text)
    {
        Painter::DrawText(75, 4, text);
    }

    DrawFrequencyCounter();

    DrawConsole();

    Painter::EndScene();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Display::SetButton(PanelControl)
{
    //text = PanelControlName(control);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Display::DrawSignal(Channel ch)
{
    if (CHANNEL_ENABLED(ch))
    {
        int y0 = (ch == A) ? MP_TITLE_HEIGHT : MP_TITLE_HEIGHT + SIGNAL_HEIGHT;
        Painter::DrawRectangle(0, y0, SIGNAL_WIDTH, SIGNAL_HEIGHT, Color::FILL);

        Painter::DrawBigText(5, y0 + 5, 2, (ch == A) ? "1" : "2");

        DrawSignalUGO(ch, y0);

        DrawSignalParameters(ch, y0);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Display::DrawSignalUGO(Channel chan, int y0)
{
    y0 += 30;
    int height = 50;
    int width = 90;
    int aveY = y0 + 5 + height / 2;
    int minY = y0 + 5;
    int maxY = minY + height;
    int x0 = 10;

    Painter::DrawVLine(x0, minY, maxY);
    Painter::DrawHLine(aveY, x0, x0 + width);

    WaveForm form = WAVE_FORM_CH(chan);

    if (form == Form_Sine)
    {
        float speed = 0.1f;
        int delta = 1;

        for (int i = delta; i < width; i++)
        {
            int y1 = aveY - (int)(sinf((i - delta) * speed) * height / 2.0f);
            int y2 = aveY - (int)(sinf(i * speed) * height / 2.0f);

            Painter::DrawLine(x0 + i - delta, y1, x0 + i, y2);
            Painter::RunDisplay();
        }
    }
    else if (form == Form_Saw)
    {
        int dX = 30;
        for (int x = x0; x < x0 + 80; x += dX)
        {
            Painter::DrawLine(x, aveY, x + dX, minY);
            Painter::DrawLine(x + dX, aveY, x + dX, minY);
        }
    }
    else if (form == Form_Impulse)
    {
        int deltaX = 20;
        for (int i = 0; i < 5; i++)
        {
            Painter::DrawVLine(x0, minY, aveY);
            Painter::DrawHLine(minY, x0, x0 + 5);
            Painter::DrawVLine(x0 + 5, minY, aveY);
            x0 += deltaX;
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Display::DrawSignalParameters(Channel ch, int y0)
{
    int x0 = 107;
    
    WaveForm form = WAVE_FORM_CH(ch);

    y0 += 5;

    Painter::DrawText(22, y0 + 3, WaveForm_Name(form), Color::FILL);

    AllowableParameters allowParameters;
    InputWindowStruct::FillAllowParameters(ch, form, &allowParameters);
    
    for (int i = 0; i < NumParameters; i++)
    {
        if (allowParameters.allow[i])
        {
            Color color = Color::FILL;
            if (ch == CURRENT_CHANNEL)
            {
                pString curPar = Menu::NameCurrentParameter();
                pString parName = Parameter_Name((WaveParameter)i);
                if(strcmp(curPar, parName) == 0)
                {
                    Painter::FillRegion(x0, y0, 139, 8, Color::FILL);
                    color = Color::BACK;
                }
            }
            Painter::DrawText(x0 + 1, y0, Parameter_Name((WaveParameter)i), color);

            DrawParameterValue(ch, (WaveParameter)i, x0 + 80, y0);

            y0 += 10;
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Display::DrawParameterValue(Channel ch, WaveParameter param, int x, int y)
{
    WaveForm form = WAVE_FORM_CH(ch);

    x = Painter::DrawText(x, y, (&PARAMETER(ch, form, param))->StringValue());

    char buffer[10];
    Painter::DrawText(x, y, NameUnit(buffer, PARAMETER_ORDER(ch, form, param), PARAMETER_UNIT(ch, form, param)));
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Display::AddStringToConsole(char *string)
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
void Display::DrawConsole(void)
{
    if (!CONSOLE_ENABLED)
    {
        return;
    }

    int y = 1;
    for (int i = 0; i < STRING_IN_CONSOLE; i++)
    {
        int length = Font::GetLengthText(&bufferConsole[i][0]);
        Painter::FillRegion(2, y, length, 8, Color::BACK);
        Painter::DrawText(2, y, &bufferConsole[i][0], Color::FILL);
        y += 8;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Display::AddStringToIndicating(pString)
{

}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Display::DrawFrequencyCounter()
{
    if(!FREQ_COUNTER_ENABLED)
    {
        return;
    }

    Painter::DrawText(8, SCREEN_HEIGHT - 16, "F", Color::FILL);
}
