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
    // ���������� ��������� �� ����������� (�������) ������ ������
    char *FindEnd(char *buffer);

    char *Float2String(float value);

    // \brief ��������� ����� � ��������� ������ � ��������� ���. numDigits - ����� �������� ���������
    // \attention ������ ����� ��������� �� ���������� ������ �������. ���� ��������� ����� ������� ���������� �������, �� ��� ����� ������������ ����.
    // \retval ��������� �� ������ � ������.
    char *Float2String(float value, bool alwaysSign, int numDigits, char bufferOut[20]);

    float Buffer2Float(const uint8 *buffer);

    bool String2Int(pchar buffer, int *value, char **end);

    String Int2String(int value, bool alwaysSign, int numMinFields);

    // @brief ����������� value � ��������� ������
    // @attention ������ ����� ��������� �� ���������� ������ �������. ���� ��������� ����� ������� ���������� �������, �� ��� ����� ����������� ����
    // \param numMinFields ����������� ����� ���������� ���������. ���� ��� ������ ����� ������� �� ���������, ������ ����������� ������
    char *Int2String(int value, bool alwaysSign, int numMinFields, char *buffer);

    char *UInt64_2String(uint64 value, char bufferOut[20] = 0);

    char *UInt2String(uint value,                // ��������
        char bufferOut[20] = 0     // ���� ������������ ������������ ��������
    );

    // ��������� ���������� ����� � ���������� ������ ������ allDigits. ����� ����� ������� �� forFract �������� �� �����
    char *UInt2StringThisPoint(uint value, char bufferOut[20], int allDigits, int forFract);

    // ����������� �������� ����� ,�������������� ������� �� ����� � ������, � uint
    uint StringToBin32(char buffer[33]);

    char *Buffer2FloatString(const uint8 *buffer);

    // ���������� ����� ���� � ������ string
    int NumWords(pchar string);

    // ���������� ��������� �� n ����� � ������. ���� char == 0 - ����� ���, ���� ret value == 0xffffffff - �������� ����� ������� ���
    char *GetWord(char *string, int n, char *out, int size);

    // ��������� ����� �������� � ����� ���� int.
    int NumDigitsInNumber(int value);

    // ��� ������� ����� ����������� � �������� �������� �����.
    bool GetWord(pchar string, Word *word, const int numWord);

    bool WordEqualZeroString(Word *word, char *string);

    // ���� ������� ���, ���������� -1
    int FindSymbol(pchar string, char symbol);

    char ToUpper(char symbol);

    char *ToUpper(void *_str, uint size);

    void ConcatenateSymbol(char *str, char symbol);

    char *ToUpper(char *str);

    char ToLower(char symbol);

    bool String2UInt64(pchar str, uint64 *value);

    bool String2UInt(pchar str, uint *value);

    // ��������� ������ � �������� ��� � �������� ����������� ��������
    char *Bin2StringN(uint value, char buffer[33], int n);

    String GetSubString(pchar, int first, int last);
};
