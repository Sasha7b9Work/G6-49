#include "Display/Painter.h"
#include "Display/Text.h"
#include "Display/WaveGraphics.h"
#include "Generator/Signals.h"
#include "Generator/Viewer.h"
#include "Menu/Pages/Pages.h"
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
    
        Font::StoreAndSet(TypeFont::_GOSTB20);
        String((ch == Chan::A) ? "A" : "B").Draw(x0 + 5, y0 + 5, Color::Chan(ch));
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


void WaveGraphics::DrawParameters(Chan::E ch, int y)
{
    Form *form = FORM(ch);

    int x = 107;

    y += 5;

    Font::ForceUpperCase(true);

    String(form->Name()).Draw(22, y + 3, Color::Chan(ch));
    
    for (int i = 0; i < form->NumParameters(); i++)
    {
        form->GetParameter(i)->viewer.Draw(y, x, x + 84, x + 125);

        y += 11;
    }
}
