// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "AD9952.h"
#include "AD5697.h"
#include "FPGA.h"
#include "common/Command.h"


class DGenerator
{
public:
    static const int DDS_NUM_POINTS = 8 * 1024;

    static void Init();

    static void EnableChannel(const Chan &, bool enable);

    static void SingleStart();

    static void SetFormWave(const Chan &, TypeForm::E form);

    static void SetOffset(const Chan &, Value offset);

    static void SetFrequency(const Chan &, Value frequency);

    static void SetAmplitude(const Chan &, Value amplitude);

    static void SetDuration(const Chan &, Value duration);

    static void SetDutyRatio(const Chan &, Value dutyRatio);

    static void SetPhase(const Chan &, Value phase);

    static void SetDelay(const Chan &, Value duration);

    static void SetManipulationDuration(const Chan &, Value duration);

    static void SetManipulationPeriod(const Chan &, Value period);

    static void SetPacketPeriod(const Chan &, Value duration);

    static void SetPacketNumber(const Chan &, Value number);

    static void SetPeriod(const Chan &, Value period);
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
    
    // ��������� ��������� � ������������ � �������� �����������
    static void Tune(Chan::E ch);

    static void Tune(Chan::E, int range);
    
    // ���������� ����������� �������� � ����� ����� ������, ������� ���������� � ���������. ��������� ��� �������� 10 ���, �� ������ 10 �� ����� ��������� �� 3.16
    static double GetAmplification(Chan::E ch);

    // ���������� �������� ��������� � �������������
    static void TuneAndLock(Chan::E ch, bool enabled, Attenuation::E attenuation);

    static void Lock();

    // ��������������
    static void Unlock();
   
    // ���������� true, ���� �� ������ ������ ������� ���������
    static bool IsEnabled(Chan::E ch) { return isEnabled[ch]; }

    // ��������/��������� ���������� ��������� ���������
    static void Enable(Chan::E ch, bool enable);

//    static void LogState();

private:
    
    static void SetAttenuation(Chan::E ch, Attenuation::E attenuation);

    static Attenuation::E attenuation[Chan::Count];
  
    // true, ���� ��������� ��������������� ������ �������
    static bool isEnabled[Chan::Count];

    static bool locked;
};


struct SettingsGenerator
{
    friend class DGenerator;
public:
    // ���������� ������������� �� ������ ���������. ��������� ������������ ��� ����� ����������
    static double Amplitude(Chan::E ch)     { return amplitude[ch].ToDouble(); }
    static Value AmplitudeValue(Chan::E ch) { return amplitude[ch]; }
    // ���������� ������������� �� ������ ��������
    static double Offset(Chan::E ch)        { return offset[ch].ToDouble(); }
    static Value OffsetValue(Chan::E ch)    { return offset[ch]; }
    // ���������� ������������� ������� �� ������
    static double Frequency(Chan::E ch) { return frequency[ch].ToDouble(); }
    // ���������� true, ���� �� ������ ch ����������� �������������� ����� �������
    static bool FormIsSine(Chan::E ch) { return waveIsSine[ch]; }
private:
    // true, ���� �� ������ ����������� ����� ������� "���������"
    static bool waveIsSine[Chan::Count];
    // ������� ������������� ��������� �� ������
    static Value amplitude[Chan::Count];
    // ������� ������������� �������� �� ������
    static Value offset[Chan::Count];
    // ������� ������� �� ������
    static Value frequency[Chan::Count];
};
