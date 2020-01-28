#include "defines.h"
#include "CommonFunctions.h"
#include "log.h"
#include <cstdio>
#include <cstring>


template<class T> void LoggingArray(T *data, char *format, int num)
{
    char message[200] = {0};

    for (int i = 0; i < num; i++)
    {
        char buffer[50];
			  std::snprintf(buffer, 49, format, data[i]);
        std::strcat(message, buffer);
    }
}


void LoggingFloatArray(float *data, int num)
{
    LoggingArray<float>(data, "%4.1f ", num);
}


void LoggingUint8Array(uint8 *data, int num)
{
    LoggingArray<uint8>(data, "%d ", num);
}


void EmptyFuncVV()
{
}


void EmptyFuncVpV(void *)
{
}


void EmptyFuncVII(int, int)
{
}


void EmptyFuncVI(int)
{
}


bool EmptyFuncBV()
{
    return true;
}


bool EmptyFuncBKey(const Key&)
{
    return false;
}


void EmptyFuncVB(bool)
{
}


void EmptyFuncpVII(void *, int, int)
{
}

