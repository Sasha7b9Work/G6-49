#include "log.h"
#include "common/Message.h"
#include <cstdarg>
#include <cstdio>


void Log::AddString(char *format, ...) //-V2560
{
    char string[100];

    std::va_list args;
    va_start(args, format);
    std::vsprintf(string, format, args);
    va_end(args);

    Message::Log(string).Transmit();
}
