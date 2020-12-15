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


static String FormValue(uint8 lang);


void PFreqMeter::Draw() //-V2506
{
    if (set.freq.measure == FreqMeasure::Disable)
    {
        return;
    }

    Font::ForceUpperCase(false);

    String text = FormValue(LANGUAGE);

    BigText(text.c_str(), 3).Draw(24, SIGNAL_HEIGHT + Page::Title::HEIGHT + 10, inactive ? Color::GRAY_10 : Color::FILL);

    Font::ForceUpperCase(true);

    if (sendToSCPI)
    {
        SCPI::SendAnswer(FormValue(1).c_str());

        sendToSCPI = false;
    }
}


static String FormValue(uint8 lang)
{
    char buffer[20];
    char text[50];

    if(set.freq.measure == FreqMeasure::Freq)
    {
        std::sprintf(text, ((lang == 0) ? "%sк√ц" : "%skHz"), SU::UInt2StringThisPoint(valueFreq, buffer, 8, static_cast<int>(set.freq.billingTime)));
    }
    else
    {
        static const struct StrOut
        {
            int forFract;
            const char * suffix[2];
        }
        strs[5][5] =
        {
            {{0, {"мс", "ms"}}, {1, {"мс", "ms"}}, {2, {"мс", "ms"}}, {0, {"мкс", "us"}}, {1, {"мкс", "us"}}},
            {{1, {"мс", "ms"}}, {2, {"мс", "ms"}}, {3, {"мс", "ms"}}, {1, {"мкс", "us"}}, {2, {"мкс", "us"}}},
            {{2, {"мс", "ms"}}, {3, {"мс", "ms"}}, {4, {"мс", "ms"}}, {2, {"мкс", "us"}}, {3, {"мкс", "us"}}},
            {{3, {"мс", "ms"}}, {4, {"мс", "ms"}}, {5, {"мс", "ms"}}, {3, {"мкс", "us"}}, {4, {"мкс", "us"}}},
            {{4, {"мс", "ms"}}, {5, {"мс", "ms"}}, {6, {"мс", "ms"}}, {4, {"мкс", "us"}}, {5, {"мкс", "us"}}}
        };

        StrOut str = strs[set.freq.avePeriod][set.freq.timeStamps];

        std::sprintf(text, "%s%s", SU::UInt2StringThisPoint(valueFreq, buffer, 9, str.forFract), str.suffix[lang]);
    }

    return String(text);
}


void PFreqMeter::SetMeasure(uint value) //-V2506
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

    Value lValue("1.0", Order::One);
    lValue.FromUnits(value, 0, 0, 0, 1);

    PGenerator::LoadRegister(Register::FreqMeterLevel, lValue.ToUINT64());
}


void PFreqMeter::LoadHysteresis()
{
    int max = 4 * 1024;

    float step = static_cast<float>(max) / 100.0F;

    int value = static_cast<int>(step * static_cast<float>(set.freq.hysteresis));

    Math::Limitation(&value, 0, max - 1);

    Value lValue("0.0", Order::One);
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
