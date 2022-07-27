// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct Word
{
    char   *address;
    int8    numSymbols;
    uint8   notUsed0;
    uint8   notUsed1;
    uint8   notUsed2;
};


struct Locale
{
    static void FindSeparator();
    static uint8 separator;
};


struct SU
{
    // Возвращает указатель на завершающий (нулевой) символ строки
    static char *FindEnd(char *buffer);

    static char *Float2String(float value);
    
    // \brief Переводит число с плавающей точкой в текстовый вид. numDigits - число цифровых знакомест
    // \attention Строка будет храниться до следующего вызова функции. Если результат нужен большее количество времени, то его нужно скопироавать себе.
    // \retval указатель на строку с числом.
    static char *Float2String(float value, bool alwaysSign, int numDigits, char bufferOut[20]);

    static float Buffer2Float(const uint8 *buffer);

    static bool String2Int(pchar buffer, int *value, char **end);

    static String Int2String(int value, bool alwaysSign, int numMinFields);
    
    // @brief Преобразует value в текстовую строку
    // @attention Строка будет храниться до следующего вызова функции. Если результат нужен большее количество времени, то его нужно скопировать себе
    // \param numMinFields минимальное число занимаемых знакомест. Если для вывода числа столько не требуется, лишние заполняются нулями
    static char *Int2String(int value, bool alwaysSign, int numMinFields, char *buffer);

    static char *UInt64_2String(uint64 value, char bufferOut[20] = 0);

    static char *UInt2String(uint value,                // значение
                             char bufferOut[20] = 0     // сюда записывается возвращаемое значение
    );
    
    // Перевести десятичное число в символьную строку длиной allDigits. После точка отстоит на forFract символов от конца
    static char *UInt2StringThisPoint(uint value, char bufferOut[20], int allDigits, int forFract);
    
    // Преобразует двоичное число ,представленное строкой из нулей и единиц, в uint
    static uint StringToBin32(char buffer[33]);

    static char *Buffer2FloatString(const uint8 *buffer);
    
    // Возвращает число слов в строке string
    static int NumWords(pchar string);
    
    // Возвращает указатель на n слово в строке. Если char == 0 - слова нет, если ret value == 0xffffffff - выходной буфер слишком мал
    static char *GetWord(char *string, int n, char *out, int size);
    
    // Вычисляет число разрядов в целом типа int.
    static int NumDigitsInNumber(int value);
    
    // Эта команда сразу преобразует к верхенму регистру слово.
    static bool GetWord(pchar string, Word *word, const int numWord);

    static bool WordEqualZeroString(Word *word, char* string);

    static int FindSymbol(pchar string, char symbol);

    static char ToUpper(char symbol);

    static char *ToUpper(void *_str, uint size);

    static void ConcatenateSymbol(char *str, char symbol);

    static char *ToUpper(char *str);

    static char ToLower(char symbol);

    static bool String2UInt64(pchar str, uint64 *value);

    static bool String2UInt(pchar str, uint *value);

    // Переводит числов в двоичный вид с заданный количеством разрядов
    static char *Bin2StringN(uint value, char buffer[33], int n);

private:
    
    // Возвращает false, если выбор невозможен - строка кончилась.
    static bool ChooseSymbols(pchar *string);
    
    // Возвращает false, если выбор невозможен - строка кончилась.
    static bool ChooseSpaces(pchar *string);
};
