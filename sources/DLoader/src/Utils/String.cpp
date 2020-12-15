#include "defines.h"
#include "String.h"
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cstdarg>


String::String(const String &rhs) : buffer(0)
{
    if (Allocate(std::strlen(rhs.CString()) + 1)) //-V2513
    {
        std::strcpy(buffer, rhs.CString()); //-V2513
    }
}


String::String(char symbol) : buffer(0)
{
    if (Allocate(2))
    {
        buffer[0] = symbol; //-V2563
        buffer[1] = 0; //-V2563
    }
}


String::String(const char *format, ...)
{
#define SIZE 100
    char buf[SIZE + 1];

    std::va_list args;
    va_start(args, format); //-V2567
    std::vsprintf(buf, format, args);
    va_end(args);

    if (Allocate(std::strlen(buf) + 1)) //-V2513
    {
        std::strcpy(buffer, buf); //-V2513
    }
}


String::~String()
{
    Free();
}


void String::Free()
{
    std::free(buffer); //-V2511
    buffer = 0;
}


char *String::CString() const
{
    return buffer;
}


bool String::Allocate(uint size) //-V2506
{
    buffer = static_cast<char *>(std::malloc(size)); //-V2511
    if (buffer)
    {
        return true;
    }

    return false;
}
