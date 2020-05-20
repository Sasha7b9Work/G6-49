#include "Display/Painter.h"
#include "Display/Text.h"
#include "Display/WaveGraphics.h"
#include "Generator/Signals.h"
#include "Generator/Viewer.h"
#include "Menu/Pages/PageSignals/PageSignals.h"
#include "Settings/Settings.h"


using namespace Primitives;


void WaveGraphics::Draw(Chan::E ch)
{
    if((set.freq.measure != FreqMeasure::Disable) && (CURRENT_CHANNEL != ch))
    {
        return;
    }

    int x0 = X();
    int y0 = Y(ch);
    Rectangle(Width() - 2, Height() - 2).Fill(x0 + 1, y0 + 1, Color::GREEN_5);

    if (set.enabled[ch])
    {
        Rectangle(Width(), Height()).Draw(x0, y0, Color::FILL);

        Font::Store();
        Font::Set(TypeFont::_GOSTB20);
        Text::Draw(x0 + 5, y0 + 5, (ch == Chan::A) ? "A" : "B", Color::Chan(ch));
        Font::Restore();

        FORM(ch)->DrawUGO(ch, y0);
        DrawParameters(ch, y0);
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


void WaveGraphics::DrawParameters(Chan::E ch, int y0)
{
    Form *form = FORM(ch);

    int x0 = 107;

    y0 += 5;

    String(form->Name(LANGUAGE)).Draw(22, y0 + 3, Color::Chan(ch));
    
    for (int i = 0; i < form->NumParameters(); i++)
    {
        form->GetParameter(i)->viewer.Draw(x0, y0);

        y0 += 11;
    }
}
