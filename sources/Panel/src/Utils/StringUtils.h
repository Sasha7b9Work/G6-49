#pragma once
#include "Utils/String.h"


struct Word
{
    char   *address;
    int8    numSymbols;
    uint8   notUsed0;
    uint8   notUsed1;
    uint8   notUsed2;
};

/// \brief Переводит число с плавающей точкой в текстовый вид. numDigits - число цифровых знакомест
/// \attention Строка будет храниться до следующего вызова функции. Если результат нужен большее количество времени, то его нужно скопироавать себе.
/// \retval указатель на строку с числом.
char *Float2String(float value, bool alwaysSign, int numDigits, char bufferOut[20]);

float Buffer2Float(const uint8 *buffer);

String Int2String(int value, bool alwaysSign, int numMinFields);

char* UInt64_2String(uint64 value, char bufferOut[20] = 0);

char* UInt2String(uint value,                     ///< значение
                 char bufferOut[20] = 0         ///< сюда записывается возвращаемое значение
);
/// Перевести десятичное число в символьную строку длиной allDigits. После точка отстоит на forFract символов от конца
char* UInt2StringThisPoint(uint value, char bufferOut[20], int allDigits, int forFract);
/// Преобразует двоичное число ,представленное строкой из нулей и единиц, в uint
uint StringToBin32(char buffer[33]);


struct SU
{
    static char *Buffer2FloatString(const uint8 *buffer);
    /// Возвращает число слов в строке string
    static int NumWords(const char *string);
    /// Возвращает указатель на n слово в строке. Если char == 0 - слова нет, если ret value == 0xffffffff - выходной буфер слишком мал
    static char *GetWord(char *string, int n, char *out, int size);
    /// Вычисляет число разрядов в целом типа int.
    static int NumDigitsInNumber(int value);
    /// Эта команда сразу преобразует к верхенму регистру слово.
    static bool GetWord(const char *string, Word *word, const int numWord);

    static bool WordEqualZeroString(Word *word, char* string);

    static int FindSymbol(const char *string, char symbol);

    static char ToUpper(char symbol);

    static char *ToUpper(void *_str, uint size);

    static void ConcatenateSymbol(char *str, char symbol);

    static char *ToUpper(char *str);

    static char ToLower(char symbol);
private:
    /// Возвращает false, если выбор невозможен - строка кончилась.
    static bool ChooseSymbols(const char **string);
    /// Возвращает false, если выбор невозможен - строка кончилась.
    static bool ChooseSpaces(const char **string);
};

bool String2UInt64(const char *str, uint64 *value);

bool String2UInt(const char *str, uint *value);
/// Переводит числов в двоичный вид с заданный количеством разрядов
char *Bin2StringN(uint value, char buffer[33], int n);
