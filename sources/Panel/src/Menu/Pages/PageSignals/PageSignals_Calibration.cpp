#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Generator/GeneratorPanel.h"
#include "Interface/InterfacePanel.h"
#include "Menu/Pages/Include/PageSignals.h"
#include "Settings/CalibrationSettings.h"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PageSignals::PageCalibration::PrepareForOffsetAD9952(Chan /*ch*/)
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageSignals::PageCalibration::PrepareForAmplitudeAD9952(Chan /*ch*/)
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageSignals::PageCalibration::WriteKoeffCal(Chan ch, KoeffCal::E koeff)
{
    Message message;
    setCal.CreateMessage(&message, ch.value, koeff);
    
    Interface::Send(&message);

    setCal.Save();

    if (koeff == KoeffCal::AD9952_ZERO)
    {
        Generator::SetOffset(ch, 0.0f);
    }
    else if (koeff == KoeffCal::AD9952_POS)
    {
        Generator::SetOffset(ch, +5.0f);
    }
    else if (koeff == KoeffCal::AD9952_NEG)
    {
        Generator::SetOffset(ch, -5.0f);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageSignals::PageCalibration::OnPress_OffsetAD9952(Chan ch, bool enter, KoeffCal::E koeff)
{
    static const struct StructValue
    {
        float value;
        StructValue(float v) : value(v) {};
    }
    values[KoeffCal::Number] =
    {
        -5.0f,
        0.0f,
        5.0f,
        0.0f,
        0.0f,
        0.0f
    };

    if (enter)
    {
        PrepareForOffsetAD9952(ch);
        Generator::SetAmplitude(ch, 0.0f);
        Generator::SetOffset(ch, values[koeff].value);
    }

    if (koeff == KoeffCal::AD9952_ZERO)
    {
        Generator::SetOffset(ch, 0.0f);
    }
    else if (koeff == KoeffCal::AD9952_POS)
    {
        Generator::SetOffset(ch, +5.0f);
    }
    else if (koeff == KoeffCal::AD9952_NEG)
    {
        Generator::SetOffset(ch, -5.0f);
    }
}
