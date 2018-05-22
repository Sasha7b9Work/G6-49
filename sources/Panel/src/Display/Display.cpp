#include "defines.h"
#include "globals.h"
#include "Display.h"
#include "DisplayTypes.h"
#include "Display/Painter.h"
#include "Display/Text.h"
#include "Display/Font/Font.h"
#include "Hardware/CPU.h"
#include "Hardware/LTDC.h"
#include "Hardware/Timer.h"
#include "InputWindow.h"
#include "InputWindowStruct.h"
#include "Menu/Menu.h"
#include "Menu/MenuItems.h"
#include "Settings/Settings.h"
#include "Settings/SettingsDisplay.h"
#include "Settings/SettingsSignals.h"
#include "Settings/SettingsTypes.h"
#include "Utils/StringUtils.h"
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
char        Display::bufferConsole[STRING_IN_CONSOLE][SYMBOLS_IN_STRING] = {};
#ifdef STM32F429xx
uint8       *Display::frontBuffer = (uint8 *)malloc(BUFFER_WIDTH * BUFFER_HEIGHT);
uint8       *Display::backBuffer = (uint8 *)malloc(BUFFER_WIDTH * BUFFER_HEIGHT);
#endif
    
#ifdef STM32F746xx
uint8       *Display::frontBuffer = (uint8 *)SDRAM_DEVICE_ADDR;
uint8       *Display::backBuffer = (uint8 *)(SDRAM_DEVICE_ADDR + BUFFER_HEIGHT * BUFFER_WIDTH);
#endif

int Display::timeFrame = 0;
int Display::timeAllFrames = 0;
uint Display::timeStartFrames = 0;
uint Display::timeAccumFrames = 0;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Display::Init()
{
    LTDC_::Init((uint)frontBuffer, (uint)backBuffer);
    
    Text::SetUpperCase(true);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
uint8 *Display::GetBuffer()
{
    return backBuffer;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Display::Update()
{
    uint timeStart = TIME_MS;

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

    DrawFrequencyCounter();

    DrawHint();

    ShowStatistics();

    DrawConsole();

    CPU::Keyboard::Draw();

    Painter::EndScene();

    timeFrame = (int)(TIME_MS - timeStart);

    timeAccumFrames += timeFrame;

    if(TIME_MS >= timeStartFrames + 1000)
    {
        timeAllFrames = (int)timeAccumFrames;
        timeStartFrames = TIME_MS;
        timeAccumFrames = 0;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Display::DrawSignal(Channel ch)
{
    int y0 = (ch == A) ? MP_TITLE_HEIGHT : MP_TITLE_HEIGHT + SIGNAL_HEIGHT;
    Painter::FillRegion(0 + 1, y0 + 1, SIGNAL_WIDTH - 2, SIGNAL_HEIGHT - 2, Color::GREEN_5);
    if (CHANNEL_ENABLED(ch))
    {
        Painter::DrawRectangle(0, y0, SIGNAL_WIDTH, SIGNAL_HEIGHT, Color::FILL);
        Text::DrawBigText(5, y0 + 5, 2, (ch == A) ? "1" : "2", Color::FILL);
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

    if (form.type == Sine)
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
    else if (form.type == RampPlus)
    {
        int dX = 30;
        for (int x = x0; x < x0 + 80; x += dX)
        {
            Painter::DrawLine(x, aveY, x + dX, minY);
            Painter::DrawLine(x + dX, aveY, x + dX, minY);
        }
    }
    else if (form.type == Impulse)
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

    Text::DrawText(22, y0 + 3, form.Name(), Color::FILL);

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
                if(strcmp(curPar, parName) == 0 && CURRENT_PAGE == 0)
                {
                    Painter::FillRegion(x0, y0, 139, 8, Color::GRAY_25);
                }
            }
            Text::DrawText(x0 + 1, y0, Parameter_Name((WaveParameter)i), color);

            DrawParameterValue(ch, (WaveParameter)i, x0 + 80, y0);

            y0 += 10;
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Display::DrawParameterValue(Channel ch, WaveParameter param, int x, int y)
{
    WaveForm form = WAVE_FORM_CH(ch);

    x = Text::DrawText(x, y, (&PARAMETER(ch, form, param))->StringValue());

    char buffer[10];
    Text::DrawText(x, y, NameUnit(buffer, PARAMETER_ORDER(ch, form, param), PARAMETER_UNIT(ch, form, param)));
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
        Text::DrawText(2, y, &bufferConsole[i][0], Color::FILL);
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

    Text::DrawText(8, SCREEN_HEIGHT - 16, "F", Color::FILL);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Display::ShowStatistics()
{
    if(SHOW_STATISTICS)
    {
        Painter::DrawFilledRectangle(0, SCREEN_HEIGHT - 12, 60, 10, Color::BACK, Color::FILL);
        char buffer[100] = {0};
        strcpy(buffer, Int2String((int)(CPU::GetFPS() + 0.5f), false, 3));
        strcat(buffer, "/");
        strcat(buffer, Int2String(timeFrame, false, 3));
        strcat(buffer, "/");
        strcat(buffer, Int2String(timeAllFrames, false, 3));
        Text::DrawText(2, SCREEN_HEIGHT - 11, buffer, Color::FILL);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Display::DrawHint()
{
    if(IN_MODE_SHOW_HINTS)
    {
        int x0 = 0;
        int y0 = MP_TITLE_HEIGHT;
        int width = 319 - MI_WIDTH;

        y0 = Text::DrawTextInBoundedRectWithTransfers(x0, y0, width,
        LANG_RU ?
        "Включён режим подсказок.\n"
        "Для получения информации по элементу меню или назначению кнопки используйте соответствующий орган управления.\n"
        "Для выхода из режима подсказок нажмите и удерживайте кнопку ESC."
        :
        "Prompt mode is enabled.\n"
        "For information about the menu item or the purpose of the button, use the appropriate control.\n"
        "To exit the prompt mode, press and hold the ESC key."
        ,
        Color::BACK, Color::FILL, Color::GRAY_50);

        Painter::DrawFilledRectangle(x0, y0, width, 239 - y0 - 1, Color::BACK, Color::FILL);

        if(Menu::ItemHint())
        {
            Control *control = Menu::ItemHint();
            Painter::SetColor(Color::GREEN_50);

            Text::DrawFormatStringInCenterRect(x0, y0 + 4, width, 10, "*** %s ***", control->Title());
            Painter::SetColor(Color::GREEN);

            y0 = control->DrawHint(x0 + 5, y0 + 17, width - 8) + 5;

            Painter::SetColor(Color::WHITE);

            char number = '1';

            if(control->Type() == Control_Choice)
            {
                Choice *choice = (Choice *)control;

                for(int i = 0; i < choice->NumSubItems(); i++)
                {
                    y0 = Text::DrawFormatTextInColumnWithTransfersDiffColors(x0 + 2, y0, width - 10, Color::GREEN, "%c. \"%s\" %s", number++, 
                                                                choice->NameSubItem(i), LANG_RU ? choice->hintsRu[i] : choice->hintsEn[i]) + 5;
                }
            }
        }
        else if(Menu::PanelControlHint())
        {

        }
    }
}
