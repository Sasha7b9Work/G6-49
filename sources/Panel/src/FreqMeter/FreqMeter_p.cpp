#include "Display/Text.h"
#include "Display/Painter.h"
#include "Settings/Settings.h"
#include "Utils/StringUtils.h"
#include "Utils/Math.h"
#include "Log.h"
#include <cstdio>


static uint valueFreq = 0;
static bool inactive = true;



void PFreqMeter::Draw()
{
    if (!FREQ_METER_ENABLED)
    {
        return;
    }

    Text::SetUpperCase(false);

    if(inactive)
    {
        Color::BLUE_50.SetAsCurrent();
    }
    else
    {
        Color::FILL.SetAsCurrent();
    }

    char buffer[20];
    char text[50];

    if(FREQ_METER_MEASURE_IS_FREQ)
    {
        std::sprintf(text, (LANGUAGE ? "%skHz" : "%sк√ц"), SU::UInt2StringThisPoint(valueFreq, buffer, 8, (int)FREQ_BILLING_TIME));
    }
    else
    {
        static const struct StrOut
        {
            int forFract;
            const char * suffix;
        }
        strs[5][5] =
        {
            {{0, "мс"}, {1, "мс"}, {2, "мс"}, {0, "мкс"}, {1, "мкс"}},
            {{1, "мс"}, {2, "мс"}, {3, "мс"}, {1, "мкс"}, {2, "мкс"}},
            {{2, "мс"}, {3, "мс"}, {4, "мс"}, {2, "мкс"}, {3, "мкс"}},
            {{3, "мс"}, {4, "мс"}, {5, "мс"}, {3, "мкс"}, {4, "мкс"}},
            {{4, "мс"}, {5, "мс"}, {6, "мс"}, {4, "мкс"}, {5, "мкс"}}
        };

        StrOut str = strs[FREQ_AVE_PERIOD][FREQ_TIME_STAMPS];

        std::sprintf(text, "%s%s", SU::UInt2StringThisPoint(valueFreq, buffer, 9, str.forFract), str.suffix);
    }

    Text::DrawBigText(24, SIGNAL_HEIGHT + Page::Title::HEIGHT + 10, 3, text);

    Text::SetUpperCase(true);
}


void PFreqMeter::SetMeasure(uint value)
{
    if(value == MAX_UINT)
    {
        return;
    }
    valueFreq = value;
    inactive = false;
}


void PFreqMeter::SetInactive()
{
    inactive = true;
}


void PFreqMeter::LoadLevel()
{
    int max = 4 * 1024;

    float step = max / 200.0F;

    int value = (int)(max / 2 + FREQ_LEVEL * step);

    Math::Limitation(&value, 0, max - 1);

    PGenerator::LoadRegister(Register::FreqMeterLevel, static_cast<uint64>(value));
}


void PFreqMeter::LoadHysteresis()
{
    int max = 4 * 1024;

    float step = max / 100.0F;

    int value = (int)(step * FREQ_HYSTERESIS);

    Math::Limitation(&value, 0, max - 1);

    PGenerator::LoadRegister(Register::FreqMeterHYS, static_cast<uint64>(value));
}


void PFreqMeter::LoadSettings()
{
    LoadLevel();
    LoadHysteresis();
}
