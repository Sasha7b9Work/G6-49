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

    // 1 - ������������� ����������, 0 - ������������� ����������
    void SetPolarity(const Chan &, uint8 polarity);

    // ��������� ������ ������� ��� ������������� (0) ������� � ����������� (1) ������� signal
    void SetStartMode(const Chan &, uint8 signal, StartMode::E);

    void EnableStartStopMode(StartStopMode::E);

    // ������ ����������� ������
    void SingleStart();

    void TransformCodeToData(const uint8 codeIn[FPGA::NUM_POINTS * 2], float dataOut[FPGA::NUM_POINTS]);

    struct PacketImpulse
    {
        // ������������� ����� ��������� � �����
        static void SetNumberImpules(uint n);

        // ������������� ������ ���������� �����
        static void SetPeriodPacket(Value period);

        static Value periodImpulse;

        static Value durationImpulse;
    };

    struct ModeWork
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

    // ���������� ��������� �� ����� �������, ������������ �� ������
    uint8 *DataFreeSignal(const Chan &);

    // ��������� ������ �������, ������������ � ������
    void SaveExtSignal(const Chan &, uint8 *data);

    // ���������� ��������� �� ����� ������������� ������� (���������� �����������)
    uint8 *DataDDS(const Chan &);

    // ����� ������ ����
    extern ModeWork::E modeWork[Chan::Count];

    static inline ModeWork::E CurrentMode(const Chan &ch) { return modeWork[ch]; }

    // �������� ������� ������������� ������������ ��������
    namespace ClockFrequencyAD992
    {
        enum E
        {
            _100MHz,
            _1MHz
        };

        extern E value;

        void Set(E);
    };
    
    // �������� ������� ������������� ���������
    struct ClockFrequencyImpulse
    {
        enum E
        {
            _100MHz,
            _1MHz
        };
    };
    
    extern ClockFrequencyImpulse::E clockImpulse;

    namespace Register
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
            _7_PeriodImpulseB,      // ������ ��������� ������ �
            _8_DurationImpulseB,    // ������������ ��������� ������ �
            _9_FreqMeter,
            _10_Offset,
            _11_Start,
            Count
        };
        // �������� �������� � �������
        void Write(E reg, uint64 value);

        uint64 Read(E reg);
    }
};
