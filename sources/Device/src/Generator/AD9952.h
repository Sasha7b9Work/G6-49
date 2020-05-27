#pragma once
#include "common/Common.h"
#include "common/CommonTypes.h"
#include "Hardware/CPU.h"
#include "Hardware/HAL/HAL_PIO.h"


struct AD9952
{
    static void Init();

    static void SetAmplitude(Chan::E ch);

    static void SetFrequency(Chan::E ch);

    static void SetPhase(Chan::E ch, DoubleValue phase);

    // ћанипул€ци€
    struct Manipulation
    {
        // True означает, что манипул€ци€ включена
        static bool enabled[Chan::Count];
        // ”становить/отменить модулирование синусоиды сигналом "пилы"
        static void SetEnabled(Chan::E ch, bool enable);
        static inline bool IsEnabled(Chan::E ch) { return enabled[ch]; };
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

    static void WriteToHardware(Chan::E ch, Register reg, uint value);
    static StructPIN ChipSelect(Chan::E ch);
    static void Reset();
    static void WriteRegister(Chan::E ch, Register reg);
    static void WriteCFR1(Chan::E ch);
    static void WriteCFR2(Chan::E ch);
    static void WriteARR(Chan::E ch);
    static void WriteASF(Chan::E ch);
    static void WriteFTW0(Chan::E ch);
    static void WritePOW(Chan::E ch);

    struct ClockFrequency
    {
        enum E
        {
            _100MHz,
            _1MHz
        } value;
        explicit ClockFrequency(E v) : value(v) {};
    };

    static ClockFrequency::E clock;

    static double phase[Chan::Count];
};
