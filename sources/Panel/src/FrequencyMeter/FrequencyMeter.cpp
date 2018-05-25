#include "FrequencyMeter.h"
#include "Display/Text.h"
#include "Settings/Settings.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FrequencyMeter::Draw()
{
    if (!FREQ_COUNTER_ENABLED)
    {
        return;
    }

    Text::DrawText(8, SCREEN_HEIGHT - 16, "F", Color::FILL);
}
