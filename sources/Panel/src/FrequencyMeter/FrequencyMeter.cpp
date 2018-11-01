#include "stdafx.h"
#ifndef WIN32
#include "Display/Text.h"
#include "Display/Painter.h"
#include "Settings/Settings.h"
#include "Utils/StringUtils.h"
#include "Utils/Math.h"
#include "Log.h"
#endif


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint FrequencyMeter::frequency = 0;
bool FrequencyMeter::inactive = true;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FrequencyMeter::Draw()
{
    if (!FREQ_ENABLED)
    {
        return;
    }

    Text::SetUpperCase(false);

    Painter::SetColor(inactive ? Color::BLUE_50 : Color::FILL);

    char buffer[20];

    if(FREQ_MEASURE_IS_FREQ)
    {
        char text[50];
        sprintf(text, "%sк√ц", UInt2StringThisPoint(frequency, buffer, 8, (int)FREQ_BILLING_TIME));
        Text::DrawBigText(24, SIGNAL_HEIGHT + Page::Title::HEIGHT + 10, 3, text);
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

        char text[50];
        sprintf(text, "%s %s", UInt2StringThisPoint(frequency, buffer, 9, str.forFract), str.suffix);
    }

    Text::SetUpperCase(true);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FrequencyMeter::SetMeasure(uint value)
{
    if(value == MAX_UINT)
    {
        return;
    }
    frequency = value;
    inactive = false;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FrequencyMeter::SetInactive()
{
    inactive = true;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FrequencyMeter::LoadLevel()
{
    int max = 4 * 1024;

    float step = max / 200.0f;

    int value = (int)(max / 2 + FREQ_LEVEL * step);

    Limitation(&value, 0, max - 1);

    Generator::LoadRegister(Register::FreqMeterLevel, (uint64)value);

    LOG_WRITE("%d", value);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FrequencyMeter::LoadHysteresis()
{
    int max = 4 * 1024;

    float step = max / 100.0f;

    int value = (int)(step * FREQ_HYSTERESIS);

    Limitation(&value, 0, max - 1);

    Generator::LoadRegister(Register::FreqMeterHYS, (uint64)value);

    LOG_WRITE("%d", value);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FrequencyMeter::LoadSettings()
{
    LoadLevel();
    LoadHysteresis();
}
