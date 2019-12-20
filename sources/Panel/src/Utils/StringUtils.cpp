#include "defines.h"
#include "Log.h"
#include "Utils/StringUtils.h"
#include "Utils/Dictionary.h"
#include "Utils/Math.h"
#include "Settings/Settings.h"
#include "structs.h"
#include <cmath>
#include <cstdlib>
#include <string.h>
#include <cstdio>
#include <cctype>


#ifndef LANG
#define LANG 0
#define LANG_RU true
#endif



namespace SU
{
    /// Возвращает false, если выбор невозможен - строка кончилась.
    bool ChooseSymbols(const char **string);
    /// Возвращает false, если выбор невозможен - строка кончилась.
    bool ChooseSpaces(const char **string);

}



char *Voltage2String(float voltage, bool alwaysSign, char buffer[20])
{
    if (IsEquals(voltage, ERROR_VALUE_FLOAT))
    {
        std::strcpy(buffer, ERROR_STRING_VALUE);
        return buffer;
    }

    pString suf[2][4] =
    {
        {"\x10мкВ", "\x10мВ", "\x10В", "\x10кВ"},
        {"\x10uV",  "\x10mV", "\x10V", "\x10kV"}
    };

    static const float factor[4] = {1e6F, 1e3F, 1.0F, 1e-3F};

    int num = 0;
    float absValue = std::fabsf(voltage) + 0.5e-4F;

    if      (absValue < 1e-3F) { num = 0; }
    else if (absValue < 1.0F)  { num = 1; }
    else if (absValue < 1e3F)  { num = 2; }
    else                       { num = 3; }

    CHAR_BUF(bufferOut, 20);

    Float2String(voltage * factor[num], alwaysSign, 4, bufferOut);

    std::strcpy(buffer, bufferOut);
    std::strcat(buffer, suf[LANG][num]);
    return buffer;
}


char *Float2String(float value, bool alwaysSign, int numDigits, char bufferOut[20])
{
    if (IsEquals(value, ERROR_VALUE_FLOAT))
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
    else
    {
        // здесь ничего
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
    while (std::strlen(bufferOut) < static_cast<size_t>(numDigits + (signExist ? 2 : 1)))
    {
        std::strcat(bufferOut, "0");
    }

    return bufferOut;
}


char *Time2String(float time, bool alwaysSign, char buffer[20])
{
    if (IsEquals(time, ERROR_VALUE_FLOAT))
    {
        std::strcpy(buffer, ERROR_STRING_VALUE);
        return buffer;
    }
    
    pString suffix[2][4] =
    {
        {"нс", "мкс", "мс", "с"},
        {"ns", "us",  "ms", "s"}
    };

    static const float factor[4] = {1e9F, 1e6F, 1e3F, 1.0F};

    float absTime = std::fabsf(time);

    int num = 0;

    if      (absTime + 0.5e-10F < 1e-6F) {          }
    else if (absTime + 0.5e-7F < 1e-3F)  { num = 1; }
    else if (absTime + 0.5e-3F < 1.0F)   { num = 2; }
    else                                 { num = 3; }

    char bufferOut[20];
    std::strcpy(buffer, Float2String(time * factor[num], alwaysSign, 4, bufferOut));
    std::strcat(buffer, suffix[LANG][num]);
    return buffer;
}


char *Freq2String(float freq, bool, char bufferOut[20])
{
    bufferOut[0] = 0;
    const char *suffix = 0;
    if (IsEquals(freq, ERROR_VALUE_FLOAT))
    {
        std::strcat(bufferOut, ERROR_STRING_VALUE);
        return bufferOut;
    }
    if (freq >= 1e6F)
    {
        suffix = LANG_RU ? "МГц" : "MHz";
        freq /= 1e6F;
    }
    else if (freq >= 1e3F)
    {
        suffix = LANG_RU ? "кГц" : "kHz";
        freq /= 1e3F;
    }
    else
    {
        suffix = LANG_RU ? "Гц" : "Hz";
    }
    char buffer[20];
    std::strcat(bufferOut, Float2String(freq, false, 4, buffer));
    std::strcat(bufferOut, suffix);
    return bufferOut;
}


char *FloatFract2String(float value, bool alwaysSign, char bufferOut[20])
{
    return Float2String(value, alwaysSign, 4, bufferOut);
}


char *Phase2String(float phase, bool, char bufferOut[20])
{
    char buffer[20];
    std::sprintf(bufferOut, "%s\xa8", Float2String(phase, false, 4, buffer));
    return bufferOut;
}


char *Freq2StringAccuracy(float freq, char bufferOut[20], int numDigits)
{
    bufferOut[0] = 0;
    const char *suffix = LANG_RU ? "Гц" : "Hz";
    if (IsEquals(freq, ERROR_VALUE_FLOAT))
    {
        std::strcat(bufferOut, ERROR_STRING_VALUE);
        return bufferOut;
    }
    if (freq >= 1e6F)
    {
        suffix = LANG_RU ? "МГц" : "MHz";
        freq /= 1e6F;
    }
    else if (freq >= 1e3F)
    {
        suffix = LANG_RU ? "кГц" : "kHz";
        freq /= 1e3F;
    }
    else
    {
        // здесь ничего
    }
    char buffer[20];
    std::strcat(bufferOut, Float2String(freq, false, numDigits, buffer));
    std::strcat(bufferOut, suffix);
    return bufferOut;
}


char *Bin2String(uint8 value, char buffer[9])
{
    for (int bit = 0; bit < 8; bit++)
    {
        buffer[7 - bit] = _GET_BIT(value, bit) ? '1' : '0';
    }
    buffer[8] = '\0';
    return buffer;
}


char *Bin2String16(uint16 value, char valBuffer[19])
{
    char buffer[9];
    std::strcpy(valBuffer, Bin2String(static_cast<uint8>(value >> 8), buffer));
    valBuffer[8] = ' ';
    std::strcpy(valBuffer + 9, Bin2String(static_cast<uint8>(value), buffer));
    valBuffer[18] = '\0';
    return valBuffer;
}


char *Bin2StringN(uint value, char buffer[33], int n)
{
    buffer[n] = '\0';

    for(int i = n - 1; i >= 0; i--)
    {
        buffer[i] = (value & 0x01) ? '1' : '0';
        value >>= 1;
    }

    return buffer;
}


char *Hex8toString(uint8 value, char buffer[3])
{
    std::sprintf(buffer, "%02X", value);
    return buffer;
}


char *Hex16toString(uint16 value, char buffer[5])
{
    std::sprintf(buffer, "%04X", value);
    return buffer;
}


char *Hex32toString(uint value, char buffer[9], bool upper)
{
    std::sprintf(buffer, upper ? "%08X" : "%08x", value);
    return buffer;
}


/// @brief Преобразует value в текстовую строку
/// @attention Строка будет храниться до следующего вызова функции. Если результат нужен большее количество времени, то его нужно скопировать себе
/// \param numMinFields минимальное число занимаемых знакомест. Если для вывода числа столько не требуется, лишние заполняются нулями
static char *Int2String(int value, bool alwaysSign, int numMinFields, char *buffer)
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


String Int2String(int value, bool alwaysSign, int numMinFields)
{
    char buffer[20];
    return String(Int2String(value, alwaysSign, numMinFields, buffer));
}


char *UInt2String(uint value, char buffer[20])
{
    static char buf[20];
    
    char *retValue = buffer ? buffer : buf;

    std::sprintf(retValue, "%u", value);
    return retValue;
}


char *UInt64_2String(uint64 value, char buffer[20])
{
    static char buf[20];

    char *retValue = buffer ? buffer : buf;

    std::sprintf(retValue, "%llu", value);
    return retValue;
}


char *UInt2StringThisPoint(uint value, char bufferOut[20], int allDigits, int forFract)
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



uint StringToBin32(char buffer[33])
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


bool String2Int(char *str, int *value)
{
    int sign = str[0] == '-' ? -1 : 1;
    if (str[0] < '0' || str[0] > '9')
    {
        str++;
    }
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

    if (sign == -1)
    {
        *value *= -1;
    }
    return true;
}


bool String2UInt64(const char *str, uint64 *value)
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


bool String2UInt(const char *str, uint *value)
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


char *Time2StringAccuracy(float time, bool alwaysSign, char buffer[20], int numDigits)
{
    buffer[0] = 0;
    const char *suffix = LANG_RU ? "с" : "s";

    float fabsTime = std::fabsf(time);

    if (IsEquals(time, ERROR_VALUE_FLOAT))
    {
        std::strcat(buffer, ERROR_STRING_VALUE);
        return buffer;
    }
    else if (fabsTime + 0.5e-10F < 1e-6F)
    {
        suffix = LANG_RU ? "нс" : "ns";
        time *= 1e9F;
    }
    else if (fabsTime + 0.5e-7F < 1e-3F)
    {
        suffix = LANG_RU ? "мкс" : "us";
        time *= 1e6F;
    }
    else if (fabsTime + 0.5e-3F < 1.0F)
    {
        suffix = LANG_RU ? "мс" : "ms";
        time *= 1e3F;
    }
    else
    {
        // здесь ничего
    }

    char bufferOut[20];
    std::strcat(buffer, Float2String(time, alwaysSign, numDigits, bufferOut));
    std::strcat(buffer, suffix);

    return buffer;
}


char *Db2String(float value, int numDigits, char bufferOut[20])
{
    bufferOut[0] = 0;
    char buffer[20];
    std::strcat(bufferOut, Float2String(value, false, numDigits, buffer));
    std::strcat(bufferOut, "дБ");
    return bufferOut;
}


int BCD2Int(uint bcd)
{
    uint pow = 1;

    int value = 0;

    for (int i = 0; i < 8; i++)
    {
        value += (bcd & 0x0f) * pow;
        pow *= 10;
        bcd = bcd >> 4;
    }

    return value;
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
    value = Abs(value) / 10;
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


bool EqualsZeroStrings(char *str1, char *str2)
{
    while ((*str1) == (*str2))
    {
        if ((*str1) == '\0')
        {
            return true;
        }
        str1++;
        str2++;
    }
    return false;
}


bool EqualsStrings(const char *str1, const char *str2, int size)
{
    for (int i = 0; i < size; i++)
    {
        if (str1[i] != str2[i])
        {
            return false;
        }
    }
    return true;
}


bool EqualsStrings(const char *str1, const char *str2)
{
    uint size = std::strlen(str1);

    for (uint i = 0; i < size; i++)
    {
        if (str1[i] != str2[i])
        {
            return false;
        }
    }
    return true;
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
    if(symbol >= 0xe0) //-V547
    {
        return symbol - 0x20;
    }
    else if (symbol == 0xb8) //-V547
    {
        return symbol - 0x10;
    }
    else if (symbol <= 0x7a)
    {
        return static_cast<char>(std::toupper(symbol));
    }
    else
    {
        // здесь ничего
    }
    return symbol;
}


void SU::ConcatenateSymbol(char *str, char symbol)
{
    char con[2] = {symbol, 0};
    std::strcat(str, con);
}


char *Float2String(float value)
{
    static char result[100];
    std::sprintf(result, "%f", value);
    return result;
}


char *Buffer2FloatString(const uint8 *buffer)
{
    return Float2String(Buffer2Float(buffer));
}


float Buffer2Float(const uint8 *buffer)
{
    return BitSet32(buffer).floatValue;
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
    else
    {
        // здесь ничего
    }

    return symbol;
}
