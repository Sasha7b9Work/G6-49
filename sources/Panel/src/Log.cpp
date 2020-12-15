#include "defines.h"
#include "log.h"
#include "Display/Console.h"
#include "Display/Painter.h"
#include "Display/Display.h"
#include "Hardware/CPU.h"
#include "Hardware/VCP_p.h"
#include "Settings/Settings.h"
#include <cstdarg>
#include <cstring>
#include <cstdio>



#define SIZE_BUFFER_LOG 200


void Log::Write(TypeTrace::E, char *format, ...)
{
    char buffer[SIZE_BUFFER_LOG];
    char *pointer = buffer;
    std::va_list args;
    va_start(args, format); //-V2563 //-V2567 //-V2571
    std::vsprintf(pointer, format, args);
    va_end(args);
    Console::AddString(buffer);
}


void Log::Finalize()
{
    Painter::BeginScene(Color::BACK);
    Console::Draw();
    Painter::EndScene();
}



void Log::Trace(TypeTrace::E type, const char *module, const char *func, int numLine, char *format, ...)
{
    char buffer[SIZE_BUFFER_LOG];
    char message[SIZE_BUFFER_LOG];
    std::va_list args;
    va_start(args, format); //-V2567 //-V2563 //-V2571
    std::vsprintf(buffer, format, args);
    va_end(args);
    const int SIZE = 20;
    char numBuffer[SIZE];
    std::snprintf(numBuffer, SIZE, ":%d", numLine);
    message[0] = 0;

    if (type == TypeTrace::Error)
    {
        std::strcat(message, "!!!ERROR!!! "); //-V2513
    }
    else // if (type == TypeTrace::Info)
    {
        std::strcat(message, "            "); //-V2513
    }

    std::strcat(message, module); //-V2513
    std::strcat(message, " "); //-V2513
    std::strcat(message, func); //-V2513
    std::strcat(message, numBuffer); //-V2513
    Console::AddString(message);
    Console::AddString(buffer);
}
