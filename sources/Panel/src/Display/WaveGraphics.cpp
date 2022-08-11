// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "Display/Painter.h"
#include "Display/Text.h"
#include "Signals/Viewer.h"
#include "Display/WaveGraphics.h"
#include "Signals/Signals.h"
#include "Menu/Pages/Pages.h"
#include "Settings/Settings.h"


using namespace Primitives;


namespace WaveGraphics
{
    static void DrawParameters(const Chan &, int y0);

    // Нарисовать УГО сигнала
    static void DrawUGO(const Chan &, int x, int y);
}


void WaveGraphics::Draw(const Chan &ch)
{
    if((set.freq.measure != FreqMeasure::Disable) && !ch.Is(CURRENT_CHANNEL))
    {
        return;
    }

    const int x0 = X();
    const int y0 = Y(ch);
    Rectangle(Width() - 2, Height() - 2).Fill(x0 + 1, y0 + 1, Color::GREEN_5);

    Rectangle(Width(), Height()).Draw(x0, y0, Color::FILL);

    Font::StoreAndSet(TypeFont::_GOSTB20);
    String(ch.IsA() ? "A" : "B").Draw(x0 + 5, y0 + 5, Color::Channel(ch));
    Font::Restore();

    DrawParameters(ch, y0);

    Color::Channel(ch).SetAsCurrent();

    DrawUGO(ch, x0, y0);
}


void WaveGraphics::DrawUGO(const Chan &ch, int x, int y)
{
    if (ch.Enabled())
    {
        FORM(ch)->DrawUGO(ch, y);
    }
    else
    {
        Font::StoreAndSet(TypeFont::_GOST28);
        Font::Spacing::SetAndStore(3);

        String(LANG_RU ? "ОТКЛ" : "DIS").Draw(x + 20, y + 50);

        Font::Restore();
        Font::Spacing::Restore();
    }
}


int WaveGraphics::X()
{
    return 0;
}


int WaveGraphics::Y(const Chan &ch)
{
    return (ch.IsA() || (set.freq.measure != FreqMeasure::Disable)) ? Page::Title::HEIGHT : Page::Title::HEIGHT + SIGNAL_HEIGHT;
}


int WaveGraphics::Width()
{
    return SIGNAL_WIDTH;
}


int WaveGraphics::Height()
{
    return SIGNAL_HEIGHT;
}


void WaveGraphics::DrawParameters(const Chan &ch, int y)
{
    Form *form = FORM(ch);

    int x = 107;

    y += 5;

    Font::ForceUpperCase(true);

    String(form->Name()).Draw(22, y + 3, Color::Channel(ch));
    
    for (int i = 0; i < form->NumParameters(); i++)
    {
        GetParameterForDraw(ch, i)->viewer.Draw(y, x, x + 84, x + 125, ch);

        y += 11;
    }
}


Param *WaveGraphics::GetParameterForDraw(const Chan &ch, int i)
{
    Param *parameter = FORM(ch)->GetParameter(i);

    if (FORM_A->IsDDS() && FORM_B->IsDDS())             // Если установлены произвольные сигналы на обоих каналах и выводим нетекущий канал,
    {                                                   // то значение частоты будем брать из текущего канала (особенность работы аппаратной части прибора)
        if(FORM_A->IsDDS() && FORM_B->IsDDS())
        {
            if (parameter->IsDouble())
            {
                DParam *paramDouble = reinterpret_cast<DParam *>(parameter);
            
                if (paramDouble->GetType() == TypeDParam::Frequency)
                {
                    parameter = FORM(Chan(ch).GetInverse())->GetParameter(i);
                }
            }
        }

        
    }

    return parameter;
}
