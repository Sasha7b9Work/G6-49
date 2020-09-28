#include "Display/Painter.h"
#include "Display/Text.h"
#include "Display/Viewer.h"
#include "Display/WaveGraphics.h"
#include "Generator/Signals.h"
#include "Menu/Pages/Pages.h"
#include "Settings/Settings.h"


using namespace Primitives;


void WaveGraphics::Draw(Chan::E ch)
{
    if((set.freq.measure != FreqMeasure::Disable) && (CURRENT_CHANNEL != ch))
    {
        return;
    }

    const int x0 = X();
    const int y0 = Y(ch);
    Rectangle(Width() - 2, Height() - 2).Fill(x0 + 1, y0 + 1, Color::GREEN_5);

    Rectangle(Width(), Height()).Draw(x0, y0, Color::FILL);

    Font::StoreAndSet(TypeFont::_GOSTB20);
    String((ch == Chan::A) ? "A" : "B").Draw(x0 + 5, y0 + 5, Color::Chan(ch));
    Font::Restore();

    DrawParameters(ch, y0);

    Color::Chan(ch).SetAsCurrent();

    DrawUGO(ch, x0, y0);
}


void WaveGraphics::DrawUGO(Chan::E ch, int x, int y)
{
    if (ENABLED_CH(ch))
    {
        FORM(ch)->DrawUGO(ch, y);
    }
    else
    {
        Font::StoreAndSet(TypeFont::_GOST28);
        Font::Spacing::SetAndStore(3);

        String(LANG_IS_RU ? "ОТКЛ" : "DIS").Draw(x + 20, y + 50);

        Font::Restore();
        Font::Spacing::Restore();
    }
}


int WaveGraphics::X()
{
    return 0;
}


int WaveGraphics::Y(Chan::E ch)
{
    return (ch == Chan::A || (set.freq.measure != FreqMeasure::Disable)) ? Page::Title::HEIGHT : Page::Title::HEIGHT + SIGNAL_HEIGHT;
}


int WaveGraphics::Width()
{
    return SIGNAL_WIDTH;
}


int WaveGraphics::Height()
{
    return SIGNAL_HEIGHT;
}


void WaveGraphics::DrawParameters(Chan::E ch, int y)
{
    Form *form = FORM(ch);

    int x = 107;

    y += 5;

    Font::ForceUpperCase(true);

    String(form->Name()).Draw(22, y + 3, Color::Chan(ch));
    
    for (int i = 0; i < form->NumParameters(); i++)
    {
        GetParameterForDraw(ch, i)->viewer.Draw(y, x, x + 84, x + 125, ch);

        y += 11;
    }
}


Parameter *WaveGraphics::GetParameterForDraw(Chan::E ch, int i)
{
    Parameter *parameter = FORM(ch)->GetParameter(i);

    if (ch != CURRENT_CHANNEL && FORM(Chan::A)->IsDDS() && FORM(Chan::B)->IsDDS())        // Если установлены произвольные сигналы на обоих каналах и выводим нетекущий канал,
    {                                                                                   // то значение частоты будем брать из текущего канала (особенность работы аппаратной части прибора)
        if (parameter->IsDouble())
        {
            ParameterDouble *paramDouble = reinterpret_cast<ParameterDouble *>(parameter);

            if (paramDouble->GetType() == ParameterDoubleType::Frequency)
            {
                parameter = FORM(Chan(ch).GetInverse())->GetParameter(i);
            }
        }
    }

    return parameter;
}
