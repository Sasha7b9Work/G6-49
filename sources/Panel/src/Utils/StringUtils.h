// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct Word
{
    char *address;
    int8    numSymbols;
    uint8   notUsed0;
    uint8   notUsed1;
    uint8   notUsed2;
};


namespace Locale
{
    void FindSeparator();
    extern uint8 separator;
};


namespace SU
{
    // Возвращает указатель на завершающий (нулевой) символ строки
    char *FindEnd(char *buffer);

    char *Float2String(float value);

    // \brief Переводит число с плавающей точкой в текстовый вид. numDigits - число цифровых знакомест
    // \attention Строка будет храниться до следующего вызова функции. Если результат нужен большее количество времени, то его нужно скопироавать себе.
    // \retval указатель на строку с числом.
    char *Float2String(float value, bool alwaysSign, int numDigits, char bufferOut[20]);

    float Buffer2Float(const uint8 *buffer);

    bool String2Int(pchar buffer, int *value, char **end);

    String Int2String(int value, bool alwaysSign, int numMinFields);

    // @brief Преобразует value в текстовую строку
    // @attention Строка будет храниться до следующего вызова функции. Если результат нужен большее количество времени, то его нужно скопировать себе
    // \param numMinFields минимальное число занимаемых знакомест. Если для вывода числа столько не требуется, лишние заполняются нулями
    char *Int2String(int value, bool alwaysSign, int numMinFields, char *buffer);

    char *UInt64_2String(uint64 value, char bufferOut[20] = 0);

    char *UInt2String(uint value,                // значение
        char bufferOut[20] = 0     // сюда записывается возвращаемое значение
    );

    // Перевести десятичное число в символьную строку длиной allDigits. После точка отстоит на forFract символов от конца
    char *UInt2StringThisPoint(uint value, char bufferOut[20], int allDigits, int forFract);

    // Преобразует двоичное число ,представленное строкой из нулей и единиц, в uint
    uint StringToBin32(char buffer[33]);

    char *Buffer2FloatString(const uint8 *buffer);

    // Возвращает число слов в строке string
    int NumWords(pchar string);

    // Возвращает указатель на n слово в строке. Если char == 0 - слова нет, если ret value == 0xffffffff - выходной буфер слишком мал
    char *GetWord(char *string, int n, char *out, int size);

    // Вычисляет число разрядов в целом типа int.
    int NumDigitsInNumber(int value);

    // Эта команда сразу преобразует к верхенму регистру слово.
    bool GetWord(pchar string, Word *word, const int numWord);

    bool WordEqualZeroString(Word *word, char *string);

    // Если символа нет, возвращает -1
    int FindSymbol(pchar string, char symbol);

    char ToUpper(char symbol);

    char *ToUpper(void *_str, uint size);

    void ConcatenateSymbol(char *str, char symbol);

    char *ToUpper(char *str);

    char ToLower(char symbol);

    bool String2UInt64(pchar str, uint64 *value);

    bool String2UInt(pchar str, uint *value);

    // Переводит числов в двоичный вид с заданный количеством разрядов
    char *Bin2StringN(uint value, char buffer[33], int n);

    String GetSubString(pchar, int first, int last);
};
