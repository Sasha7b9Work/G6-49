#include "Log.h"
#include "Display/Display.h"
#include <stdarg.h>
#include <string.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SIZE_BUFFER_LOG 200


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Log_Write(TypeTrace type, char *format, ...)
{
    char buffer[SIZE_BUFFER_LOG];
    char *pointer = buffer;

    if (type == TypeTrace_Error)
    {
        buffer[0] = 0;
        strcat(buffer, "!!! ERROR !!! ");
        while (*pointer++)
        {
        };
        ++pointer;
    }

    va_list args;
    va_start(args, format);
    vsprintf(pointer, format, args);
    va_end(args);
    Display_AddStringToConsole(buffer);
}
