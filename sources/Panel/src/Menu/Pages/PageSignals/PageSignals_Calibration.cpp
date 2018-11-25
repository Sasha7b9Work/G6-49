#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "FrequencyMeter/FrequencyMeter.h"
#include "Generator/GeneratorPanel.h"
#include "Interface/InterfacePanel.h"
#include "Menu/Pages/Include/PageSignals.h"
#include "Settings/CalibrationSettings.h"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ”становить нужный дл€ данной калибровки параметр
static void SetParameter(Chan ch, KoeffCal::E koeff)
{
    if (koeff == KoeffCal::AD9952_ZERO)
    {
        Generator::SetOffset(ch, 0.0f);
    }
    else if (koeff == KoeffCal::AD9952_NEG)
    {
        Generator::SetOffset(ch, -5.0f);
    }
    else if (koeff == KoeffCal::AD9952_POS)
    {
        Generator::SetOffset(ch, +5.0f);
    }
    else if (koeff == KoeffCal::AD9952_AMPL)
    {
        Generator::SetAmplitude(ch, 10.0f);
    }
    else if (koeff == KoeffCal::FREQ_LEVEL_TRIG)
    {
        FrequencyMeter::LoadLevel();
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageSignals::PageCalibration::WriteKoeffCal(Chan ch, KoeffCal::E koeff)
{
    Message message;
    setCal.CreateMessage(&message, ch.value, koeff);
    
    Interface::Send(&message);

    setCal.Save();

    SetParameter(ch, koeff);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageSignals::PageCalibration::OnPress_OffsetAD9952(Chan ch, bool enter, KoeffCal::E koeff)
{
    if (enter)
    {
        Generator::SetAmplitude(ch, 0.0f);
        SetParameter(ch, koeff);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageSignals::PageCalibration::OnPress_AmplitudeAD9952(Chan ch, bool enter, KoeffCal::E koeff)
{
    if (enter)
    {
        Generator::SetOffset(ch, 0.0f);
        SetParameter(ch, koeff);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageSignals::PageCalibration::OnPress_DDS(Chan /*ch*/, bool /*enter*/, KoeffCal::E /*koeff*/)
{

}
