// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "AD9952.h"
#include "AD5697.h"
#include "FPGA.h"
#include "common/Command.h"


namespace DGenerator
{
    static const int DDS_NUM_POINTS = 8 * 1024;

    void Init();

    void EnableChannel(const Chan &, bool enable);

    void SingleStart();

    void SetFormWave(const Chan &, TypeForm::E form);

    void SetOffset(const Chan &, Value offset);

    void SetFrequency(const Chan &, Value frequency);

    void SetAmplitude(const Chan &, Value amplitude);

    void SetDuration(const Chan &, Value duration);

    void SetDutyRatio(const Chan &, Value dutyRatio);

    void SetPhase(const Chan &, Value phase);

    void SetDelay(const Chan &, Value duration);

    void SetManipulationDuration(const Chan &, Value duration);

    void SetManipulationPeriod(const Chan &, Value period);

    void SetPacketPeriod(const Chan &, Value duration);

    void SetPacketNumber(const Chan &, Value number);

    void SetPeriod(const Chan &, Value period);
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

    explicit Attenuation(Attenuation::E v) : value(v) { }
    float Units() const;
    pString Name() const;
};


class Amplifier
{
public:
    
    static void Init();
    
    // Настроить усилитель в соответствии с текущими настройками
    static void Tune(const Chan &);

    static void Tune(const Chan &, int range);
    
    // Возвращает коэффициент усиления в разах всего тракта, включая аттенюатор и усилитель. Усилитель даёт усиление 10 раз, за каждые 10 Дб нужно разделить на 3.16
    static double GetAmplification(const Chan &);

    // Установить заданные настройки и заблокировать
    static void TuneAndLock(const Chan &, bool enabled, Attenuation::E attenuation);

    static void Lock();

    // Разблокировать
    static void Unlock();
   
    // Возвращает true, если на данном канале включён усилитель
    static bool IsEnabled(const Chan &ch) { return isEnabled[ch]; }

    // Включить/выключить аппаратный усилитель усилитель
    static void Enable(const Chan &, bool enable);

//    static void LogState();

private:
    
    static void SetAttenuation(const Chan &, Attenuation::E attenuation);

    static Attenuation::E attenuation[Chan::Count];
  
    // true, если усилитель соотвествующего канала включён
    static bool isEnabled[Chan::Count];

    static bool locked;
};


struct SettingsGenerator
{
public:
    // Возвращает установленную на канале амплитуду. Амплитуда возвращается без учёта аттёнюатора
    static double Amplitude(const Chan &ch)     { return amplitude[ch].ToDouble(); }
    static Value AmplitudeValue(const Chan &ch) { return amplitude[ch]; }
    // Возвращает установленное на канале смещение
    static double Offset(const Chan &ch)        { return offset[ch].ToDouble(); }
    static Value OffsetValue(const Chan &ch)    { return offset[ch]; }
    // Возвращает установленную частоту на канале
    static double Frequency(const Chan &ch) { return frequency[ch].ToDouble(); }
    // Возвращает true, если на канале ch установлена синусоидальная форма сигнала
    static bool FormIsSine(const Chan &ch) { return waveIsSine[ch]; }
    // true, если на канале установлена форма сигнала "синусоида"
    static bool waveIsSine[Chan::Count];
    // Текущая частота на канале
    static Value frequency[Chan::Count];
    // Текущая установленная амплитуда на канале
    static Value amplitude[Chan::Count];
    // Текущая установленное смещение на кнаале
    static Value offset[Chan::Count];
};
