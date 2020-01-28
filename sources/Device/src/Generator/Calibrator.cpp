#include "defines.h"
#include "Generator/Calibrator.h"
#include "Generator/Generator_d.h"
#include "Settings/CalibrationSettings.h"


void Calibrator::SetFormWave(Chan::E ch, uint8 signal)
{
    if(signal == 0)
    {
        DGenerator::SetFormWave(ch, TypeForm::Sine);
    }
    else
    {
        DGenerator::SetFormWave(ch, TypeForm::Meander);
    }
}


void Calibrator::SetAmplitude(Chan::E ch, uint8 range)
{
    static const float amplitude[6] =
    {
        100e-3F,
        300e-3F,
        1.0F,
        3.0F,
        10.0F
    };

    DGenerator::SetAmplitude(ch, FloatValue(amplitude[range]));
}


void Calibrator::SetOffset(Chan::E ch, uint8 param)
{
    static const float offset[4] =
    {
        0.0F,
        5.0F,
        0.0F,
        -5.0F
    };

    DGenerator::SetOffset(ch, FloatValue(offset[param]));
}


void Calibrator::SetK(uint8 channel, uint8 signal, uint8 range, uint8 param, int16 k)
{
    *setCal.GetK(channel, signal, range, param) = k;

    Chan::E ch = static_cast<Chan::E>(channel);

    SetFormWave(ch, signal);

    SetOffset(ch, param);

    if(param == 0)
    {
        SetAmplitude(ch, range);
    }
    else
    {
        DGenerator::SetAmplitude(ch, FloatValue(0.0F));
    }
}


uint8 Calibrator::CalculateRange(Chan::E ch)
{
    float amplitude = DGenerator::GetAmplitude(ch);

    if(amplitude > 3.16F)
    {
        return 4;
    }
    else if(amplitude > 1.0F)
    {
        return 3;
    }
    else if(amplitude > 0.316F)
    {
        return 2;
    }
    else if(amplitude > 0.1F)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


float Calibrator::GetAmplitudeK(Chan::E ch)
{
    uint8 range = CalculateRange(ch);

    int16 k = *setCal.GetK(static_cast<uint8>(ch), DGenerator::FormIsSine(ch) ? 0U : 1U, range, 0U);

    return 1.0F + k / 1000.0F;
}


float Calibrator::GetOffsetK_Zero(Chan::E ch)
{
    uint8 range = CalculateRange(ch);

    bool isSine = DGenerator::FormIsSine(ch);

    if(isSine)
    {
        return 2048.0F + *setCal.GetK(static_cast<uint8>(ch), 0U, range,  2U);
    }
    else
    {
        return 0.0F;
    }
}


float Calibrator::GetOffsetK_Negative(Chan::E)
{
    return 4095.0F;
}


float Calibrator::GetOffsetK_Positive(Chan::E)
{
    return 0.0F;
}


float Calibrator::GetFreqMeterK_Trig()
{
    return 0.0F;
}
