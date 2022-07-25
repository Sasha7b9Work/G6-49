// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "String.h"
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cstdarg>


String::String(const String &rhs) : buffer(0)
{
    if (Allocate(std::strlen(rhs.CString()) + 1))
    {
        std::strcpy(buffer, rhs.CString());
    }
}


String::String(char symbol) : buffer(0)
{
    if (Allocate(2))
    {
        buffer[0] = symbol;
        buffer[1] = 0;
    }
}


String::String(const char *format, ...)
{
#define SIZE 100
    char buf[SIZE + 1];

    std::va_list args;
    va_start(args, format);
    std::vsprintf(buf, format, args);
    va_end(args);

    if (Allocate(std::strlen(buf) + 1))
    {
        std::strcpy(buffer, buf);
    }
}


String::~String()
{
    Free();
}


void String::Free()
{
    std::free(buffer);
    buffer = 0;
}


char *String::CString() const
{
    return buffer;
}


bool String::Allocate(uint size)
{
    buffer = static_cast<char *>(std::malloc(size));
    if (buffer)
    {
        return true;
    }

    return false;
}
