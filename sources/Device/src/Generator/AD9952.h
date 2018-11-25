#pragma once
#include "defines.h"
#include "Hardware/CPU.h"
#include "GeneratorSettingsTypes.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class AD9952
{
public:

    static void Init();

    static void SetFrequency(Chan::E ch, float frequency);

    static void SetAmplitude(Chan::E ch, float amplitude);

    static void SetPhase(Chan::E ch, float phase);

    /// Манипуляция
    class Manipulation
    {
    friend class AD9952;
    public:
        struct Type
        {
            enum E
            {
                OSK,    ///< Манипуляция импульсами со сглаженными фронтами
                FPGA    ///< Манипуляция прямоугольными импульсами
            } value;
            Type(uint8 v) : value((E)v) {};
            operator uint8() const { return (uint8)value; };
            bool Is(E v) const { return value == v; };
        };
        /// Установить/отменить модулирование синусоиды сигналом "пилы"
        static void SetEnabled(Chan::E ch, bool enable);
        static bool IsEnabled(Chan::E ch) { return enabled[ch]; };
        static void SetType(Chan::E ch, Type type);
        static Type GetType(Chan::E ch) { return type[ch]; };
    private:
        /// True означает, что манипуляция включена
        static bool enabled[Chan::Number];
        static Type type[Chan::Number];
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
        operator uint8() const { return (uint8)value;  };
        pString Name() const;
        bool Is(E v) const { return value == v; };
    };

    static void WriteToHardware(Chan::E ch, Register reg, uint value);
    static GeneratorWritePin ChipSelect(Chan::E ch);
    static void Reset();
    static void WriteRegister(Chan::E ch, Register reg);
    static void WriteCFR1(Chan::E ch);
    static void WriteCFR2(Chan::E ch);
    static void WriteARR(Chan::E ch);
    static void WriteASF(Chan::E ch);
    static void WriteFTW0(Chan::E ch);
    static void WritePOW(Chan::E ch);
};
