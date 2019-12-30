#include "defines.h"
#include "SCPI/SCPI.h"
#include "SCPI/ManipulationSCPI.h"


// :MANIPULATION:DURATION
static const char *FuncDuration(const char *);
static void HintDuration(String *);

// :MANIPULATION:ENABLED
static const char *FuncEnabled(const char *);
static void HintEnabled(String *);

// :MANIPULATION:PERIOD
static const char *FuncPeriod(const char *);
static void HintPeriod(String *);


const StructSCPI SCPI::manipulation[] =
{
    SCPI_LEAF(":DURATION", FuncDuration, "Set duration of manipulation wave",     HintDuration),
    SCPI_LEAF(":ENABLED",  FuncEnabled,  "Enabled or disabled mode manipulation", HintEnabled),
    SCPI_LEAF(":PERIOD",   FuncPeriod,   "Set period of manipulation wave",       HintPeriod),
    SCPI_EMPTY()
};


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static const char *const enabledNames[] =
{
    " OFF",
    " ON",
    ""
};

static const char *FuncEnabled(const char *buffer)
{
    return SCPI::ProcessParameterChoice(buffer, ParameterChoice::ManipulationEnabled, enabledNames);
}

static void HintEnabled(String *message)
{
    SCPI::ProcessHint(message, enabledNames);
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static const char *FuncDuration(const char *buffer)
{
    return SCPI::ProcessParameterValue(buffer, ParameterValue::ManipulationDuration);
}

static void HintDuration(String *)
{
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static const char *FuncPeriod(const char *buffer)
{
    return SCPI::ProcessParameterValue(buffer, ParameterValue::ManipulationPeriod);
}


static void HintPeriod(String *)
{

}
