#include "globals.h"

//----------------------------------------------------------------------------------------------------------------------------------------------------
static Wave::Form formsA[Wave::Form::Number] =
{
    Wave::Form::Sine,
    Wave::Form::RampPlus,
    Wave::Form::RampMinus,
    Wave::Form::Meander,
    Wave::Form::Impulse
};


//----------------------------------------------------------------------------------------------------------------------------------------------------
static Wave::Form formsB[Wave::Form::Number] =
{
    Wave::Form::Sine,
    Wave::Form::RampPlus,
    Wave::Form::RampMinus,
    Wave::Form::Meander,
    Wave::Form::Impulse
};


//----------------------------------------------------------------------------------------------------------------------------------------------------
Wave wave[Chan::Number] =
{
    Wave(Chan::A, formsA),
    Wave(Chan::B, formsB)
};
