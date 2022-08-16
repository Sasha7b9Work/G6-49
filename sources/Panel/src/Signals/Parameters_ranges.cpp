// 2022/08/16 08:26:33 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Signals/Parameters.h"
#include "Signals/Wave.h" 


Value PPeriodPacket::CalculateMinValue() const
{
    // Значение периода не может быть меньше (N - 1) * Tи + tи + 10нс

    PPeriod *par_period = (PPeriod *)form->FindParameter(TypeDParam::Period);
    IParam *par_number = form->FindParameter(TypeIParam::PacketNumber);
    PDuration *par_duration = (PDuration *)form->FindParameter(TypeDParam::Duration);

    if (par_period && par_number && par_duration)
    {
        Value min_value = par_period->GetValue();

        min_value.Mul((uint)(par_number->GetValue().Integer() - 1));

        min_value.Add(par_duration->GetValue());

        min_value.Add(Value("10", Order::Nano));

        return min_value;
    }

    return Value("20", Order::Nano);
}


SMinMax Param::ValueInRange() const
{
    return SMinMax(Min(), Max(), GetValue());
}


Value DParam::Min() const
{
    switch (type)
    {
    case TypeDParam::DurationFall:          break;
    case TypeDParam::DurationStady:         break;
    case TypeDParam::DutyFactor:            break;
    case TypeDParam::DurationManipulation:  break;
    case TypeDParam::PeriodManipulation:    break;
    case TypeDParam::PeriodPacket:          break;

    case TypeDParam::Frequency:
    case TypeDParam::Period:                break;
    case TypeDParam::AmplitudePic:          break;
    case TypeDParam::Offset:                break;
    case TypeDParam::Duration:              break;
    case TypeDParam::Phase:                 break;
    case TypeDParam::Delay:                 break;

    case TypeDParam::Count:                 break;
    }

    return Value(-1);
}


Value DParam::Max() const
{
    switch (type)
    {
    case TypeDParam::DurationFall:          break;
    case TypeDParam::DurationStady:         break;
    case TypeDParam::DutyFactor:            break;
    case TypeDParam::DurationManipulation:  break;
    case TypeDParam::PeriodManipulation:    break;
    case TypeDParam::PeriodPacket:          break;

    case TypeDParam::Frequency:
    case TypeDParam::Period:                break;
    case TypeDParam::AmplitudePic:          break;
    case TypeDParam::Offset:                break;
    case TypeDParam::Duration:              break;
    case TypeDParam::Phase:                 break;
    case TypeDParam::Delay:                 break;

    case TypeDParam::Count:                 break;
    }

    return Value(-1);
}


Value PFrequency::Min() const
{
    return Value(-1);
}


Value PFrequency::Max() const
{
    return Value(-1);
}


Value PPeriod::Min() const
{
    return Value(-1);
}


Value PPeriod::Max() const
{
    return Value(-1);
}


Value PAmplitudePic::Min() const
{
    return Value(-1);
}


Value PAmplitudePic::Max() const
{
    return Value(-1);
}


Value POffset::Min() const
{
    return Value(-1);
}


Value POffset::Max() const
{
    return Value(-1);
}


Value PDuration::Min() const
{
    return Value(-1);
}


Value PDuration::Max() const
{
    return Value(-1);
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
    return Value(-1);
}


Value PDelay::Max() const
{
    return Value(-1);
}


Value IParam::Min() const
{
    switch (type)
    {
    case TypeIParam::PacketNumber:      break;
    case TypeIParam::Count:             break;
    }

    return Value(0);
}


Value IParam::Max() const
{
    switch (type)
    {
    case TypeIParam::PacketNumber:      break;
    case TypeIParam::Count:             break;
    }

    return Value(0);
}


/*
SMinMax POffset::InRange(Form *form)
{
    // Ампл == 0  | [0 ... 5]
    // Ампл <= 1В | [0 ... 2.5], ampl / 2 + fabs(см) <= 2.5
    // Ампл > 1В  | [0 ... 5],   ампл / 2 + fabs(см) <= 5

    Value amplitude = form->FindParameter(TypeDParam::AmplitudePic)->GetValue();
    DParam *param_offset = form->FindParameter(TypeDParam::Offset);

    SMinMax result(false);
    result.max = param_offset->Max();

    if (amplitude.Abs() == 0)
    {
        result.max = param_offset->Max();
    }
    else if (amplitude.ToDouble() <= 1.0F)
    {
        result.max.Div(2);
    }

    amplitude.Div(2);

    result.max.Sub(amplitude);

    result.min = result.max;
    result.min.SetSign(-1);

    Value offset = param_offset->GetValue();

    result.valid = (offset <= result.max) && (offset >= result.min);

    return result;
}


SMinMax PAmplitudePic::InRange(Form *form)
{
    // Ампл == 0  | offset[0 ... 5]
    // Ампл <= 1В | offset[0 ... 2.5]; ampl / 2 + fabs(см) <= 2.5;
    // Ампл > 1В  | offset[0 ... 5];   ампл / 2 + fabs(см) <= 5

    SMinMax result(false);
    result.min = Value(0);

    DParam *param_ampl = form->FindParameter(TypeDParam::AmplitudePic);
    Value amplitude = param_ampl->GetValue();

    if (amplitude.Abs() == 0)
    {
        result.max = Value(10);
        result.valid = true;
    }
    else
    {
        result.max.FromUnits(5);
        if (amplitude.ToDouble() <= 1.0)
        {
            result.max.FromUnits(2, 500);
        }

        Value offset = form->FindParameter(TypeDParam::Offset)->GetValue();

        result.max.Sub(offset);
        result.max.Mul(2);

        result.valid = (amplitude <= result.max);
    }

    return result;
}
*/
