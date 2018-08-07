#include "FrequencyMeter.h"
#include "Display/Text.h"
#include "Display/Painter.h"
#include "Settings/Settings.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint FrequencyMeter::frequency = 0;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FrequencyMeter::Draw()
{
    if (!FREQ_ENABLED)
    {
        return;
    }

    Painter::SetColor(Color::FILL);
    Text::DrawFormatText(8, SCREEN_HEIGHT - 16, "F = %d", frequency);

    if(FREQ_MEASURE_IS_FREQ)
    {

    }
    else
    {
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FrequencyMeter::SetMeasure(uint value)
{
    frequency = value;
}
