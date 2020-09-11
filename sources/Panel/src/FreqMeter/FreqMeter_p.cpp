#include "defines.h"
#include "Display/Text.h"
#include "Display/Painter.h"
#include "SCPI/SCPI.h"
#include "Settings/Settings.h"
#include "Utils/StringUtils.h"
#include "Utils/Math.h"
#include "Log.h"
#include <cstdio>


static uint valueFreq = 0;
static bool inactive = true;

bool PFreqMeter::sendToSCPI = false;



void PFreqMeter::Draw()
{
    if (set.freq.measure == FreqMeasure::Disable)
    {
        return;
    }

    Font::ForceUpperCase(false);

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

    if(set.freq.measure == FreqMeasure::Freq)
    {
        std::sprintf(text, (LANG_IS_RU ? "%sк√ц" : "%skHz"), SU::UInt2StringThisPoint(valueFreq, buffer, 8, static_cast<int>(set.freq.billingTime)));
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

        StrOut str = strs[set.freq.avePeriod][set.freq.timeStamps];

        std::sprintf(text, "%s%s", SU::UInt2StringThisPoint(valueFreq, buffer, 9, str.forFract), str.suffix);
    }

    BigText(text, 3).Draw(24, SIGNAL_HEIGHT + Page::Title::HEIGHT + 10);

    if (sendToSCPI)
    {
        SCPI::SendAnswer(text);
        sendToSCPI = false;
    }

    Font::ForceUpperCase(true);
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

    float step = static_cast<float>(max) / 200.0F;

    int value = static_cast<int>(static_cast<float>(max / 2) + static_cast<float>(set.freq.level) * step);

    Math::Limitation(&value, 0, max - 1);

    Value lValue("1.0");
    lValue.FromUnits(value, 0, 0, 0, 1);

    PGenerator::LoadRegister(Register::FreqMeterLevel, lValue.ToUINT64());
}


void PFreqMeter::LoadHysteresis()
{
    int max = 4 * 1024;

    float step = static_cast<float>(max) / 100.0F;

    int value = static_cast<int>(step * static_cast<float>(set.freq.hysteresis));

    Math::Limitation(&value, 0, max - 1);

    Value lValue("0.0");
    lValue.FromUnits(value, 0, 0, 0, 1);

    PGenerator::LoadRegister(Register::FreqMeterHYS, lValue.ToUINT64());
}


void PFreqMeter::LoadSettings()
{
    LoadLevel();
    LoadHysteresis();
}


void PFreqMeter::SendMeasureToSCPI()
{
    sendToSCPI = true;
}
