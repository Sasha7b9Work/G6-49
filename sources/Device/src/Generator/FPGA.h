// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Generator/Generator_d.h"
#include "common/Command.h"


namespace FPGA
{
    static const int NUM_POINTS = 1024 * 8;

    void Init();

    void SetWaveForm(const Chan &, TypeForm::E);

    void SetFrequency(const Chan &);

    void SetAmplitude();

    void SetDurationImpulse(const Chan &, const Value &duration);

    void SetPeriodImpulse(const Chan &, const Value &period);

    // �������� ����� ���������� �� ������� ������� � ������� �������
    void SetDelayStartStop(const Value &delay);

    // 1 - ������������� ����������, 0 - ������������� ����������
    void SetPolarity(const Chan &, uint8 polarity);

    // ��������� ������ ������� ��� ������������� (0) ������� � ����������� (1) ������� signal
    void SetStartMode(const Chan &, uint8 signal, StartMode::E);

    void EnableStartStopMode(StartStopMode::E);

    // ������ ����������� ������
    void SingleStart();

    void TransformCodeToData(const uint8 codeIn[FPGA::NUM_POINTS * 2], float dataOut[FPGA::NUM_POINTS]);

    // ���������� ��������� �� ����� �������, ������������ �� ������
    uint8 *DataFreeSignal(const Chan &);

    // ��������� ������ �������, ������������ � ������
    void SaveExtSignal(const Chan &, uint8 *data); 

    // ���������� ��������� �� ����� ������������� ������� (���������� �����������)
    uint8 *DataDDS(const Chan &);

    namespace PacketImpulse
    {
        // ������������� ����� ��������� � �����
        void SetNumberImpules(uint n);

        // ������������� ������ ���������� �����
        void SetPeriodPacket(const Value &);

        extern Value periodImpulse;
        extern Value durationImpulse;
    };

    namespace ModeWork
    {
        enum E
        {
            None,
            DDS,            // �������� �� ����������� ������ ������� ������������ �������� - ����, �����������
            Impulse,        // �����, � ������� ������c� ����� ����� ������ �������
            Impulse2,       // �����, � ������� �������� ����� ���������� �������. ��� ���� ����� ������������ �������� ������� ������ ���. �������
            Rectangle,
            Meander,
            PackedImpulse,  // ������ ���������
            Sine,
            Free,
            Count
        };
    };

    // �������� ������� ������������� ������������ ��������
    struct ClockAD992
    {
        enum E
        {
            _100MHz,
            _1MHz
        };

        static void Set(E);

        static E Get();

    private:

        static E value;
    };

    // ������� ������� ��� ������������ �������� � ������� ���������. ��� ������������ �������� [0 ��� ... 40 ���]
    // ������� ������� 100 ���, ��� ������������ ����� - 1 ���
    struct ClockImpulse
    {
        enum E
        {
            _100MHz,
            _1MHz
        };

        static void Set(E v);
        static bool Is1MHz() { return value == _1MHz; }
        static bool Is100MHz() { return value == _100MHz; }
        static E Get() { return value; };
    private:
        static E value;
    };

    struct Register
    {
        // �������� ����
        enum E
        {
            _0_Control,
            _1_Freq,
            _2_Amplitude,
            _3_RectA,               // \brief ������� ���� ������� ������������� ����� ������ �. ������������ ��� ��������� ���� �����. 1-� ��� ������-
                                    // ���� ������ �������� �������������� ������� - ��������. 2-� ��� ������������� ������� �������� ������� - ��� ��
                                    // ���������.
                                    // b0...b13 - ������ ��������, b14...b27 - ������� ��������
            _4_RectB,               // ������ _3_RectA ��� ������ B
            _5_PeriodImpulseA,      // ������ ��������� ������ �
            _6_DurationImpulseA,    // ������������ ��������� ������ �
            _7_PeriodImpulseB,      // ������ ��������� ������ �. � ������ ������-����� - �������� ����� ������ � ������ �������
            _8_DurationImpulseB,    // ������������ ��������� ������ �
            _9_FreqMeter,
            _10_Offset,
            _11_Start,
            Count
        };
        // �������� �������� � �������
        static void Write(E, uint64 value);
        static uint64 Read(E);
    private:
        // ��������� ���������� ��������
        static uint64 content[Count];
    };
};
