// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Common.h"
#include "common/CommonTypes.h"
#include "Hardware/CPU.h"
#include "Hardware/HAL/HAL_PIO.h"


struct AD9952
{
    static void Init();

    // ��� ��������� ����� ������� "������" ����� �������� ��� �������, ����� ��� ������� ��������� � ������ ������ ��������-���� �� �����
    static void SetAmplitudeForMeander(const Chan &);

    static void SetAmplitude(const Chan &);

    static void SetFrequency(const Chan &);

    static void SetPhase(const Chan &, Value phase);

    // �����������
    struct Manipulation
    {
        // True ��������, ��� ����������� ��������
        static bool enabled[Chan::Count];
        // ����������/�������� ������������� ��������� �������� "����"
        static void SetEnabled(const Chan &, bool enable);
        static inline bool IsEnabled(const Chan &ch) { return enabled[ch]; };
    };

private:

    struct Register
    {
        enum E
        {
            CFR1,
            CFR2,
            ASF,
            ARR,
            FTW0,
            POW,
            Number
        } value;
        Register(E v) : value(v) { };
        operator uint8() const { return static_cast<uint8>(value); };
        pString Name() const;
        bool Is(E v) const { return value == v; };
    };

    static void WriteToHardware(const Chan &, Register::E reg, uint value);
    static StructPIN ChipSelect(const Chan &);
    static void Reset();
    static void WriteRegister(const Chan &, Register::E reg);
    static void WriteCFR1(const Chan &);
    static void WriteCFR2(const Chan &);
    static void WriteARR(const Chan &);
    static void WriteASF(const Chan &);
    static void WriteFTW0(const Chan &);
    static void WritePOW(const Chan &);

    static double phase[Chan::Count];
};
