#include "globals.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static Wave::Form::Parameter params_SineA[Wave::Form::Parameter::Number] =
{
    Wave::Form::Parameter::Frequency,
    Wave::Form::Parameter::Period,
    Wave::Form::Parameter::Amplitude,
    Wave::Form::Parameter::Offset,
    Wave::Form::Parameter::Modulation,
    Wave::Form::Parameter::Number
};

static Wave::Form::Parameter  params_RampPlusA[Wave::Form::Parameter::Number] =
{
    Wave::Form::Parameter::Frequency,
    Wave::Form::Parameter::Period,
    Wave::Form::Parameter::Amplitude,
    Wave::Form::Parameter::Offset,
    Wave::Form::Parameter::Number
};

static Wave::Form::Parameter params_RampMinusA[Wave::Form::Parameter::Number] =
{
    Wave::Form::Parameter::Frequency,
    Wave::Form::Parameter::Period,
    Wave::Form::Parameter::Amplitude,
    Wave::Form::Parameter::Offset,
    Wave::Form::Parameter::Number
};

static Wave::Form::Parameter params_MeanderA[Wave::Form::Parameter::Number] =
{
    Wave::Form::Parameter::Frequency,
    Wave::Form::Parameter::Period,
    Wave::Form::Parameter::Amplitude,
    Wave::Form::Parameter::Offset,
    Wave::Form::Parameter::Number
};

static Wave::Form::Parameter params_ImpulseA[Wave::Form::Parameter::Number] =
{
    Wave::Form::Parameter::Frequency,
    Wave::Form::Parameter::Period,
    Wave::Form::Parameter::Amplitude,
    Wave::Form::Parameter::Offset,
    Wave::Form::Parameter::Number
};


//----------------------------------------------------------------------------------------------------------------------------------------------------
static Wave::Form formsA[Wave::Form::Number] =
{
    Wave::Form(Wave::Form::Sine,      params_SineA),
    Wave::Form(Wave::Form::RampPlus,  params_RampPlusA),
    Wave::Form(Wave::Form::RampMinus, params_RampMinusA),
    Wave::Form(Wave::Form::Meander,   params_MeanderA),
    Wave::Form(Wave::Form::Impulse,   params_ImpulseA)
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static Wave::Form::Parameter params_SineB[Wave::Form::Parameter::Number] =
{
    Wave::Form::Parameter::Frequency,
    Wave::Form::Parameter::Period,
    Wave::Form::Parameter::Amplitude,
    Wave::Form::Parameter::Offset,
    Wave::Form::Parameter::Modulation,
    Wave::Form::Parameter::Number
};

static Wave::Form::Parameter  params_RampPlusB[Wave::Form::Parameter::Number] =
{
    Wave::Form::Parameter::Frequency,
    Wave::Form::Parameter::Period,
    Wave::Form::Parameter::Amplitude,
    Wave::Form::Parameter::Offset,
    Wave::Form::Parameter::Number
};

static Wave::Form::Parameter params_RampMinusB[Wave::Form::Parameter::Number] =
{
    Wave::Form::Parameter::Frequency,
    Wave::Form::Parameter::Period,
    Wave::Form::Parameter::Amplitude,
    Wave::Form::Parameter::Offset,
    Wave::Form::Parameter::Number
};

static Wave::Form::Parameter params_MeanderB[Wave::Form::Parameter::Number] =
{
    Wave::Form::Parameter::Frequency,
    Wave::Form::Parameter::Period,
    Wave::Form::Parameter::Amplitude,
    Wave::Form::Parameter::Offset,
    Wave::Form::Parameter::Number
};

static Wave::Form::Parameter params_ImpulseB[Wave::Form::Parameter::Number] =
{
    Wave::Form::Parameter::Frequency,
    Wave::Form::Parameter::Period,
    Wave::Form::Parameter::Amplitude,
    Wave::Form::Parameter::Offset,
    Wave::Form::Parameter::Number
};


static Wave::Form formsB[Wave::Form::Number] =
{
    Wave::Form(Wave::Form::Sine,        params_SineB),
    Wave::Form(Wave::Form::RampPlus,    params_RampPlusB),
    Wave::Form(Wave::Form::RampMinus,   params_RampMinusB),
    Wave::Form(Wave::Form::Meander,     params_MeanderB),
    Wave::Form(Wave::Form::Impulse,     params_ImpulseB)
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Wave wave[Chan::Number] =
{
    Wave(Chan::A, formsA),
    Wave(Chan::B, formsB)
};
