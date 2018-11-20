#include "stdafx.h"
#ifndef WIN32
#include "Menu/Pages/Include/PageSignals.h"
#include "Settings/Settings.h"
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
void PageSignals::PageCalibration::WriteKoeffCal(Chan ch, KoeffCal::E)
{
    static const struct StructCal
    {
        int16 *pointer;
        StructCal(int16 *p) : pointer(p) {};
    }
    values[KoeffCal::Number] =
    {
        &CAL_AD9952_OFFSET_NEG(ch),
        &CAL_AD9952_OFFSET_ZERO(ch),
        &CAL_AD9952_OFFSET_POS(ch),
        &CAL_AD9952_AMPLITUDE(ch),
        &CAL_DDS_MAX(ch),
        &CAL_DDS_MIN(ch)
    };

    //PageSignals::PageCalibration::WriteKoeffCal(ch, koeff, *values[koeff]);
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
        Generator::SetOffset(ch, values[koeff].value);
    }
}
