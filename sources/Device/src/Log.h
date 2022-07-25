// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Command.h"


#define LOG_TRACE(...)      ::Log::Trace(__FILE__, __LINE__, __VA_ARGS__)
#define LOG_WRITE(...)      ::Log::AddString(__VA_ARGS__)
#define LOG_ERROR(...)      ::Log::AddString(__VA_ARGS__)
#define LOG_FUNC_ENTER()    ::Log::AddString("%s %d enter", __FUNCTION__, __LINE__)
#define LOG_ERROR_FINALIZE(...)


namespace Log
{
    void Trace(char *file, int line, char *format, ...);
    void AddString(char *format, ...);
};
