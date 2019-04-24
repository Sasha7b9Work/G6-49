#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "log.h"
#include "Display/Console.h"
#include "Display/Painter.h"
#include <Display/Display.h>
#include <Hardware/CPU.h>
#include "Hardware/VCP.h"
#include "Settings/Settings.h"
#include <cstdarg>
#include <cstring>
#include <cstdio>
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SIZE_BUFFER_LOG 200

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Log::Write(TypeTrace::E, char *format, ...)
{
    char buffer[SIZE_BUFFER_LOG];
    char *pointer = buffer;
    std::va_list args;
    va_start(args, format);
    std::vsprintf(pointer, format, args);
    va_end(args);
    Console::AddString(buffer);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Log::Finalize()
{
    Painter::BeginScene(Color::BACK);
    Console::Draw();
    Painter::EndScene();
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Log::Trace(TypeTrace::E type, const char *module, const char *func, int numLine, char *format, ...)
{
    char buffer[SIZE_BUFFER_LOG];
    char message[SIZE_BUFFER_LOG];
    std::va_list args;
    va_start(args, format);
    std::vsprintf(buffer, format, args);
    va_end(args);
    const int SIZE = 20;
    char numBuffer[SIZE];
    snprintf(numBuffer, SIZE, ":%d", numLine);
    message[0] = 0;

    if (type == TypeTrace::Error)
    {
        std::strcat(message, "!!!ERROR!!! ");
    }
    else // if (type == TypeTrace::Info)
    {
        std::strcat(message, "            ");
    }

    std::strcat(message, module);
    std::strcat(message, " ");
    std::strcat(message, func);
    std::strcat(message, numBuffer);
    Console::AddString(message);
    Console::AddString(buffer);
}
