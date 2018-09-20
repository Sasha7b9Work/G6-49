#include "Signals.h"
#include "Wave.h"
#include "Display/DisplayTypes.h"
#include "Display/Painter.h"
#include "Display/Text.h"
#include "Menu/Menu.h"
#include "Menu/Pages/PageSignals.h"
#include "Settings/Settings.h"
#include "Utils/Debug.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Wave::Graphics::Draw(Chan ch)
{
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
    return ch == Chan::A ? MP_TITLE_HEIGHT : MP_TITLE_HEIGHT + SIGNAL_HEIGHT;
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
        srand(2);
        for (int i = 0; i < 100; i++)
        {
            yNoise[i] = aveY - (rand() % 50 - 25);

            yExp[i] = aveY - (int)(expf(i / 12.5f) + 0.5f) + 1;
        }

        first = false;
    }

    Painter::DrawVLine(x0, minY, maxY);
    Painter::DrawHLine(aveY, x0, x0 + width);

    static const struct StructFunc
    {
        typedef void (*pFuncIIII)(int, int, int, int);
        pFuncIIII func;
        StructFunc(pFuncIIII f) : func(f) {};
    } func[Form::Number] =
    {
        DrawSine,
        DrawRampPlus,
        DrawRampMinus,
        DrawMeander,
        DrawImpulse,
        DrawPacketImpulse
    };

    int index = FORM(chan)->value;
    
    func[index].func(x0, minY, width, height);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Wave::Graphics::DrawSine(int x0, int y0, int width, int height)
{
    float speed = 0.1f;
    int delta = 1;
    y0 += height / 2;

    for (int i = delta; i < width; i++)
    {
        int y1 = y0 - (int)(sinf((i - delta) * speed) * height / 2.0f);
        int y2 = y0 - (int)(sinf(i * speed) * height / 2.0f);

        Painter::DrawLine(x0 + i - delta, y1, x0 + i, y2);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Wave::Graphics::DrawRampPlus(int x0, int y0, int, int height)
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
void Wave::Graphics::DrawRampMinus(int x0, int y0, int, int height)
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
void Wave::Graphics::DrawMeander(int x0, int y0, int, int height)
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
void Wave::Graphics::DrawImpulse(int x0, int y0, int, int height)
{
    int minY = y0;
    int aveY = y0 + height / 2;
    int deltaX = 20;
    for (int i = 0; i < 5; i++)
    {
        Painter::DrawVLine(x0, minY, aveY);
        Painter::DrawHLine(minY, x0, x0 + 5);
        Painter::DrawVLine(x0 + 5, minY, aveY);
        x0 += deltaX;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Wave::Graphics::DrawPacketImpulse(int x0, int y0, int, int height)
{
    int minY = y0;
    int aveY = y0 + height / 2;
    int deltaX = 8;
    for(int j = 0; j < 2; j++)
    {
        for (int i = 0; i < 3; i++)
        {
            Painter::DrawVLine(x0, minY, aveY);
            Painter::DrawHLine(minY, x0, x0 + 2);
            Painter::DrawVLine(x0 + 2, minY, aveY);
            x0 += deltaX;
        }

        x0 += 30;
    }
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
        Parameter *param = form->GetParameter(i);
        if ((ch == CURRENT_CHANNEL) && strcmp(PARAM_CURRENT->Name(), param->Name()) == 0 && CURRENT_PAGE == PageSignals::pointer)
        {
            Painter::FillRegion(x0, y0, 139, 9, Color::GRAY_25);
        }
        Text::DrawText(x0 + 1, y0, param->Name(), Color::FILL);
        DrawParameterValue(param, x0 + 80, y0);
        y0 += 11;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Wave::Graphics::DrawParameterValue(Parameter *param, int x, int y)
{
    x = Text::DrawText(x, y, param->GetStringValue());
    char buffer[10];
    Text::SetUpperCase(false);
    Text::DrawText(x + 2, y, param->NameUnit(buffer));
    Text::SetUpperCase(true);
}
