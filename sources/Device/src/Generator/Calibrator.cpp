#include "defines.h"
#include "Generator/Calibrator.h"
#include "Generator/Generator_d.h"
#include "Settings/CalibrationSettings.h"


bool Calibrator::inModeCalibration = false;
uint8 Calibrator::range[Chan::Count] = { 255, 255 };


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


void Calibrator::SetAmplitude(Chan::E ch)
{
    static const float amplitude[6] =
    {
        100e-3F,
        300e-3F,
        1.0F,
        3.0F,
        10.0F
    };

    DGenerator::SetAmplitude(ch, FloatValue(amplitude[range[ch]]));
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

    if(param == 0)                   // Для калибровки амплитуды просто устанавливаем смещение
    {
        DGenerator::SetOffset(ch, FloatValue(offset[param]));
    }
    else                            // А для калибровки смещений блокируем переключение аттенюаторов
    {
        Amplifier::Block();
        DGenerator::SetOffset(ch, FloatValue(offset[param]));
        Amplifier::Unblock();
    }
}


void Calibrator::SetK(uint8 channel, uint8 signal, uint8 _range, uint8 param, int16 k)
{
    inModeCalibration = true;

    range[channel] = _range;

    *setCal.GetK(channel, signal, range[channel], param) = k;

    Chan::E ch = static_cast<Chan::E>(channel);

    SetFormWave(ch, signal);

    SetAmplitude(ch);

    if(param != 0)              // Для калибровки смещения нужно установить нулевой уровень на выходе, но не аттенюатор не трогать
    {
        Amplifier::Block();
        DGenerator::SetAmplitude(ch, FloatValue(0, 0));
        Amplifier::Unblock();
    }

    SetOffset(ch, param);

    inModeCalibration = false;
}


uint8 Calibrator::CalculateRange(Chan::E ch)
{
    if(inModeCalibration)
    {
        return range[ch];
    }

    float amplitude = SettingsGenerator::Amplitude(ch);

    if(amplitude > Attenuation(Attenuation::_10Db).Units())
    {
        return 4;
    }
    else if(amplitude > 1.0F)
    {
        return 3;
    }
    else if(amplitude > Attenuation(Attenuation::_10Db).Units() / 10)
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

    return 1.0F + k / 1000.0F;
}


float Calibrator::GetOffsetK_Zero(Chan::E ch)
{
    uint8 r = CalculateRange(ch);

    bool isSine = SettingsGenerator::FormIsSine(ch);

    if(isSine)
    {
        return 2048.0F + *setCal.GetK(static_cast<uint8>(ch), 0U, r,  2U);
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
