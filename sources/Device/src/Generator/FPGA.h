#pragma once
#include "Generator/Generator_d.h"
#include "common/Command.h"


struct FPGA
{
    static const int NUM_POINTS = 1024 * 8;

    static void Init();

    static void SetWaveForm(Chan::E ch, TypeForm::E form);

    static void SetFrequency(Chan::E ch);

    static void SetAmplitude();

    static void SetDurationImpulse(Chan::E ch, Value duration);

    static void SetPeriodImpulse(Chan::E ch, Value period);

    // 1 - ������������� ����������, 0 - ������������� ����������
    static void SetPolarity(Chan::E ch, uint8 polarity);

    // ��������� ������ ������� ��� ������������� (0) ������� � ����������� (1) ������� signal
    static void SetStartMode(Chan::E ch, uint8 signal, StartMode mode);
    
    // ������ ����������� ������
    static void SingleStart();

    static void TransformCodeToData(const uint8 codeIn[FPGA::NUM_POINTS * 2], float dataOut[FPGA::NUM_POINTS]);

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
        } value;
        explicit ModeWork(E v) : value(v) {};
    };

    struct ClockFrequency
    {
        enum E
        {
            _100MHz,
            _1MHz
        } value;
        explicit ClockFrequency(E v) : value(v) {};
    };

    // �������� ����
    struct RG
    {
        enum E
        {
            _0_Control,
            _1_Freq,
            _2_Amplitude,
            _3_RectA,           // \brief ������� ���� ������� ������������� ����� ������ �. ������������ ��� ��������� ���� �����. 1-� ��� ������-
                                // ���� ������ �������� �������������� ������� - ��������. 2-� ��� ������������� ������� �������� ������� - ��� ��
                                // ���������.
                                // b0...b13 - ������ ��������, b14...b27 - ������� ��������
            _4_RectB,           // ������ _3_RectA ��� ������ B
            _5_PeriodImpulseA,
            _6_DurationImpulseA,    // ���� �� ������������
            _7_PeriodImpulseB,
            _8_DurationImpulseB,
            _9_FreqMeter,
            _10_Offset,
            _11_Start,
            Count
        } value;
        explicit RG(E v) : value(v) { };
    };

    static void SetClockAD992(ClockFrequency::E clock);

    static ClockFrequency::E clock;
    
    // ����� ������ ����
    static ModeWork::E modeWork[Chan::Count];

    static inline ModeWork::E CurrentMode(Chan::E ch) { return modeWork[ch]; }
    
    // ���������� ��������� �� ����� �������, ������������ �� ������
    static uint8 *DataFreeSignal(Chan::E ch);
    
    // ��������� ������ �������, ������������ � ������
    static void SaveExtSignal(Chan::E ch, uint8 *data);
    
    // ���������� ��������� �� ����� ������������� ������� (���������� �����������)
    static uint8 *DataDDS(Chan::E ch);
    
    // �������� �������� � �������
    static void WriteRegister(RG::E reg, uint64 value);

private:
    struct RG0
    {
        enum E
        {
            _0_WriteData,           // � ���� ��� ���������� 0, ����� ��������� ������ ������� � ����
            _1_ImpulseA,            // 1, ���� � ������ � ���� ��������� ��������/�������������
            _2_ImpulseB,            // 1, ���� � ������ B ���� ��������� ��������/�������������
            _3_ManipulationOSK2,    // ����� 0, ���� ����� ������ 1 ������ ���������������� �������� OSK2 ("�����" �� AD9952 ������� ������)
            deleted__4_ManipulationFPGA1,   // ����� 0, ���� ����� ������ 1 ������ ���������������� �������������� ��������� ������ 1
            _5_ManipulationOSK1,    // ����� 0, ���� ����� ������ 2 ������ ���������������� �������� OSK1 ("�����" �� AD9952 ������� ������)
            deleted_6_ManipulationFPGA2,   // ����� 0, ���� ����� ������ 2 ������ ���������������� �������������� ��������� ������ 2
            _7_Freq_MHz,            // 1, ���� �������� ������� 1���
            _8_MeanderA,            // 1, ���� ������ �� ������ A
            _9_MeanderB,            // 1, ���� ������ �� ������ B
            _10_HandStartA,         // ���� ��� ���������� � 1, �� ������ ����� �������
            _11_HandStartB,
            _12_PacketImpulse,      // 1, ���� ������� �������� ����� ���������
            _13_StartMode0,         // ������� ��� ���������� ������� �������
            _14_StartMode1          // ������� ��� ���������� ������� �������
        };
    };
    
    static void SetFormSine(Chan::E ch);
    
    // ���������� ����� ����+
    static void SetFormRampPlus(Chan::E ch);
    
    // ���������� ����� ����-
    static void SetFormRampMinus(Chan::E ch);
    
    // ���������� ����� �����������
    static void SetFormTriangle(Chan::E ch);
    
    // ���������� ����� ������������� �������, ������������ � ������
    static void SetFormFree(Chan::E ch);
    
    static void SetFormMeander(Chan::E ch);
    
    static void SetFormImpulse(Chan::E ch);
    
    static void SetFormPackedImpulse(Chan::E ch);
    
    // ������� ������������ ����� ����� ������� � ����
    static void SendData();

    // ������� ������������ ����� ������ ������ � ����
    static void SendDataChannel(Chan::E ch);
    
    // ���������� �� A0_RG...A3_RG �����, ������������� ��������
    static void WriteAddress(RG::E reg);
    
    // ������ ������������ ��������
    static void WriteControlRegister();
    
    // ���������� ����, ��������������� ������������� � ������������ ��������
    static void WriteMaxAmplitude(Chan::E ch);
    
    // ���������� ����, ��������������� ������ �������
    static void SetBitsStartMode(uint16 &data);
    static void SetBitsStartMode(Chan::E ch, uint16 &data);

    // ���������� true, ���� �� ������ ch �������� DDS
    static bool InModeDDS(Chan::E ch);

    static StartMode startMode[Chan::Count][2];          // ����� ������� ��� ������������� ������� (0) � ��� ����������� ������� (1)
    
    static uint64 registers[RG::Count];     // ����� �������� ���������� � �������� ��������
};
