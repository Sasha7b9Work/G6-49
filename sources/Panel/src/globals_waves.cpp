#include "globals.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static Parameter params_SineA[Parameter::Number] =
{
    Parameter::Frequency,
    Parameter::Period,
    Parameter::Amplitude,
    Parameter::Offset,
    Parameter::Modulation,
    Parameter::Number
};

static Parameter  params_RampPlusA[Parameter::Number] =
{
    Parameter::Frequency,
    Parameter::Period,
    Parameter::Amplitude,
    Parameter::Offset,
    Parameter::Number
};

static Parameter params_RampMinusA[Parameter::Number] =
{
    Parameter::Frequency,
    Parameter::Period,
    Parameter::Amplitude,
    Parameter::Offset,
    Parameter::Number
};

static Parameter params_MeanderA[Parameter::Number] =
{
    Parameter::Frequency,
    Parameter::Period,
    Parameter::Amplitude,
    Parameter::Offset,
    Parameter::Number
};

static Parameter params_ImpulseA[Parameter::Number] =
{
    Parameter::Frequency,
    Parameter::Period,
    Parameter::Amplitude,
    Parameter::Offset,
    Parameter::Number
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
    Parameter::Frequency,
    Parameter::Period,
    Parameter::Amplitude,
    Parameter::Offset,
    Parameter::Modulation,
    Parameter::Number
};

static Parameter  params_RampPlusB[Parameter::Number] =
{
    Parameter::Frequency,
    Parameter::Period,
    Parameter::Amplitude,
    Parameter::Offset,
    Parameter::Number
};

static Parameter params_RampMinusB[Parameter::Number] =
{
    Parameter::Frequency,
    Parameter::Period,
    Parameter::Amplitude,
    Parameter::Offset,
    Parameter::Number
};

static Parameter params_MeanderB[Parameter::Number] =
{
    Parameter::Frequency,
    Parameter::Period,
    Parameter::Amplitude,
    Parameter::Offset,
    Parameter::Number
};

static Parameter params_ImpulseB[Parameter::Number] =
{
    Parameter::Frequency,
    Parameter::Period,
    Parameter::Amplitude,
    Parameter::Offset,
    Parameter::Number
};


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
