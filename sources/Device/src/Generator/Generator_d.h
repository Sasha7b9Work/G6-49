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


namespace SettingsGenerator
{
    // true, ���� �� ������ ����������� ����� ������� "���������"
    extern bool waveIsSine[Chan::Count];

    // ������� ������� �� ������
    extern Value frequency[Chan::Count];

    // ������� ������������� ��������� �� ������
    extern Value amplitude[Chan::Count];

    // ������� ������������� �������� �� ������
    extern Value offset[Chan::Count];

    // ���������� ������������� �� ������ ���������. ��������� ������������ ��� ����� ����������
    inline double Amplitude(const Chan &ch) { return amplitude[ch].ToDouble(); }

    inline Value AmplitudeValue(const Chan &ch) { return amplitude[ch]; }

    // ���������� ������������� �� ������ ��������
    inline double Offset(const Chan &ch) { return offset[ch].ToDouble(); }

    inline Value OffsetValue(const Chan &ch) { return offset[ch]; }

    // ���������� ������������� ������� �� ������
    inline double Frequency(const Chan &ch) { return frequency[ch].ToDouble(); }

    // ���������� true, ���� �� ������ ch ����������� �������������� ����� �������
    inline bool FormIsSine(const Chan &ch) { return waveIsSine[ch]; }
};
