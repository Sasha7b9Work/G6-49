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
        static void Write(const E, const uint64 value);
        static uint64 Read(const E);
    private:
        // ��������� ���������� ��������
        static uint64 content[Count];
    };

    struct RG0
    {
        enum E
        {
            /*
            *  RG0.10 | RG0.11 | RG0.12 |
            * --------+--------+--------+-----------------------------------------
            *    1    |   1    |   0    | �������������� ����� ������� Start/Stop
            *    1    |   1    |   1    | ������ ���� ������� Start/Stop
            * --------+--------+--------+----------------------------------------
            */
            _0_WriteData,               // � ���� ��� ���������� 0, ����� ��������� ������ ������� � ����
            _1_ImpulseA,                // 1, ���� � ������ � ���� ��������� ��������/�������������
            _2_ImpulseB,                // 1, ���� � ������ B ���� ��������� ��������/�������������
            _3_ManipulationOSK2,        // ����� 0, ���� ����� ������ 1 ������ ���������������� �������� OSK2 ("�����" �� AD9952 ������� ������)
            _4_ClockImpulse,            // �������� ������� ��� ��������� : 0 - 100��� (����� 10 ��), 1 - 1��� (����� 1 ���)
            _5_ManipulationOSK1,        // ����� 0, ���� ����� ������ 2 ������ ���������������� �������� OSK1 ("�����" �� AD9952 ������� ������)
            _del_6_ManipulationFPGA2,   // ����� 0, ���� ����� ������ 2 ������ ���������������� �������������� ��������� ������ 2
            _7_ClockAD9952,             // 0 - �������� ������� 100 ���, 1 - �������� ������� 1 ���
            _8_MeanderA,                // 1, ���� ������ �� ������ A
            _9_MeanderB,                // 1, ���� ������ �� ������ B
            _10_HandStartA,             // ���� ��� ���������� � 1, �� ������ ������ ������ �
            _11_HandStartB,             // ���� ��� ���������� � 1, �� ������ ������ ������ �
            _12_PacketImpulse,          // 1, ���� ������� �������� ����� ���������
            _13_StartMode0,             // ������� ��� ���������� ������� �������
            _14_StartMode1              // ������� ��� ���������� ������� �������
        };
    };
};