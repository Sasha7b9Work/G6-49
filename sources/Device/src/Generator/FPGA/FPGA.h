// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Generator/Generator_d.h"
#include "common/Command.h"
#include "Generator/FPGA/RegistersFPGA.h"
#include "Generator/FPGA/ParametersFPGA.h"


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

    // ������ ������������ ��������
    void WriteControlRegister();

    namespace PacketImpulse
    {
        // ������������� ����� ��������� � �����
        void SetNumberImpulses(uint n);

        extern Value periodImpulse;
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

        E Current(const Chan &);
    };
};
