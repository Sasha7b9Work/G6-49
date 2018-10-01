#include "Signals.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static ParameterValue paramsSineModulationA[] =
{
    ParameterValue(ParameterValue::Manipulation,         -1.0f, 1.0f,   "",     0, Order::One),
    ParameterValue(ParameterValue::ManipulationDuration, 0.0f,  10e3f,  "5000", 0, Order::Milli),
    ParameterValue(ParameterValue::ManipulationPeriod,   0.0f,  10e3f,  "2500", 1, Order::Milli),
    ParameterValue(ParameterValue::Exit,                 -1.0f, 1.0f,   "",     0, Order::One)
};

static ParameterValue params_SineA[] =
{
    ParameterValue(ParameterValue::Frequency,          0.1f,    100e6f, "10000", 0, Order::Kilo),
    ParameterValue(ParameterValue::Amplitude,          0.0f,    10.0f,  "50000", 0, Order::One),
    ParameterValue(ParameterValue::Offset,             0.0f,    10.0f,  "50000", 0, Order::One),
    ParameterValue(ParameterValue::Manipulation,       -1.0f,    -1.0f, "",      0, Order::One, paramsSineModulationA, 4)
};

static ParameterValue  params_RampPlusA[] =
{
    ParameterValue(ParameterValue::Frequency, 0.1f,    100e6f, "10000",  0, Order::Kilo),
    ParameterValue(ParameterValue::Amplitude, 0.0f,    10.0f,  "10000",  1, Order::One),
    ParameterValue(ParameterValue::Offset,    0.0f,    10.0f,  "50000",  0, Order::One),
};

static ParameterValue params_RampMinusA[] =
{
    ParameterValue(ParameterValue::Frequency, 0.1f,    100e6f, "10000", 0, Order::Kilo),
    ParameterValue(ParameterValue::Amplitude, 0.0f,    10.0f,  "10000", 1, Order::One),
    ParameterValue(ParameterValue::Offset,    0.0f,    10.0f,  "50000", 0, Order::One)
};

static ParameterValue params_MeanderA[] =
{
    ParameterValue(ParameterValue::Frequency, 0.1f,    100e6f,  "10000", 0, Order::Kilo),
    ParameterValue(ParameterValue::Amplitude, 0.0f,    10.0f,   "10000", 1, Order::One),
    ParameterValue(ParameterValue::Offset,    0.0f,    10.0f,   "50000", 0, Order::One)
};

static ParameterValue params_ImpulseA[] =
{
    ParameterValue(ParameterValue::Period,    3.3e-8f, 10e6f,  "10000", 0, Order::Milli),
    ParameterValue(ParameterValue::Duration,  0.0f,    100e6f, "01000", 0, Order::Milli),
    ParameterValue(ParameterValue::Amplitude, 0.0f,    10.0f,  "10000", 1, Order::One),
    ParameterValue(ParameterValue::Offset,    0.0f,    10.0f,  "50000", 0, Order::One)
};

static ParameterValue params_PacketA[] =
{
    ParameterValue(ParameterValue::Period,        0.0f,   100e6f, "02000", 0, Order::Milli),
    ParameterValue(ParameterValue::Duration,      0.0f,   100e6f, "01000", 0, Order::Milli),
    ParameterValue(ParameterValue::PacketNumber,  0.0f,   100e6f, "30000", 0, Order::One),
    ParameterValue(ParameterValue::PacketPeriod,  0.0f,   100e6f, "10000", 0, Order::Milli),
    ParameterValue(ParameterValue::Amplitude,     0.0f,   10.0f,  "10000", 1, Order::One),
    ParameterValue(ParameterValue::Offset,        0.0f,   10.0f,  "50000", 0, Order::One),

};

//----------------------------------------------------------------------------------------------------------------------------------------------------
extern Wave wave[Chan::Number];

static Form formsA[] =
{
    Form(Form::Sine,            params_SineA,      4, &wave[Chan::A]),
    Form(Form::RampPlus,        params_RampPlusA,  3, &wave[Chan::A]),
    Form(Form::RampMinus,       params_RampMinusA, 3, &wave[Chan::A]),
    Form(Form::Meander,         params_MeanderA,   3, &wave[Chan::A]),
    Form(Form::Impulse,         params_ImpulseA,   4, &wave[Chan::A]),
    Form(Form::PacketImpuls,    params_PacketA,    6, &wave[Chan::A])
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static ParameterValue paramsSineModulationB[] =
{
    ParameterValue(ParameterValue::Manipulation,         -1.0f, 1.0f,   "",     0, Order::One),
    ParameterValue(ParameterValue::ManipulationDuration, 0.0f,  10e3f,  "5000", 0, Order::Milli),
    ParameterValue(ParameterValue::ManipulationPeriod,   0.0f,  10e3f,  "2500", 1, Order::Milli),
    ParameterValue(ParameterValue::Exit,                 -1.0f, 1.0f,   "",     0, Order::One)
};

static ParameterValue params_SineB[] =
{
    ParameterValue(ParameterValue::Frequency,          0.1f,    100e6f, "10000", 0, Order::Kilo),
    ParameterValue(ParameterValue::Amplitude,          0.0f,    10.0f,  "10000", 1, Order::One),
    ParameterValue(ParameterValue::Offset,             0.0f,    10.0f,  "50000", 0, Order::One),
    ParameterValue(ParameterValue::Phase,              0.0f,    360.0f, "00000", 0, Order::One),
    ParameterValue(ParameterValue::Manipulation,       -1.0f,   -1.0f,  "10000", 0, Order::One, paramsSineModulationB, 4)
};

static ParameterValue  params_RampPlusB[] =
{
    ParameterValue(ParameterValue::Frequency, 0.1f,    100e6f, "10000", 0, Order::Kilo),
    ParameterValue(ParameterValue::Amplitude, 0.0f,    10.0f,  "50000", 0, Order::One),
    ParameterValue(ParameterValue::Offset,    0.0f,    10.0f,  "50000", 0, Order::One)
};

static ParameterValue params_RampMinusB[] =
{
    ParameterValue(ParameterValue::Frequency, 0.1f,    100e6f, "10000", 0, Order::Kilo),
    ParameterValue(ParameterValue::Amplitude, 0.0f,    10.0f,  "50000", 0, Order::One),
    ParameterValue(ParameterValue::Offset,    0.0f,    10.0f,  "50000", 0, Order::One)
};

static ParameterValue params_MeanderB[] =
{
    ParameterValue(ParameterValue::Frequency, 0.1f,    100e6f, "10000", 0, Order::Kilo),
    ParameterValue(ParameterValue::Amplitude, 0.0f,    10.0f,  "10000", 1, Order::One),
    ParameterValue(ParameterValue::Offset,    0.0f,    10.0f,  "50000", 0, Order::One)
};

static ParameterValue params_ImpulseB[] =
{
    ParameterValue(ParameterValue::Period,    3.3e-8f, 10e6f,  "10000", 0, Order::Milli),
    ParameterValue(ParameterValue::Duration,  0.0f,    100e6f, "01000", 0, Order::Milli),
    ParameterValue(ParameterValue::Amplitude, 0.0f,    10.0f,  "10000", 1, Order::One),
    ParameterValue(ParameterValue::Offset,    0.0f,    10.0f,  "50000", 0, Order::One),
};


//----------------------------------------------------------------------------------------------------------------------------------------------------
extern Wave wave[Chan::Number];

static Form formsB[] =
{
    Form(Form::Sine,         params_SineB,      5, &wave[Chan::B]),
    Form(Form::RampPlus,     params_RampPlusB,  3, &wave[Chan::B]),
    Form(Form::RampMinus,    params_RampMinusB, 3, &wave[Chan::B]),
    Form(Form::Meander,      params_MeanderB,   3, &wave[Chan::B]),
    Form(Form::Impulse,      params_ImpulseB,   4, &wave[Chan::B])
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Wave wave[Chan::Number] =
{
    Wave(Chan::A, formsA, 6),
    Wave(Chan::B, formsB, 5)
};
