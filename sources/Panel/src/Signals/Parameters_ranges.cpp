// 2022/08/16 08:26:33 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Signals/Parameters.h"
#include "Signals/Wave.h" 
#include "Signals/Signals.h"


SMinMax Param::ValueInRange() const
{
    return SMinMax(Min(), Max(), GetValue());
}


Value PFrequency::Min() const
{
    if (form == A::Sine::form || form == B::Sine::form)
    {
        return Value("300", Order::Micro);
    }
    else if (form == A::RampPlus::form  || form == B::RampPlus::form ||
             form == A::RampMinus::form || form == B::RampMinus::form ||
             form == A::Triangle::form  || form == B::Triangle::form ||
             form == A::Free::form      || form == B::Free::form ||
             form == A::Meander::form   || form == B::Meander::form)
    {
        return Value("100", Order::Micro);
    }

    return Value(-1);
}


Value PFrequency::Max() const
{
    if (form == A::Sine::form || form == B::Sine::form)
    {
        return Value("100", Order::Mega);
    }
    else if (form == A::RampPlus::form  || form == B::RampPlus::form ||
             form == A::RampMinus::form || form == B::RampMinus::form ||
             form == A::Triangle::form  || form == B::Triangle::form ||
             form == A::Free::form      || form == B::Free::form ||
             form == A::Meander::form   || form == B::Meander::form)
    {
        return Value("10", Order::Mega);
    }

    return Value(1);
}


Value PPeriod::Min() const
{
    if (form == A::Impulse::form || form == B::Impulse::form)
    {
        return Value("20", Order::Nano);
    }
    else if (form == A::Packet::form)
    {
        return Value("20", Order::Nano);
    }

    return Value(-1);
}


Value PPeriod::Max() const
{
    if (form == A::Impulse::form || form == B::Impulse::form)
    {
        return Value("100", Order::Kilo);
    }
    else if (form == A::Packet::form)
    {
        return Value("100", Order::Kilo);
    }

    return Value(1);
}


Value PAmplitudePic::Min() const
{
    return Value(0);
}


Value PAmplitudePic::Max() const
{
    // Aмпл == 0В | offset[0 ... 5]
    // Ампл <= 1В | offset[0 ... 2.5]; ampl / 2 + fabs(см) <= 2.5
    // Ампл > 1В  | offset[0 ... 5];   ampl / 2 + fabs(см) <= 5

    DParam &param_ampl = form->FindParameter(TypeDParam::AmplitudePic);
    Value amplitude = param_ampl.GetValue();

    if (amplitude.Abs() == 0)
    {
        return Value(10);
    }

    Value result(5);

    if (amplitude.ToDouble() <= 1.0)
    {
        result.FromUnits(2, 500);
    }

    Value offset = form->FindParameter(TypeDParam::Offset).GetValue();
    result.Sub(offset);
    result.Mul(2);

    return result;
}


Value POffset::AbsLimit() const
{
    // Ampl = 0В  | [0 ... 5]
    // Ampl <= 1В | [0 ... 2.5], ampl / 2 + fabs(см) <= 2.5
    // Ampl > 1В  | [0 ... 5],   ampl / 2 + fabs(см) <= 5

    Value amplitude = form->FindParameter(TypeDParam::AmplitudePic).GetValue();

    Value result(5);

    if (amplitude.Abs() == 0)
    {
        result.FromUnits(5);
    }
    else if (amplitude.ToDouble() <= 1.0)
    {
        result.Div(2);
    }

    amplitude.Div(2);

    result.Sub(amplitude);

    return result;
}


Value POffset::Min() const
{
    Value result = AbsLimit();

    result.SetSign(-1);

    return result;
}


Value POffset::Max() const
{
    Value result = AbsLimit();

    result.SetSign(1);

    return result;
}


Value PDuration::Min() const
{
    if (form == A::Impulse::form || form == B::Impulse::form || form == A::Packet::form)
    {
        return Value("10", Order::Nano);
    }

    return Value(-1);
}


Value PDuration::Max() const
{
    if (form == A::Impulse::form || form == B::Impulse::form || form == A::Packet::form)
    {
        return Value("100", Order::Kilo);
    }

    return Value(1);
}


Value PPhase::Min() const
{
    return Value(0);
}


Value PPhase::Max() const
{
    return Value(360);
}


Value PDelay::Min() const
{
    if (form == A::Impulse::form)
    {
        return Value("20", Order::Nano);
    }

    return Value(-1);
}


Value PDelay::Max() const
{
    if (form == A::Impulse::form)
    {
        return A::Impulse::period->GetValue();
    }

    return Value(0);
}


Value PDurationManipulation::Min() const
{
    if (form == A::Sine::form || form == B::Sine::form)
    {
        return Value("10", Order::Nano);
    }

    return Value(-1);
}


Value PDurationManipulation::Max() const
{
    if (form == A::Sine::form || form == B::Sine::form)
    {
        return Value("10", Order::One);
    }

    return Value(1);
}


Value PPeriodManipulation::Min() const
{
    if (form == A::Sine::form || form == B::Sine::form)
    {
        return Value("20", Order::Nano);
    }

    return Value(-1);
}


Value PPeriodManipulation::Max() const
{
    if (form == A::Sine::form || form == B::Sine::form)
    {
        return Value("10000", Order::One);
    }

    return Value(1);
}


Value PPeriodPacket::Min() const
{
    if (form == A::Packet::form)
    {
        // Значение периода не может быть меньше (N - 1) * Tи + tи + 10нс

        PPeriod &par_period = (PPeriod &)form->FindParameter(TypeDParam::Period);
        IParam &par_number = form->FindParameter(TypeIParam::PacketNumber);
        PDuration &par_duration = (PDuration &)form->FindParameter(TypeDParam::Duration);

        if (par_period.Exist() && par_number.Exist() && par_duration.Exist())
        {
            Value min_value = par_period.GetValue();

            min_value.Mul((uint)(par_number.GetValue().Integer() - 1));

            min_value.Add(par_duration.GetValue());

            min_value.Add(Value("10", Order::Nano));

            return min_value;
        }

        return Value("20", Order::Nano);
    }

    return Value(-1);
}


Value PPeriodPacket::Max() const
{
    if (form == A::Packet::form)
    {
        return Value("100", Order::Kilo);
    }

    return Value(1);
}


Value IParam::Min() const
{
    if (type == TypeIParam::PacketNumber)
    {
        if (form == A::Packet::form)
        {
            Value("1", Order::One);
        }
    }

    return Value(-1);
}


Value IParam::Max() const
{
    if (type == TypeIParam::PacketNumber)
    {
        if (form == A::Packet::form)
        {
            return Value("1000000000", Order::One);
        }
    }

    return Value(1);
}
