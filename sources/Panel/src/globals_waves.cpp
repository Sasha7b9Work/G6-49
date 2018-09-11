#include "globals.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static Form::Parameter params_SineA[Form::Parameter::Number] =
{
    Form::Parameter::Frequency,
    Form::Parameter::Period,
    Form::Parameter::Amplitude,
    Form::Parameter::Offset,
    Form::Parameter::Modulation,
    Form::Parameter::Number
};

static Form::Parameter  params_RampPlusA[Form::Parameter::Number] =
{
    Form::Parameter::Frequency,
    Form::Parameter::Period,
    Form::Parameter::Amplitude,
    Form::Parameter::Offset,
    Form::Parameter::Number
};

static Form::Parameter params_RampMinusA[Form::Parameter::Number] =
{
    Form::Parameter::Frequency,
    Form::Parameter::Period,
    Form::Parameter::Amplitude,
    Form::Parameter::Offset,
    Form::Parameter::Number
};

static Form::Parameter params_MeanderA[Form::Parameter::Number] =
{
    Form::Parameter::Frequency,
    Form::Parameter::Period,
    Form::Parameter::Amplitude,
    Form::Parameter::Offset,
    Form::Parameter::Number
};

static Form::Parameter params_ImpulseA[Form::Parameter::Number] =
{
    Form::Parameter::Frequency,
    Form::Parameter::Period,
    Form::Parameter::Amplitude,
    Form::Parameter::Offset,
    Form::Parameter::Number
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
static Form::Parameter params_SineB[Form::Parameter::Number] =
{
    Form::Parameter::Frequency,
    Form::Parameter::Period,
    Form::Parameter::Amplitude,
    Form::Parameter::Offset,
    Form::Parameter::Modulation,
    Form::Parameter::Number
};

static Form::Parameter  params_RampPlusB[Form::Parameter::Number] =
{
    Form::Parameter::Frequency,
    Form::Parameter::Period,
    Form::Parameter::Amplitude,
    Form::Parameter::Offset,
    Form::Parameter::Number
};

static Form::Parameter params_RampMinusB[Form::Parameter::Number] =
{
    Form::Parameter::Frequency,
    Form::Parameter::Period,
    Form::Parameter::Amplitude,
    Form::Parameter::Offset,
    Form::Parameter::Number
};

static Form::Parameter params_MeanderB[Form::Parameter::Number] =
{
    Form::Parameter::Frequency,
    Form::Parameter::Period,
    Form::Parameter::Amplitude,
    Form::Parameter::Offset,
    Form::Parameter::Number
};

static Form::Parameter params_ImpulseB[Form::Parameter::Number] =
{
    Form::Parameter::Frequency,
    Form::Parameter::Period,
    Form::Parameter::Amplitude,
    Form::Parameter::Offset,
    Form::Parameter::Number
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
