#include "defines.h"
#include "Generator/Calibrator.h"
#include "Generator/Generator_d.h"
#include "Settings/CalibrationSettings.h"



/// Устанавливает форму сигнала для соотвествующей формы сигнала
static void SetFormWave(Chan::E ch, uint8 signal)
{
    if(signal == 0)
    {
        DGenerator::SetFormWave(ch, Form::Sine);
    }
    else
    {
        DGenerator::SetFormWave(ch, Form::Meander);
    }
}


/// Установить амплитуду в зависимости от выбранного диапазона
static void SetAmplitude(Chan::E ch, uint8 range)
{
    static const float amplitude[6] =
    {
        30e-3F,
        100e-3F,
        300e-3F,
        1.0F,
        3.0F,
        10.0F
    };

    DGenerator::SetAmplitude(ch, FloatValue(amplitude[range]));
}


static void SetOffset(Chan::E ch, uint8 param)
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


uint8 Calibrator::CalculateRange(float amplitude)
{
    if(amplitude > 3.16F)
    {
        return 5;
    }
    else if(amplitude > 1.0F)
    {
        return 4;
    }
    else if(amplitude > 0.316F)
    {
        return 3;
    }
    else if(amplitude > 0.1F)
    {
        return 2;
    }
    else if(amplitude > 0.0316F)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


float Calibrator::GetAmplitudeK(Chan::E ch, Form::E form, float amplitude)
{
    uint8 range = CalculateRange(amplitude);

    int16 k = *setCal.GetK(static_cast<uint8>(ch), form == Form::Sine ? 0U : 1U, range, 0U);

    return k / 10.0F;
}
