#pragma once


#define LOG_WRITE(...) Log_Write(TypeTrace_Info, __VA_ARGS__)

typedef enum
{
    TypeTrace_Info,
    TypeTrace_Error
} TypeTrace;


void Log_Write(TypeTrace type, char *format, ...);
