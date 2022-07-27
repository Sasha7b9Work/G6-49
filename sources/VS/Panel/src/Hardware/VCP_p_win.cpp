#include "defines.h"
#include "Display/Text.h"
#include "GUI/ConsoleSCPI.h"
#include "Hardware/VCP_p.h"


void PVCP::SendStringAsynch(pchar data)
{
    String text(">>> %s", data);
    ConsoleSCPI::Self()->AddText(text.c_str());
}
