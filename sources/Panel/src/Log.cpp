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
volatile static bool loggerUSB = false;


#define SIZE_BUFFER_LOG 200

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef OPEN
void Log::Write(TypeTrace::E, char *, ...)
{
}

void Log::Trace(TypeTrace::E, const char *, const char *, int, char *, ...)
{
}

#endif


#ifndef OPEN

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Log::Write(TypeTrace::E, char *format, ...)
{
    char buffer[SIZE_BUFFER_LOG];
    char *pointer = buffer;

    /*
    if (type == TypeTrace::Error)
    {
        buffer[0] = 0;
        std::strcat(buffer, "!!! ERROR !!! 1111111111111111111111111");
        while (*pointer++) {};
        ++pointer;
    }
    */
    std::va_list args;
    va_start(args, format);
    std::vsprintf(pointer, format, args);
    va_end(args);
    Console::AddString(buffer);
    if(loggerUSB)
    {
        VCP::SendFormatStringAsynch(buffer);
    }
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
    snprintf(numBuffer, 100, ":%d", numLine);
    message[0] = 0;
    if (type == TypeTrace::Error)
    {
        std::strcat(message, "!!!ERROR!!! ");
    }
    else if (type == TypeTrace::Info)
    {
        std::strcat(message, "            ");
    }
    std::strcat(message, module);
    std::strcat(message, " ");
    std::strcat(message, func);
    std::strcat(message, numBuffer);
    Console::AddString(message);
    Console::AddString(buffer);
    if(loggerUSB)
    {
        VCP::SendFormatStringAsynch(message);
        VCP::SendFormatStringAsynch(buffer);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Log::DisconnectLoggerUSB()
{
    //static uint8 data = 20;
    //Log_Write("посылаю %d", data);
    //VCP_SendData(&data, 1);
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Log::EnableLoggerUSB(bool enable)
{
    loggerUSB = enable;
}
#endif
