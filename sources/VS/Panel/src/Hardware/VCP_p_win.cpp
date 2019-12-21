#include "defines.h"
#include "GUI/ConsoleSCPI.h"
#include "Hardware/VCP_p.h"
#include "Utils/String.h"


void PVCP::SendStringAsynch(const char *data)
{
    String text(">>> %s", data);
    ConsoleSCPI::Self()->AddText(text.c_str());
}
