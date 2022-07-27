// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "SCPI/SCPI.h"
#include "SCPI/ManipulationSCPI.h"


// :MANIPULATION:DURATION
static pchar FuncMDuration(pchar);
static void HintMDuration(String *);

// :MANIPULATION:ENABLED
static pchar FuncEnabled(pchar);
static void HintEnabled(String *);

// :MANIPULATION:PERIOD
static pchar FuncMPeriod(pchar);
static void HintMPeriod(String *);


const StructSCPI SCPI::manipulation[] =
{
    SCPI_LEAF(":DURATION", FuncMDuration, "Set duration of manipulation wave",    HintMDuration),
    SCPI_LEAF(":ENABLED",  FuncEnabled,  "Enabled or disabled mode manipulation", HintEnabled),
    SCPI_LEAF(":PERIOD",   FuncMPeriod,   "Set period of manipulation wave",      HintMPeriod),
    SCPI_EMPTY()
};


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static pchar const enabledNames[] =
{
    " OFF",
    " ON",
    ""
};

static pchar FuncEnabled(pchar buffer)
{
    return SCPI::ProcessParameterChoice(buffer, ParameterChoiceType::ManipulationEnabled, enabledNames);
}

static void HintEnabled(String *message)
{
    SCPI::ProcessHint(message, enabledNames);
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static pchar FuncMDuration(pchar buffer)
{
    return SCPI::ProcessParameterDouble(buffer, ParameterDoubleType::ManipulationDuration);
}

static void HintMDuration(String *)
{
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static pchar FuncMPeriod(pchar buffer)
{
    return SCPI::ProcessParameterDouble(buffer, ParameterDoubleType::ManipulationPeriod);
}


static void HintMPeriod(String *)
{

}
