#include "stdafx.h"
#ifndef WIN32
#include "log.h"
#include "Message.h"
#include <cstdarg>
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Log::AddString(char *format, ...)
{
    char string[100];

    std::va_list args;
    va_start(args, format);
    std::vsprintf(string, format, args);
    va_end(args);

    Message *msg = new Message(Command::Log, string);

    msg->Transmit();
}
