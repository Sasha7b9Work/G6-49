// 2026/11/10 08:55:16 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "common/Log_d.h"
#include "Utils/StringUtils.h"
#include "Hardware/VCP_d.h"
#include <cstdarg>
#include <cstdio>
#include <cstring>


namespace Log
{
    static int counter = 0;

    static void WriteLine(char *);
}



void Log::Write(pchar file, int line, pchar format, ...) //-V2560
{
    char message[1024];

    std::va_list args;
    va_start(args, format);
    std::vsprintf(message, format, args);
    va_end(args);

    char full_message[2048];

    std::sprintf(full_message, ":LOG:Log%4d : %s:%3d : - %s", counter++, SU::LeaveTheLastOnes(file, 27), line, message);

    WriteLine(full_message);
}


void Log::WriteLine(char *line)
{
    std::strcat(line, "\x0D");
    DVCP::SendString(line);
}
