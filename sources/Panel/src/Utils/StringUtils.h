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
    // ���������� ��������� �� ����������� (�������) ������ ������
    static char *FindEnd(char *buffer);

    static char *Float2String(float value);
    
    // \brief ��������� ����� � ��������� ������ � ��������� ���. numDigits - ����� �������� ���������
    // \attention ������ ����� ��������� �� ���������� ������ �������. ���� ��������� ����� ������� ���������� �������, �� ��� ����� ������������ ����.
    // \retval ��������� �� ������ � ������.
    static char *Float2String(float value, bool alwaysSign, int numDigits, char bufferOut[20]);

    static float Buffer2Float(const uint8 *buffer);

    static bool String2Int(pchar buffer, int *value, char **end);

    static String Int2String(int value, bool alwaysSign, int numMinFields);
    
    // @brief ����������� value � ��������� ������
    // @attention ������ ����� ��������� �� ���������� ������ �������. ���� ��������� ����� ������� ���������� �������, �� ��� ����� ����������� ����
    // \param numMinFields ����������� ����� ���������� ���������. ���� ��� ������ ����� ������� �� ���������, ������ ����������� ������
    static char *Int2String(int value, bool alwaysSign, int numMinFields, char *buffer);

    static char *UInt64_2String(uint64 value, char bufferOut[20] = 0);

    static char *UInt2String(uint value,                // ��������
                             char bufferOut[20] = 0     // ���� ������������ ������������ ��������
    );
    
    // ��������� ���������� ����� � ���������� ������ ������ allDigits. ����� ����� ������� �� forFract �������� �� �����
    static char *UInt2StringThisPoint(uint value, char bufferOut[20], int allDigits, int forFract);
    
    // ����������� �������� ����� ,�������������� ������� �� ����� � ������, � uint
    static uint StringToBin32(char buffer[33]);

    static char *Buffer2FloatString(const uint8 *buffer);
    
    // ���������� ����� ���� � ������ string
    static int NumWords(pchar string);
    
    // ���������� ��������� �� n ����� � ������. ���� char == 0 - ����� ���, ���� ret value == 0xffffffff - �������� ����� ������� ���
    static char *GetWord(char *string, int n, char *out, int size);
    
    // ��������� ����� �������� � ����� ���� int.
    static int NumDigitsInNumber(int value);
    
    // ��� ������� ����� ����������� � �������� �������� �����.
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

    // ��������� ������ � �������� ��� � �������� ����������� ��������
    static char *Bin2StringN(uint value, char buffer[33], int n);

private:
    
    // ���������� false, ���� ����� ���������� - ������ ���������.
    static bool ChooseSymbols(pchar *string);
    
    // ���������� false, ���� ����� ���������� - ������ ���������.
    static bool ChooseSpaces(pchar *string);
};
