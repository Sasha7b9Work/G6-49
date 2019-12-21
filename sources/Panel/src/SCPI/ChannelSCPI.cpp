#include "defines.h"
#include "SCPI/ChannelSCPI.h"



static const char *FuncControlA(const char *);
static bool TestControlA();



const StructSCPI SCPI::channel[] =
{
    SCPI_LEAF(":A", FuncControlA, TestControlA, "Set active channel A"),
    SCPI_LEAF(":B"), FuncControlB, 
};