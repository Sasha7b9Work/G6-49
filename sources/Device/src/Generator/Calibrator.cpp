#include "defines.h"
#include "Generator/Calibrator.h"
#include "Generator/Generator_d.h"
#include "Settings/CalibrationSettings.h"
#include <cmath>


bool Calibrator::inModeCalibration = false;
uint8 Calibrator::range[Chan::Count] = { 255, 255 };
uint8 Calibrator::signal[Chan::Count] = { 0 , 0 };


void Calibrator::SetFormWave(Chan::E ch, uint8 sig)
{
    bool isSine = (sig == 0);

    DGenerator::SetFormWave(ch, isSine ? TypeForm::Sine : TypeForm::Meander);

    DGenerator::SetFrequency(ch, isSine ? Value(1e3F) : Value("0.1", Order::One));
}


void Calibrator::SetOffset(Chan::E ch, uint8 param)
{
    static const float offset[4] =
    {
        0.0F,
        range[ch] < 3 ? 2.5F : 5.0F,
        0.0F,
        range[ch] < 3 ? -2.5F : -5.0F
    };

    DGenerator::SetOffset(ch, Value(offset[param]));
}


void Calibrator::SetK(uint8 channel, uint8 _signal, uint8 _range, uint8 param, int16 k)
{
    inModeCalibration = true;

    range[channel] = _range;

    signal[channel] = _signal;

    *setCal.GetK(channel, _signal, range[channel], param) = k;

    Chan::E ch = static_cast<Chan::E>(channel);

    SetFormWave(ch, _signal);

    if (param != 0 && _range > 2)
    {
        Amplifier::TuneAndLock(ch, _range > 2, Attenuation::_10Db); //-V547
    }

    AD5697::EnabledCalibrateMode(true);

    SetAmplitude(ch, param != 0);       // Для калибровки смещения нужно установить нулевой уровень на выходе, но аттенюатор не трогать

    SetOffset(ch, param);

    AD5697::EnabledCalibrateMode(false);

    Amplifier::Unlock();

    inModeCalibration = false;
}


void Calibrator::SetAmplitude(Chan::E ch, bool zeroAmplitude)
{
    if (zeroAmplitude)
    {
        DGenerator::SetAmplitude(ch, Value("0", Order::One));
    }
    else
    {
        static const float amplitude[6] =
        {
            100e-3F,
            300e-3F,
            1.0F,
            3.0F,
            10.0F
        };

        DGenerator::SetAmplitude(ch, Value(amplitude[range[ch]]));
    }
}


uint8 Calibrator::CalculateRange(Chan::E ch) //-V2506
{
    if(inModeCalibration)
    {
        return range[ch];
    }

    if (SettingsGenerator::AmplitudeValue(ch).Abs() == 0)
    {
        return std::fabs(SettingsGenerator::Offset(ch)) < 2.5 ? 0U : 3U;
    }

    double amplitude = SettingsGenerator::Amplitude(ch);

    if(amplitude > Attenuation(Attenuation::_10Db).Units())
    {
        return 4;
    }
    else if(amplitude > 1.0F)
    {
        return 3;
    }
    else if(amplitude > Attenuation(Attenuation::_10Db).Units() / 10) //-V2564
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
    uint8 r = CalculateRange(ch);

    int16 k = *setCal.GetK(static_cast<uint8>(ch), SettingsGenerator::FormIsSine(ch) ? 0U : 1U, r, 0U);

    return 1.0F + static_cast<float>(k) / 1e3F;
}


float Calibrator::GetOffsetK_Zero(Chan::E ch)
{
    uint8 r = CalculateRange(ch);

    return 2048.0F + static_cast<float>(*setCal.GetK(static_cast<uint8>(ch), SettingsGenerator::FormIsSine(ch) ? 0U : 1U, r, 2U));
}


double Calibrator::GetOffsetK_Negative(Chan::E ch)
{
    uint8 r = CalculateRange(ch);

    return 4095.0 - *setCal.GetK(static_cast<uint8>(ch), SettingsGenerator::FormIsSine(ch) ? 0U : 1U, r, 3U); //-V2564
}


double Calibrator::GetOffsetK_Positive(Chan::E ch)
{
    uint8 r = CalculateRange(ch);

    return 0.0 - *setCal.GetK(static_cast<uint8>(ch), SettingsGenerator::FormIsSine(ch) ? 0U : 1U, r, 1U); //-V2564
}


float Calibrator::GetFreqMeterK_Trig()
{
    float step = 4096.0F / 200.0F;

    return static_cast<float>(*setCal.GetFreqMeterK()) * step;
}


void Calibrator::SetFreqMeterK_Trig(int16 k)
{
    *setCal.GetFreqMeterK() = k;
}
