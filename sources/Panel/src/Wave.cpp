#include "globals.h"
#include "Wave.h"
#include "Display/DisplayTypes.h"
#include "Display/Painter.h"
#include "Display/Text.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Wave::DrawSignal(Channel ch)
{
    int x0 = X();
    int y0 = Y(ch);
    Painter::FillRegion(x0 + 1, y0 + 1, Width() - 2, Height() - 2, Color::GREEN_5);
    if (CHANNEL_ENABLED(ch))
    {
        Painter::DrawRectangle(x0, y0, Width(), Height(), Color::FILL);
        Text::DrawBigText(x0 + 5, y0 + 5, 2, (ch == A) ? "A" : "B", Color::FILL);
        DrawSignalUGO(ch, y0);
        DrawSignalParameters(ch, y0);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Wave::X()
{
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Wave::Y(Channel ch)
{
    return (ch == A) ? MP_TITLE_HEIGHT : MP_TITLE_HEIGHT + SIGNAL_HEIGHT;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Wave::Width()
{
    return SIGNAL_WIDTH;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Wave::Height()
{
    return SIGNAL_HEIGHT;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Wave::DrawSignalUGO(Channel chan, int y0)
{
    y0 += 30;
    int height = 50;
    int width = 90;
    int aveY = y0 + 5 + height / 2;
    int minY = y0 + 5;
    int maxY = minY + height;
    int x0 = 10;


    static bool first = true;

    static int yNoise[100];
    static int yExp[100];

    if (first)
    {
        srand(2);
        for (int i = 0; i < 100; i++)
        {
            yNoise[i] = aveY - (rand() % 50 - 25);

            yExp[i] = aveY - (int)(expf(i / 12.5f) + 0.5f) + 1;
        }

        first = false;
    }

    WaveForm form = WAVE_FORM_CH(chan).type;

    if (!form.Is(WaveForm::Free))
    {
        Painter::DrawVLine(x0, minY, maxY);
        Painter::DrawHLine(aveY, x0, x0 + width);
    }

    if (form.Is(WaveForm::Sine))
    {
        float speed = 0.1f;
        int delta = 1;

        for (int i = delta; i < width; i++)
        {
            int y1 = aveY - (int)(sinf((i - delta) * speed) * height / 2.0f);
            int y2 = aveY - (int)(sinf(i * speed) * height / 2.0f);

            Painter::DrawLine(x0 + i - delta, y1, x0 + i, y2);
        }
    }
    else if(form.Is(WaveForm::Cosine))
    {
        float speed = 0.1f;
        int delta = 1;

        for (int i = delta; i < width; i++)
        {
            int y1 = aveY - (int)(cosf((i - delta) * speed) * height / 2.0f);
            int y2 = aveY - (int)(cosf(i * speed) * height / 2.0f);

            Painter::DrawLine(x0 + i - delta, y1, x0 + i, y2);
        }
    }
    else if(form.Is(WaveForm::Meander))
    {
        int dX = 40;
        int dY = 20;
        for(int x = x0; x < x0 + 80; x += dX)
        {
            Painter::DrawHLine(aveY - dY, x, x + dX / 2);
            Painter::DrawVLine(x + dX / 2, aveY - dY, aveY + dY);
            Painter::DrawHLine(aveY + dY, x + dX / 2, x + dX);
            Painter::DrawVLine(x + dX, aveY - dY, aveY + dY);
        }
    }
    else if (form.Is(WaveForm::RampPlus))
    {
        int dX = 28;
        for (int x = x0; x < x0 + 80; x += dX)
        {
            Painter::DrawLine(x, aveY, x + dX, minY);
            Painter::DrawLine(x + dX, aveY, x + dX, minY);
        }
    }
    else if(form.Is(WaveForm::RampMinus))
    {
        int dX = 28;
        int dY = 20;
        for (int x = x0; x < x0 + 80; x += dX)
        {
            Painter::DrawLine(x, aveY - dY, x + dX, aveY);
            Painter::DrawVLine(x + dX, aveY - dY, aveY);
        }
    }
    else if(form.Is(WaveForm::Triangle))
    {
        int dX = 38;
        int dY = 20;
        for(int x = x0; x < x0 + 80; x += dX)
        {
            int xAve = x + dX / 2;
            if (xAve < x0 + 80)
            {
                Painter::DrawLine(x, aveY, xAve, aveY - dY);
                Painter::DrawLine(xAve, aveY - dY, x + dX, aveY);
            }
        }
    }
    else if(form.Is(WaveForm::Trapeze))
    {
        int dX = 20;
        int dY = 20;
        for (int i = 0; i < 2; i++)
        {
            Painter::DrawLine(x0, aveY, x0 + dX / 2, aveY - dY);
            Painter::DrawHLine(aveY - dY, x0 + dX / 2, x0 + 3 * dX / 2);
            Painter::DrawLine(x0 + 3 * dX / 2, aveY - dY, x0 + 2 * dX, aveY);
            x0 += 2 * dX;
        }
    }
    else if (form.Is(WaveForm::Impulse))
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
    else if (form.Is(WaveForm::ExpPlus))
    {
        for(int i = 0; i < 2; i++)
        {
            for (int x = 1 + (i * 40); x < 40 + (i * 40); x++)
            {
                Painter::SetPoint(x0 + x, yExp[x - (i * 40)]);
                Painter::DrawLine(x0 + x - 1, yExp[x - 1 - i * 40], x0 + x, yExp[x - i * 40]);
            }
            Painter::DrawVLine(x0 + 40 * (i + 1), yExp[40], aveY);
        }
    }
    else if (form.Is(WaveForm::ExpMinus))
    {
        for (int i = 0; i < 2; i++)
        {
            for (int x = 1 + (i * 40); x < 40 + (i * 40); x++)
            {
                Painter::SetPoint(x0 + x, yExp[40 - (x - (i * 40))]);
                Painter::DrawLine(x0 + x - 1, yExp[40 - (x - 1 - i * 40)], x0 + x, yExp[40 - (x - i * 40)]);
            }
            Painter::DrawVLine(x0 + 40 * (i + 1), yExp[40], aveY);
        }
    }
    else if(form.Is(WaveForm::Noise))
    {
        int i = 0;
        int dX = 1;
        for(int x = x0; x <= x0 + 85; x += dX)
        {
            Painter::DrawLine(x, yNoise[i], x + dX, yNoise[i + 1]);
            i++;
        }
    }
    else if(form.Is(WaveForm::Free))
    {
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Wave::DrawSignalParameters(Channel ch, int y0)
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
                pString parName = Parameter_Name((Type_WaveParameter)i);
                if (strcmp(curPar, parName) == 0 && CURRENT_PAGE == 0)
                {
                    Painter::FillRegion(x0, y0, 139, 8, Color::GRAY_25);
                }
            }
            Text::DrawText(x0 + 1, y0, Parameter_Name((Type_WaveParameter)i), color);

            DrawParameterValue(ch, (Type_WaveParameter)i, x0 + 80, y0);

            y0 += 10;
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Wave::DrawParameterValue(Channel ch, Type_WaveParameter param, int x, int y)
{
    WaveForm form = WAVE_FORM_CH(ch);

    x = Text::DrawText(x, y, (&PARAMETER(ch, form, param))->StringValue());

    char buffer[10];
    Text::DrawText(x, y, NameUnit(buffer, PARAMETER_ORDER(ch, form, param), PARAMETER_UNIT(ch, form, param)));
}
