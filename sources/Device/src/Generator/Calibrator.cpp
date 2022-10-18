// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Generator/Calibrator.h"
#include "Generator/Generator_d.h"
#include "Settings/CalibrationSettings.h"
#include <cmath>


namespace Calibrator
{
    static uint8 CalculateRange(const Chan &);

    // Устанавливает форму сигнала для соотвествующей формы сигнала
    static void SetFormWave(const Chan &, CalSignal::E);

    // Установить амплитуду для заданного диапазона
    static void SetAmplitude(const Chan &, bool zeroAmplitude);

    // Установить смещение для заданного параметра
    static void SetOffset(const Chan &, uint8 param);

    // Если true, то мы находимся в режиме калибровки, и диапазон нужно не рассчитывать, исходя из установленного значения, а брать тот, который сейчас калибруется
    static bool inModeCalibration = false;

    // Для этого диапазона сейчас происходит калибровка
    static uint8 range[Chan::Count] = { 255, 255 };

    // Для этой формы сигнала происходит калибровка. 0 - синус, 1 - произвольный
    CalSignal::E signal[Chan::Count] = { CalSignal::Sine , CalSignal::Sine };
}


void Calibrator::SetFormWave(const Chan &ch, CalSignal::E sig)
{
    DGenerator::SetFormWave(ch, (sig == CalSignal::Sine) ? TypeForm::Sine : TypeForm::Meander);

    DGenerator::SetFrequency(ch, (sig == CalSignal::Sine) ? Value(1e3F) : Value("0.1", Order::One));
}


void Calibrator::SetOffset(const Chan &ch, uint8 param)
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


void Calibrator::SetK(uint8 channel, CalSignal::E _signal, uint8 _range, uint8 param, int16 k)
{
    inModeCalibration = true;

    range[channel] = _range;

    signal[channel] = _signal;

    *setCal.GetK(channel, _signal, range[channel], param) = k;

    Chan ch(channel);

    SetFormWave(ch, _signal);

    AD5697::EnabledCalibrateMode(true);

    Amplifier::Lock();

    SetAmplitude(ch, (param != 0));       // Для калибровки смещения нужно установить нулевой уровень на выходе, но аттенюатор не трогать

    SetOffset(ch, param);

    Amplifier::Tune(ch, (int)_range);

    AD5697::EnabledCalibrateMode(false);

    Amplifier::Unlock();

    inModeCalibration = false;
}


void Calibrator::SetAmplitude(const Chan &ch, bool zeroAmplitude)
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


uint8 Calibrator::CalculateRange(const Chan &ch)
{
    if(inModeCalibration)
    {
        return range[ch];
    }

    if (SettingsGenerator::AmplitudeValue(ch).IsZero())
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


float Calibrator::GetAmplitudeK(const Chan &ch)
{
    uint8 r = CalculateRange(ch);

    int16 k = *setCal.GetK((uint8)ch, CalSignal::FromForm(TypeForm::Current(ch)), r, 0U);

    return 1.0F + (float)k / 1e3F;
}


float Calibrator::GetOffsetK_Zero(const Chan &ch)
{
    uint8 r = CalculateRange(ch);

    return 2048.0F + (float)(*setCal.GetK((uint8)ch, CalSignal::FromForm(TypeForm::Current(ch)), r, 2U));
}


double Calibrator::GetOffsetK_Negative(const Chan &ch)
{
    uint8 r = CalculateRange(ch);

    return 4095.0 - *setCal.GetK((uint8)ch, CalSignal::FromForm(TypeForm::Current(ch)), r, 3U);
}


double Calibrator::GetOffsetK_Positive(const Chan &ch)
{
    uint8 r = CalculateRange(ch);

    return 0.0 - *setCal.GetK((uint8)ch, CalSignal::FromForm(TypeForm::Current(ch)), r, 1U);
}


float Calibrator::GetFreqMeterK_Trig()
{
    float step = 4096.0F / 200.0F;

    return (float)(*setCal.GetFreqMeterK()) * step;
}


void Calibrator::SetFreqMeterK_Trig(int16 k)
{
    *setCal.GetFreqMeterK() = k;
}
