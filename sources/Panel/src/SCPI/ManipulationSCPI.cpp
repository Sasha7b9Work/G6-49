#include "defines.h"
#include "SCPI/SCPI.h"
#include "SCPI/ManipulationSCPI.h"


// :MANIPULATION:ENABLED
static const char *FuncEnabled(const char *);
static void HintEnabled(String *);


const StructSCPI SCPI::manipulation[] =
{
    SCPI_LEAF(":ENABLED", FuncEnabled, "Enabled or disabled mode manipulation", HintEnabled),
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
