// 2022/08/30 11:42:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Generator/FPGA/Clock.h"
#include "Generator/FPGA/FPGA.h"


namespace FPGA
{
    namespace Clock
    {
        namespace Impulse
        {
            static E clock = _100MHz;

            // Если при установке длительности импульса нужно изменять опорную частоту - переписать все значения
            static void RewriteRegisters();

            static bool Is1MHz() { return clock == _1MHz; }

            static void Set(E);

            // Возвращает true, если хотя бы одно значение периода либо длительности меньше value
            static bool AtLeastOneValueGreater(const Value &value);

            // Возращает true, если все значения меньше либо равны value
            static bool AllValuesLessOrEqual(const Value &value);
        }

        namespace AD992
        {
            static E clock = _100MHz;
        }

    }
}


bool FPGA::Clock::Impulse::AtLeastOneValueGreater(const Value &)
{
    return false;
}


bool FPGA::Clock::Impulse::AllValuesLessOrEqual(const Value &)
{
    return false;
}


void FPGA::Clock::Impulse::RecalculateRegistersIfNeed()
{
    if (AtLeastOneValueGreater(Value(40)) && Is100MHz())
    {
        Set(_1MHz);

        RewriteRegisters();
    }
    else if (AllValuesLessOrEqual(Value(40)) && Is1MHz())
    {
        Set(_100MHz);

        RewriteRegisters();
    }
}


bool FPGA::Clock::Impulse::Is100MHz()
{
    return clock == _100MHz;
}


int FPGA::Clock::Impulse::GetDivider()
{
    return (clock == _100MHz) ? 10 : 1000;
}


void FPGA::Clock::Impulse::Set(E _clock)
{
    clock = _clock;

    uint64 reg = Register::Read(Register::_0_Control);

    if (clock == _100MHz)
    {
        _CLEAR_BIT(reg, RG0::_4_ClockImpulse);
    }
    else
    {
        _SET_BIT(reg, RG0::_4_ClockImpulse);
    }

    Register::Write(Register::_0_Control, reg);
}


void FPGA::Clock::Impulse::RewriteRegisters()
{
    /*
    * --+--------------------+-----------+
        | параметра          | коррекция |
      --+--------------------+-----------+
      5 | period packet      | -         | +
        | period impulse A   | +         | +
      --+--------------------+-----------+
      6 | diration impulse A | -         | +
        | number impulse     | -         | +
      --+--------------------+-----------+
      7 | period impulse B   | +         | +
        | delay start stop   | +         |
      --+--------------------+-----------+
      8 | duration impulse B | -         | +
      --+--------------------+-----------+
    */

    /*
    *   В режиме импульсов переписываем:
    *   - период
    *   - длительность
    *       если включён режим стартА/стопВ - задержку
    * 
    *   В режиме пакета:
    *   
    */
//    ModeWork::E mode = ModeWork::Current(ch);
//
//    switch (mode)
//    {
//    case ModeWork::Impulse:
//        if (StartStop::Mode::Current() == StartStop::Mode::Disable)     // СтартА/СтопВ выключено
//        {
//
//        }
//        else                                                            // СтартА/СтопВ включено
//        {
//
//        }
//        break;
//
//    case ModeWork::PackedImpulse:
//        break;
//
//    case ModeWork::None:
//    case ModeWork::DDS:
//    case ModeWork::Rectangle:
//    case ModeWork::Meander:
//    case ModeWork::Sine:
//    case ModeWork::Free:
//    case ModeWork::Count:
//        break;
//    }
}


void FPGA::Clock::AD992::Set(E _clock)
{
    clock = _clock;
    WriteControlRegister();
}


bool FPGA::Clock::AD992::Is100MHz()
{
    return (clock == _100MHz);
}
