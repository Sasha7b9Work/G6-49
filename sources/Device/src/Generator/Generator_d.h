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


namespace Amplifier
{
    void Init();
    
    // ��������� ��������� � ������������ � �������� �����������
    void Tune(const Chan &);

    void Tune(const Chan &, int range);
    
    // ���������� ����������� �������� � ����� ����� ������, ������� ���������� � ���������. ��������� ��� �������� 10 ���, �� ������ 10 �� ����� ��������� �� 3.16
    double GetAmplification(const Chan &);

    // ���������� �������� ��������� � �������������
    void TuneAndLock(const Chan &, bool enabled, Attenuation::E attenuation);

    void Lock();

    // ��������������
    void Unlock();
   
    // ���������� true, ���� �� ������ ������ ������� ���������
    bool IsEnabled(const Chan &);

    // ��������/��������� ���������� ��������� ���������
    void Enable(const Chan &, bool enable);
};


struct SettingsGenerator
{
public:
    // ���������� ������������� �� ������ ���������. ��������� ������������ ��� ����� ����������
    static double Amplitude(const Chan &ch)     { return amplitude[ch].ToDouble(); }
    static Value AmplitudeValue(const Chan &ch) { return amplitude[ch]; }
    // ���������� ������������� �� ������ ��������
    static double Offset(const Chan &ch)        { return offset[ch].ToDouble(); }
    static Value OffsetValue(const Chan &ch)    { return offset[ch]; }
    // ���������� ������������� ������� �� ������
    static double Frequency(const Chan &ch) { return frequency[ch].ToDouble(); }
    // ���������� true, ���� �� ������ ch ����������� �������������� ����� �������
    static bool FormIsSine(const Chan &ch) { return waveIsSine[ch]; }
    // true, ���� �� ������ ����������� ����� ������� "���������"
    static bool waveIsSine[Chan::Count];
    // ������� ������� �� ������
    static Value frequency[Chan::Count];
    // ������� ������������� ��������� �� ������
    static Value amplitude[Chan::Count];
    // ������� ������������� �������� �� ������
    static Value offset[Chan::Count];
};
