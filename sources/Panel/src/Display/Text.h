// 2022/8/1 11:34:38 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Display/Colors.h"


class Char
{
public:
    Char(char s) : symbol(s) { }
    int Draw(int x, int y, const Color & = Color::NUMBER);
    void Draw4InRect(int x, int y, const Color & = Color::NUMBER);
    void Draw2Horizontal(int x, int y, const Color & = Color::NUMBER);
    bool IsLetter();
    bool IsConsonant();
private:
    char symbol;
};


class BigChar
{
public:
    BigChar(char s, int _size) : symbol(s), size(_size) { }
    int Draw(int eX, int eY);
private:
    char symbol;
    int size;
};


class BigText
{
public:
    BigText(cstr t, int s) : text(t), size(s) {}
    void Draw(int x, int y, const Color & = Color::NUMBER);
private:
    cstr text;
    int size;
};


struct TypeConversionString
{
    enum E
    {
        None,           // �������������� ������ �� ������������
        FirstUpper      // ������ ������ - � ������ ��������, ��������� - � ������
    };
};


class String // -V690
{
public:
    explicit String();
    String(const String &);
    explicit String(char symbol);
    explicit String(pchar format, ...);
    ~String();

    void Set(TypeConversionString::E conv, pchar format, ...);

    char *c_str() const;

    int Draw(int x, int y, const Color & = Color::NUMBER) const;
    
    int DrawInCenterRect(int x, int y, int width, int height, const Color & = Color::NUMBER) const;
    
    int DrawInArea(int x, int y, int width, const Color & = Color::NUMBER);
    
    void DrawRelativelyRight(int xRight, int y, const Color & = Color::NUMBER);
    
    // �������� ����� "� �������"
    void DrawInColumn(int x, int y, int width, int delta = 2);
    
    int DrawInColumnWithTransfers(const int left, const int top, const int width, const Color & = Color::NUMBER);
    
    int DrawInColumnWithTransfersDiffColors(const int left, const int top, const int width, const Color colorDif, const Color = Color::NUMBER);
    
    // ���������� ������ ���������� ��������������
    int DrawInBoundedRectWithTransfers(int x, int y, int width, Color colorBack, Color colorRect, Color colorText);
    
    // ����� ������ ������ � ������ �������(x, y, width, height)������ ColorText �� �������������� � ������� ������� widthBorder ����� colorBackground
    void DrawInCenterRectOnBackground(int x, int y, int width, int height, Color colorText, int widthBorder, Color colorBackground);
    
    int DrawInCenterRectAndBoundIt(int x, int y, int width, int height, Color colorBackground, Color colorFill);

    // ������� �������� ������ ����� � ������ ��� ������ �� �����
    int Width() const;

    void Free();

    void Append(pchar str);

    void Append(pchar str, uint numSymbols);

    void Append(char symbol);

    // ������� numSymbols �� ������ ������
    void RemoveFromBegin(uint numSymbols);

    // ������� ��������� ������
    void RemoveEndSymbol();

    // ������� ��������� ������� symbol
    void RemoveFromEnd(char symbol);

    // ������ ������ � �������� (��� ����� ������������ ����)
    int Size() const;

    // ���������� ���������� ����, ���������� ���������
    int NumberWords() const;

    char &operator[](int i) const;

    String operator=(const String &);

private:

    char *text;

    bool Allocate(uint size);

    void Conversion(TypeConversionString::E conv);

    int DrawSubString(int x, int y, cstr t);

    int DrawSpaces(int x, int y, cstr t, int *numSymbols);

    // ���������� ������ ������, ������� ����� ����� text, ��� ������ �� left �� right � ���������� height. ���� bool == false, �� ����� �� ������ �� ����� 
    bool GetHeightTextWithTransfers(int left, int top, int right, int *height);

    // ���� draw == false, �� �������� ������ �� ����, ������ ������������ ������ ��� ����������
    int DrawPartWord(char *word, int x, int y, int xRight, bool draw);

    // ���������� ������, ���������� ������ �� ������� ������� ��� ����-�������
    int WidthWithoutSpaces(pchar t) const;
};
