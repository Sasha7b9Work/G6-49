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

    static void EnableChannel(Chan::E ch, bool enable);

    static void SingleStart();

    static void SetFormWave(Chan::E ch, TypeForm::E form);

    static void SetOffset(Chan::E ch, Value offset);

    static void SetFrequency(Chan::E ch, Value frequency);

    static void SetAmplitude(Chan::E ch, Value amplitude);

    static void SetDuration(Chan::E ch, Value duration);

    static void SetDutyRatio(Chan::E ch, Value dutyRatio);

    static void SetPhase(Chan::E ch, Value phase);

    static void SetDelay(Chan::E ch, Value duration);

    static void SetManipulationDuration(Chan::E ch, Value duration);

    static void SetManipulationPeriod(Chan::E ch, Value period);

    static void SetPacketPeriod(Chan::E ch, Value duration);

    static void SetPacketNumber(Chan::E ch, Value number);

    static void SetPeriod(Chan::E ch, Value period);
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
