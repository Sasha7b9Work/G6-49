#include "stdafx.h"
#ifndef WIN32
#include "log.h"
#include "Message.h"
#include "Interface/InterfaceDevice.h"
#include <stdarg.h>
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Log::AddString(char *format, ...)
{
    char string[100];

    va_list args;
    va_start(args, format);
    vsprintf(string, format, args);
    va_end(args);

    Message *msg = new Message(std::strlen(string) + 1 + 1, Command::Log);
    std::strcpy((char *)msg->Data() + 1, string);
    Interface::AddMessageInQueue(msg);
}
