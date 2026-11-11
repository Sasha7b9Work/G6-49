// 2026/11/10 08:01:30 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once


#define LOG_WRITE_TRACE(...)        Log::Write(__FILE__, __LINE__, __VA_ARGS__)


namespace Log
{
    void Write(pchar file, int line, pchar format, ...);
}
