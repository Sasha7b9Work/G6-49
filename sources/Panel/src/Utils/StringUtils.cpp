#include "defines.h"
#include "Log.h"
#include "Display/Text.h"
#include "Utils/Buffer.h"
#include "Utils/Dictionary.h"
#include "Utils/Math.h"
#include "Utils/StringUtils.h"
#include "Settings/Settings.h"
#include "structs.h"
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cctype>


#ifndef LANG
#define LANG 0
#define LANG_RU true
#endif



uint8 Locale::separator = '.';


char *SU::Float2String(float value, bool alwaysSign, int numDigits, char bufferOut[20])
{
    if (Math::IsEquals(value, ERROR_VALUE_FLOAT))
    {
        std::strcpy(bufferOut, ERROR_STRING_VALUE);
        return bufferOut;
    }

    value = Math::RoundFloat(value, numDigits);
    
    char *pBuffer = bufferOut;

    if (value < 0)
    {
        *pBuffer++ = '-';
    }
    else if (alwaysSign)
    {
        *pBuffer++ = '+';
    }

    char format[10] = "%4.2f\0\0";

    format[1] = static_cast<char>(numDigits) + 0x30;

    int numDigitsInInt = Math::DigitsInIntPart(value);

    format[3] = static_cast<char>((numDigits - numDigitsInInt) + 0x30);
    if (numDigits == numDigitsInInt)
    {
        format[5] = '.';
    }

    float absValue = std::fabsf(value);
    std::sprintf(pBuffer, format, static_cast<double>(absValue));

    float val = static_cast<float>(std::atof(pBuffer)); //-V2508

    if (Math::DigitsInIntPart(val) != numDigitsInInt)
    {
        numDigitsInInt = Math::DigitsInIntPart(val);
        format[3] = static_cast<char>((numDigits - numDigitsInInt) + 0x30);
        if (numDigits == numDigitsInInt)
        {
            format[5] = '.';
        }
        std::sprintf(pBuffer, format, static_cast<double>(value));
    }

    bool signExist = alwaysSign || value < 0;
    while (std::strlen(bufferOut) < static_cast<uint>(numDigits + (signExist ? 2 : 1)))
    {
        std::strcat(bufferOut, "0");
    }

    return bufferOut;
}


char *SU::Bin2StringN(uint value, char buffer[33], int n)
{
    buffer[n] = '\0';

    for(int i = n - 1; i >= 0; i--)
    {
        buffer[i] = (value & 0x01) ? '1' : '0';
        value >>= 1;
    }

    return buffer;
}


char *SU::Int2String(int value, bool alwaysSign, int numMinFields, char *buffer)
{
    static char statBuf[20];
    char *buf = buffer ? buffer : statBuf;

    const int SIZE = 20;
    char format[SIZE] = "%";
    std::snprintf(&(format[1]), SIZE, "0%d", numMinFields);
    std::strcat(format, "d");
    if (alwaysSign && value >= 0)
    {
        buf[0] = '+';
        std::snprintf(buf + 1, SIZE - 1, format, value);
    }
    else
    {
        std::snprintf(buf, SIZE, format, value);
    }
    return buf;
}


String SU::Int2String(int value, bool alwaysSign, int numMinFields)
{
    char buffer[20];
    return String(Int2String(value, alwaysSign, numMinFields, buffer));
}


char *SU::UInt2String(uint value, char buffer[20])
{
    static char buf[20];
    
    char *retValue = buffer ? buffer : buf;

    std::sprintf(retValue, "%u", value);
    return retValue;
}


char *SU::UInt64_2String(uint64 value, char buffer[20])
{
    static char buf[20];

    char *retValue = buffer ? buffer : buf;

    std::sprintf(retValue, "%llu", value);
    return retValue;
}


uint SU::StringToBin32(char buffer[33])
{
    uint result = 0;

    uint size = std::strlen(buffer);

    char *pointer = buffer;

    for(int i = static_cast<int>(size) - 1; i >= 0; i--)
    {
        if(*pointer++ != '0')
        {
            _SET_BIT(result, i);
        }
    }

    return result;
}


bool SU::String2UInt64(const char *str, uint64 *value)
{
    uint length = std::strlen(str);
    if(length == 0)
    {
        return false;
    }

    *value = 0;
    uint64 pow = 1;
    uint i = length;
    do 
    {
        --i;
        int val = str[i] &(~(0x30));
        if(val < 0 || val > 9)
        {
            return false;
        }
        *value += val * pow;
        pow *= 10;
    } while (i > 0);

    return true;
}


bool SU::String2UInt(const char *str, uint *value)
{
    uint length = std::strlen(str);
    if (length == 0)
    {
        return false;
    }

    *value = 0;
    int pow = 1;
    uint i = length;
    do
    {
        --i;
        int val = str[i] & (~(0x30));
        if (val < 0 || val > 9)
        {
            return false;
        }
        *value += val * pow;
        pow *= 10;
    } while (i > 0);

    return true;
}


int SU::NumWords(const char *string)
{
    ChooseSpaces(&string);

    while (true)
    {
        int numWords = 0;

        if (ChooseSymbols(&string))
        {
            numWords++;
        }
        else
        {
            return numWords;
        }
        ChooseSpaces(&string);
    }
}


char *SU::GetWord(char *string, int n, char *out, int size)
{
    if (n >= NumWords(string))
    {
        return 0;
    }

    // Находим начало искомого слова
    int currentWord = 0;
    do
    {
        if (*string != ' ')                         // Если текущий символ - не пробел, то нашли новое слово
        {
            if (currentWord == n)                   // Если текущее слово - наше
            {
                break;                              // то выходим - string указаывает на искомое слово
            }
            ++currentWord;
            while (*string != ' ' && *string != 0)     // Выбираем буквы этого слова
            {
                ++string;
            }
        }
        while (*string == ' ')
        {
            ++string;                   // Выбираем пробелы
        }
    } while (*string);

    // Находим конец искомого символа
    char *end = string;

    while (*end != ' ' && *end != 0)
    {
        ++end;                          // Ищем конец слова
    }

    int length = end - string;

    if (length + 1 > size)
    {
        return reinterpret_cast<char *>(0xffffffffU);           // Не хватит места в выходном буфере - выходим с соответствующим кодом //-V566
    }

    for (int i = 0; i < length; i++)
    {
        out[i] = string[i];
    }
    out[length] = 0;

    return out;
}


int SU::NumDigitsInNumber(int value)
{
    value = Math::Abs(value) / 10;
    int num = 1;
    while (value > 0)
    {
        num++;
        value /= 10;
    }
    return num;
}

#define  SYMBOL(x) (*(*(x)))


bool SU::ChooseSymbols(const char **string)
{
    if (SYMBOL(string) == 0x0d && SYMBOL(string + 1) == 0x0a)
    {
        return false;
    }

    while (SYMBOL(string) != ' ' && SYMBOL(string) != 0x0d && SYMBOL(string + 1) != 0x0a)
    {
        (*string)++;
    }

    return true;
}


bool SU::ChooseSpaces(const char **string)
{
    if (SYMBOL(string) == 0x0d && SYMBOL(string + 1) == 0x0a)
    {
        return false;
    }

    while (SYMBOL(string) == ' ')
    {
        (*string)++;
    }

    return true;
}

#undef SYMBOL


bool SU::GetWord(const char *string, Word *word, const int numWord)
{
    ChooseSpaces(&string);

    int currentWord = 0;

    while (true)
    {
        if (currentWord == numWord)
        {
            word->address = const_cast<char *>(string);
            ChooseSymbols(&string);
            word->numSymbols = static_cast<int8>(string - word->address);

            char *pointer = word->address;
            int numSymbols = word->numSymbols;
            for (int i = 0; i < numSymbols; i++)
            {
                *pointer = static_cast<char>(std::toupper(*pointer));
                pointer++;
            }
            return true;
        }
        if (ChooseSymbols(&string))
        {
            currentWord++;
        }
        else
        {
            return false;
        }
        ChooseSpaces(&string);
    }
}


bool SU::WordEqualZeroString(Word *word, char* string)
{
    char *ch = string;
    char *w = static_cast<char*>(word->address);

    while (*ch != 0)
    {
        if (*ch++ != *w++)
        {
            return false;
        }
    }

    return (ch - string) == word->numSymbols;
}


int SU::FindSymbol(const char *string, char symbol)
{
    int pos = 0;
    while(*string)  
    {
        if(*string == symbol)
        {
            return pos;
        }
        ++pos;
        ++string;
    }
    return -1;
}


char SU::ToUpper(char symbol)
{
    uint8 s = static_cast<uint8>(symbol);

    if(s >= 0xe0) //-V547
    {
        return s - 0x20;
    }
    else if (s == 0xb8) //-V547
    {
        return s - 0x10;
    }
    else if (s <= 0x7a)
    {
        return static_cast<char>(std::toupper(s));
    }

    return symbol;
}


void SU::ConcatenateSymbol(char *str, char symbol)
{
    char con[2] = {symbol, 0};
    std::strcat(str, con);
}


char *SU::Float2String(float value)
{
    static char result[100];
    std::sprintf(result, "%f", value);
    return result;
}


char *SU::Buffer2FloatString(const uint8 *buffer)
{
    return Float2String(Buffer2Float(buffer));
}


float SU::Buffer2Float(const uint8 *buffer)
{
    return BitSet32(buffer).floatValue;
}


bool SU::String2Int(const char *buffer, int *value, char **end)
{
    Buffer string(std::strlen(buffer) + 1);

    std::strcpy(string.DataChar(), buffer);

    *value = std::strtol(string.DataChar(), end, 10);

    if (*end == string.DataChar())
    {
        *end = const_cast<char *>(buffer);
    }
    else
    {
        *end = const_cast<char *>(buffer) + (*end - string.DataChar());
    }

    return (*end != const_cast<char *>(buffer));
}


char *SU::ToUpper(void *s, uint size)
{
    char *str = static_cast<char *>(s);

    for (uint i = 0; i < size; i++)
    {
        str[i] = static_cast<char>(std::toupper(str[i]));
    }

    return str;
}


char *SU::ToUpper(char *str)
{
    return ToUpper(str, std::strlen(str));
}


char SU::ToLower(char symbol)
{
    uint8 s = static_cast<uint8>(symbol);

    if(s >= 0x41 && s <= 0x5a)
    {
        return static_cast<char>(s + 0x20);
    }
    else if(s >= 0xc0 && s < 0xE0)
    {
        return static_cast<char>(s + 0x20);
    }

    return symbol;
}


char *SU::UInt2StringThisPoint(uint value, char bufferOut[20], int allDigits, int forFract)
{
    int allSymbols = allDigits + 1;         // Всего символов на 1 больше, чем десятичных знаков - ещё одно место занимает точка

    char *start = bufferOut;

    char *pointer = start + allSymbols; // Выводить символы будем начиная с конца

    *pointer-- = 0;                         // Пишем ноль в конец строки как символ её конца

    while(pointer >= bufferOut)
    {
        if(forFract == 0)
        {
            *pointer = '.';
            pointer--;
        }
        forFract--;

        uint digit = value % 10;             // Находим текущую выводимую цифру как остаток от деления на 10

        value /= 10;

        *pointer = static_cast<char>(digit) | 0x30;

        pointer--;
    }

    return bufferOut;
}


char *SU::FindEnd(char *buffer)
{
    while (*buffer)
    {
        buffer++;
    }

    return buffer;
}


void Locale::FindSeparator()
{
    const char * const str = "1.5";

    char *end = nullptr;

    std::strtof(str, &end);

    separator = static_cast<uint8>((end == (str + 1) ? ',' : '.'));
}
