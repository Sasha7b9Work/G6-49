#include "common/Messages.h"
#include "log.h"
#include <cstdarg>
#include <cstdio>
#include <cstring>


void Log::AddString(char *format, ...) //-V2560
{
    char string[100];

    std::va_list args;
    va_start(args, format);
    std::vsprintf(string, format, args);
    va_end(args);

    Message::Log(string).Transmit();
}


void Log::Trace(char *file, int line, char *format, ...) //-V2560
{
    char message[200];
    std::sprintf(message, "%s:%d ", file, line);

    char text[100];
    std::va_list args;
    va_start(args, format);
    std::vsprintf(text, format, args);
    va_end(args);

    std::strcat(message, text);

    Message::Log(message).Transmit();
}
