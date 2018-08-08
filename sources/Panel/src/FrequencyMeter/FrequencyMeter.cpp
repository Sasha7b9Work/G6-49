#include "FrequencyMeter.h"
#include "Display/Text.h"
#include "Display/Painter.h"
#include "Settings/Settings.h"
#include "Utils/StringUtils.h"


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

    char buffer[20];

    if(FREQ_MEASURE_IS_FREQ)
    {
        Text::DrawFormatText(8, SCREEN_HEIGHT - 16, "F = %s к√ц", UInt2StringThisPoint(frequency, buffer, 6, (int)FREQ_BILLING_TIME));
    }
    else
    {
        static const struct StrOut
        {
            int forFract;
            const char * const suffix;
        } strs[5][5] =
        {
            {{0, "мс"}, {1, "мс"}, {2, "мс"}, {0, "мкс"}, {1, "мкс"}},
            {{1, "мс"}, {2, "мс"}, {3, "мс"}, {1, "мкс"}, {2, "мкс"}},
            {{2, "мс"}, {3, "мс"}, {4, "мс"}, {2, "мкс"}, {3, "мкс"}},
            {{3, "мс"}, {4, "мс"}, {5, "мс"}, {3, "мкс"}, {4, "мкс"}},
            {{4, "мс"}, {5, "мс"}, {6, "мс"}, {4, "мкс"}, {5, "мкс"}}
        };

        StrOut str = strs[FREQ_AVE_PERIOD][FREQ_TIME_STAMPS];

        Text::DrawFormatText(8, SCREEN_HEIGHT - 16, "T = %s %s", UInt2StringThisPoint(frequency, buffer, 9, str.forFract), str.suffix);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FrequencyMeter::SetMeasure(uint value)
{
    frequency = value;
}
