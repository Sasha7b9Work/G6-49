#include "stdafx.h"
#ifndef WIN32
#include "Signals.h"
#include "Wave.h"
#include "Display/DisplayTypes.h"
#include "Display/Painter.h"
#include "Display/Text.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Include/PageSignals.h"
#include "Settings/Settings.h"
#include "Utils/Debug.h"
#include <cmath>
#include <string.h>
#include <cstdlib>
#endif


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Wave::Graphics::Draw(Chan ch)
{
    if(FREQ_ENABLED && ch != CURRENT_CHANNEL)
    {
        return;
    }

    int x0 = X();
    int y0 = Y(ch);
    Painter::FillRegion(x0 + 1, y0 + 1, Width() - 2, Height() - 2, Color::GREEN_5);
    if (CHANNEL_ENABLED(ch))
    {
        Painter::DrawRectangle(x0, y0, Width(), Height(), Color::FILL);
        Text::DrawBigText(x0 + 5, y0 + 5, 2, ch == Chan::A ? "A" : "B", Color::FILL);
        DrawUGO(ch, y0);
        DrawParameters(ch, y0);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Wave::Graphics::X()
{
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Wave::Graphics::Y(Chan ch)
{
    return (ch == Chan::A || FREQ_ENABLED) ? Page::Title::HEIGHT : Page::Title::HEIGHT + SIGNAL_HEIGHT;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Wave::Graphics::Width()
{
    return SIGNAL_WIDTH;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Wave::Graphics::Height()
{
    return SIGNAL_HEIGHT;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Wave::Graphics::DrawUGO(Chan chan, int y0)
{
    y0 += 30;
    int height = 50;
    int width = 90;
    int aveY = y0 + 5 + height / 2;
    int minY = y0 + 5;
    int maxY = minY + height;
    int x0 = 10;


    static bool first = true;

    static volatile int yNoise[100];
    static volatile int yExp[100];

    if (first)
    {
        std::srand(2);
        for (int i = 0; i < 100; i++)
        {
            yNoise[i] = aveY - (std::rand() % 50 - 25);

            yExp[i] = aveY - (int)(std::expf(i / 12.5f) + 0.5f) + 1;
        }

        first = false;
    }

    Painter::DrawVLine(x0, minY - 3, maxY + 3);
    Painter::DrawHLine(aveY, x0, x0 + width);

    static const struct StructFunc
    {
        typedef void (*pFuncIIII)(Chan, int, int, int, int);
        pFuncIIII func;
        StructFunc(pFuncIIII f) : func(f) {};
    } func[Form::Number] =
    {
        DrawSine,
        DrawRampPlus,
        DrawRampMinus,
        DrawTriangle,
        DrawMeander,
        DrawImpulse,
        DrawPacketImpulse,
        DrawDDS
    };

    int index = FORM(chan)->value;
    
    func[index].func(chan, x0, minY, width, height);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Wave::Graphics::DrawSine(Chan, int x0, int y0, int width, int height)
{
    float speed = 0.2f;
    int delta = 1;
    y0 += height / 2;

    for (int i = delta; i < width; i++)
    {
        int y1 = y0 - (int)(std::sinf((i - delta) * speed) * height / 2.0f);
        int y2 = y0 - (int)(std::sinf(i * speed) * height / 2.0f);

        Painter::DrawLine(x0 + i - delta, y1, x0 + i, y2);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Wave::Graphics::DrawRampPlus(Chan, int x0, int y0, int, int height)
{
    y0 += height;
    int dX = 28;
    for (int x = x0; x < x0 + 80; x += dX)
    {
        Painter::DrawLine(x, y0, x + dX, y0 - height);
        Painter::DrawLine(x + dX, y0, x + dX, y0 - height);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Wave::Graphics::DrawRampMinus(Chan, int x0, int y0, int, int height)
{
    int aveY = y0 + height / 2;
    int dX = 28;
    int dY = 20;
    for (int x = x0; x < x0 + 80; x += dX)
    {
        Painter::DrawLine(x, aveY - dY, x + dX, aveY + dY);
        Painter::DrawVLine(x + dX, aveY - dY, aveY + dY);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Wave::Graphics::DrawTriangle(Chan, int x, int y, int width, int height)
{
    int min = y + height;
    Painter::DrawLine(x, min, x + width / 2, y);
    Painter::DrawLine(x + width / 2, y, x + width, y + height);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Wave::Graphics::DrawMeander(Chan, int x0, int y0, int, int height)
{
    int dX = 40;
    int dY = 20;
    int aveY = y0 + height / 2;
    for (int x = x0; x < x0 + 80; x += dX)
    {
        Painter::DrawHLine(aveY - dY, x, x + dX / 2);
        Painter::DrawVLine(x + dX / 2, aveY - dY, aveY + dY);
        Painter::DrawHLine(aveY + dY, x + dX / 2, x + dX);
        Painter::DrawVLine(x + dX, aveY - dY, aveY + dY);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Wave::Graphics::DrawImpulse(Chan ch, int x0, int y0, int, int height)
{
    int minY = y0;
    int maxY = y0 + height;

    ParameterChoice *param = (ParameterChoice *)WAVE(ch).GetCurrentForm()->FindParameter(ParameterChoice::Polarity);

    if(param->GetChoice() == 1)
    {
        minY = maxY;
        maxY = y0;
    }

    int deltaX = 20;

    for (int i = 0; i < 5; i++)
    {
        Painter::DrawVLine(x0, minY, maxY);
        Painter::DrawVLine(x0 + 5, minY, maxY);
        Painter::DrawHLine(minY, x0, x0 + 5);
        Painter::DrawHLine(maxY, x0 + 5, x0 + (i == 4 ? 7 : deltaX));
        x0 += deltaX;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Wave::Graphics::DrawPacketImpulse(Chan, int x0, int y0, int, int height)
{
    int minY = y0;
    int maxY = y0 + height;
    int deltaX = 8;
    for(int j = 0; j < 2; j++)
    {
        for (int i = 0; i < 3; i++)
        {
            Painter::DrawVLine(x0, minY, maxY);
            Painter::DrawHLine(minY, x0, x0 + 4);
            Painter::DrawVLine(x0 + 4, minY, maxY);
            Painter::DrawHLine(maxY, x0 + 4, x0 + deltaX);
            x0 += deltaX;
        }

        if(j == 0)
        {
            Painter::DrawHLine(maxY, x0, x0 + 35);
        }

        x0 += 37;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Wave::Graphics::DrawDDS(Chan, int, int, int, int)
{
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Wave::Graphics::DrawParameters(Chan ch, int y0)
{
    Form *form = FORM(ch);

    int x0 = 107;

    y0 += 5;

    Text::DrawText(22, y0 + 3, form->Name(LANG), Color::FILL);
    
    for (int i = 0; i < form->NumParameters(); i++)
    {
        ParameterBase *param = form->GetParameter(i);
        
        if ((ch == CURRENT_CHANNEL) && strcmp(PARAM_CURRENT->Name(), param->Name()) == 0 && CURRENT_PAGE == PageSignals::pointer)
        {
            Painter::FillRegion(x0, y0, 139, 9, Color::GRAY_25);
        }
        
        Text::DrawText(x0 + 1, y0, param->Name(), Color::FILL);
        
        if(i == 3 && ch.IsA())
        {
            i = i;
        }
        
        DrawParameterValue(param, x0 + 80, y0);
        y0 += 11;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Wave::Graphics::DrawParameterValue(ParameterBase *param, int x, int y)
{
    if(param->IsChoice() && ((ParameterChoice *)param)->DrawChoice(x + 6, y))
    {
        return;
    }

    if(param->IsChoice())
    {
        param = param;
    }
    x = Text::DrawText(x, y, param->GetStringValue());
    char buffer[10];
    Text::SetUpperCase(false);
    Text::DrawText(x + 2, y, param->NameUnit(buffer));
    Text::SetUpperCase(true);
}
