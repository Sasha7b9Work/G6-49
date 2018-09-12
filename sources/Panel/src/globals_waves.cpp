#include "globals.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static Parameter params_SineA[Parameter::Number] =
{
    Parameter(Parameter::Frequency,  0.1f,    100e6f),
    Parameter(Parameter::Period,     100e-8f, 100e6f),
    Parameter(Parameter::Amplitude,  0.0f,    10.0f),
    Parameter(Parameter::Offset,     0.0f,    10.0f),
    Parameter(Parameter::Modulation, -1.0f,    -1.0f)
};

static Parameter  params_RampPlusA[Parameter::Number] =
{
    Parameter(Parameter::Frequency, 0.1f,    100e6f),
    Parameter(Parameter::Period,    3.3e-8f, 10e6f),
    Parameter(Parameter::Amplitude, 0.0f,    10.0f),
    Parameter(Parameter::Offset,    0.0f,    10.0f)
};

static Parameter params_RampMinusA[Parameter::Number] =
{
    Parameter(Parameter::Frequency, 0.1f,    100e6f),
    Parameter(Parameter::Period,    3.3e-8f, 10e6f),
    Parameter(Parameter::Amplitude, 0.0f,    10.0f),
    Parameter(Parameter::Offset,    0.0f,    10.0f)
};

static Parameter params_MeanderA[Parameter::Number] =
{
    Parameter(Parameter::Frequency, 0.1f,    100e6f),
    Parameter(Parameter::Period,    3.3e-8f, 10e6f),
    Parameter(Parameter::Amplitude, 0.0f,    10.0f),
    Parameter(Parameter::Offset,    0.0f,    10.0f)
};

static Parameter params_ImpulseA[Parameter::Number] =
{
    Parameter(Parameter::Frequency, 0.1f,    100e6f),
    Parameter(Parameter::Period,    3.3e-8f, 10e6f),
    Parameter(Parameter::Amplitude, 0.0f,    10.0f),
    Parameter(Parameter::Offset,    0.0f,    10.0f)
};


//----------------------------------------------------------------------------------------------------------------------------------------------------
static Form formsA[Form::Number] =
{
    Form(Form::Sine,      params_SineA),
    Form(Form::RampPlus,  params_RampPlusA),
    Form(Form::RampMinus, params_RampMinusA),
    Form(Form::Meander,   params_MeanderA),
    Form(Form::Impulse,   params_ImpulseA)
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static Parameter params_SineB[Parameter::Number] =
{
    Parameter(Parameter::Frequency,  0.1f,    100e6f),
    Parameter(Parameter::Period,     100e-8f, 100e6f),
    Parameter(Parameter::Amplitude,  0.0f,    10.0f),
    Parameter(Parameter::Offset,     0.0f,    10.0f),
    Parameter(Parameter::Phase,      0.0f,    360.0f),
    Parameter(Parameter::Modulation, -1.0f,   -1.0f)
};

static Parameter  params_RampPlusB[Parameter::Number] =
{
    Parameter(Parameter::Frequency, 0.1f,    100e6f),
    Parameter(Parameter::Period,    3.3e-8f, 10e6f),
    Parameter(Parameter::Amplitude, 0.0f,    10.0f),
    Parameter(Parameter::Offset,    0.0f,    10.0f)
};

static Parameter params_RampMinusB[Parameter::Number] =
{
    Parameter(Parameter::Frequency, 0.1f,    100e6f),
    Parameter(Parameter::Period,    3.3e-8f, 10e6f),
    Parameter(Parameter::Amplitude, 0.0f,    10.0f),
    Parameter(Parameter::Offset,    0.0f,    10.0f)
};

static Parameter params_MeanderB[Parameter::Number] =
{
    Parameter(Parameter::Frequency, 0.1f,    100e6f),
    Parameter(Parameter::Period,    3.3e-8f, 10e6f),
    Parameter(Parameter::Amplitude, 0.0f,    10.0f),
    Parameter(Parameter::Offset,    0.0f,    10.0f)
};

static Parameter params_ImpulseB[Parameter::Number] =
{
    Parameter(Parameter::Frequency, 0.1f,    100e6f),
    Parameter(Parameter::Period,    3.3e-8f, 10e6f),
    Parameter(Parameter::Amplitude, 0.0f,    10.0f),
    Parameter(Parameter::Offset,    0.0f,    10.0f)
};


//----------------------------------------------------------------------------------------------------------------------------------------------------
static Form formsB[Form::Number] =
{
    Form(Form::Sine,        params_SineB),
    Form(Form::RampPlus,    params_RampPlusB),
    Form(Form::RampMinus,   params_RampMinusB),
    Form(Form::Meander,     params_MeanderB),
    Form(Form::Impulse,     params_ImpulseB)
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Wave wave[Chan::Number] =
{
    Wave(Chan::A, formsA),
    Wave(Chan::B, formsB)
};
