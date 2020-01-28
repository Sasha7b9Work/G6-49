#pragma once
#include "defines.h"
#include "AD9952.h"
#include "AD5697.h"
#include "FPGA.h"
#include "common/Command.h"


struct SetGenerator
{
friend class DGenerator;
public:
    /// Возвращает установленную на канале амплитуду. Амплитуда возвращается без учёта аттёнюатора
    static float Amplitude(Chan::E ch) { return amplitude[ch]; }
    /// Возвращает установленное на канале смещение
    static float Offset(Chan::E ch) { return offset[ch]; }
    /// Возвращает установленную частоту на канале
    static float Frequency(Chan::E ch) { return frequency[ch]; }
    /// Возвращает true, если на канале ch установлена синусоидальная форма сигнала
    static bool FormIsSine(Chan::E ch) { return waveIsSine[ch]; }
private:
    /// true, если на канале установлена форма сигнала "синусоида"
    static bool waveIsSine[Chan::Count];
    /// Текущая установленная амплитуда на канале
    static float amplitude[Chan::Count];
    /// Текущая установленное смещение на кнаале
    static float offset[Chan::Count];
    /// Текущая частота на канале
    static float frequency[Chan::Count];
};


class DGenerator
{
public:
    static const int DDS_NUM_POINTS = 8 * 1024;

    static void Init();

    static void EnableChannel(Chan::E ch, bool enable);

    static void SetFormWave(Chan::E ch, TypeForm::E form);

    static void SetOffset(Chan::E ch, FloatValue offset);

    static void SetFrequency(Chan::E ch, FloatValue frequency);

    static void SetAmplitude(Chan::E ch, FloatValue amplitude);

    static void SetDuration(Chan::E ch, FloatValue duration);

    static void SetDutyRatio(Chan::E ch, FloatValue dutyRatio);

    static void SetPhase(Chan::E ch, FloatValue phase);

    static void SetDelay(Chan::E ch, FloatValue duration);

    static void SetManipulationDuration(Chan::E ch, FloatValue duration);

    static void SetManipulationPeriod(Chan::E ch, FloatValue period);

    static void SetPacketPeriod(Chan::E ch, FloatValue duration);

    static void SetPacketNumber(Chan::E ch, FloatValue number);

    static void SetPeriod(Chan::E ch, FloatValue period);
};


struct Attenuation
{
    enum E
    {
        _0Db,
        _10Db,
        _20Db,
        _30Db,
        Count
    } value;

    Attenuation(E v) : value(v) { };

    uint Multiplier() const;
};


class Attenuator
{
friend class Amplifier;

public:
    /// Возвращает установленный коэффициент ослабления на канале
    static Attenuation GetAttenuation(Chan::E ch);

private:
    static void SetAttenuation(Chan::E ch, Attenuation::E attenuation);

    static void Init();

    static Attenuation::E attenuation[Chan::Count];
};


class Amplifier
{
friend class DGenerator;

private:
    static void Init();

    static void Tune(Chan::E ch);

    static void SetState(Chan::E ch, bool state);
};
