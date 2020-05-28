#pragma once
#include "Display/Colors.h"


struct Text
{
    /*************** Вывод текста в области экрана **************************/


    static int DrawTextInColumnWithTransfersDiffColors(const int left, const int top, const int width, pString text, const Color colorDif, const Color = Color::NUMBER);

    // Выводит форматированный текст в столбик шириной width. При этом слов, заключённое в двойные кавычки, выводится цветом color
    static int DrawFormatTextInColumnWithTransfersDiffColors(int x, int y, int width, Color color, pString text, ...);

    // Пишет строку текста в центре области(x, y, width, height)цветом ColorText на прямоугольнике с шириной бордюра widthBorder цвета colorBackground
    static void DrawStringInCenterRectOnBackground(int x, int y, int width, int height, pString text, Color colorText, int widthBorder, Color colorBackground);
    
    static int DrawStringInCenterRectAndBoundIt(int x, int y, int width, int height, pString text, Color colorBackground, Color colorFill);

    static char *GetWord(const char *firstSymbol, int *length, char buffer[20]);

    // Если draw == false, то рисовать символ не надо, фунция используется только для вычислений
    static int DrawPartWord(char *word, int x, int y, int xRight, bool draw);

private:

    static uint *BreakWord(char *word);
    
    // Возвращает часть слова до слога numSyllable(включительн) вместе со знаком переноса
    static char *PartWordForTransfer(const char *word, const uint *lengthSyllables, int numSyllable, char buffer[30]);
    
    // \brief Находит следующий перенос. C letters начинается часть слово, где нужно найти перенос, в lettersInSyllable будет записано число букв в 
    // найденном слоге. Если слово закончилось, функция возвращает false
    static bool FindNextTransfer(const char *letters, uint *lettersInSyllable);

    static bool IsConsonant(char symbol);

    static bool CompareArrays(const bool *array1, const bool *array2, int numElems);

private:
};


class Char
{
public:
    Char(char s) : symbol(s) { }
    int Draw(int x, int y, Color color = Color::NUMBER);
    void Draw4InRect(int x, int y, Color color = Color::NUMBER);
    void Draw2Horizontal(int x, int y, Color color = Color::NUMBER);
    bool IsLetter();
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
    BigText(pString t, int s) : text(t), size(s) {}
    void Draw(int x, int y, Color color = Color::NUMBER);
private:
    pString text;
    int size;
};


struct TypeConversionString
{
    enum E
    {
        None,           // Преобразование строки не производится
        FirstUpper      // Первый символ - в вернем регистре, остальные - в нижнем
    };
};


class String // -V690
{
public:
    explicit String();
    String(const String &);
    explicit String(char symbol);
    explicit String(const char *format, ...);
    ~String();

    void Set(TypeConversionString::E conv, const char *format, ...);

    char *c_str() const;

    int Draw(int x, int y, Color color = Color::NUMBER) const;
    int DrawInCenterRect(int x, int y, int width, int height, Color color = Color::NUMBER);
    void DrawRelativelyRight(int xRight, int y, Color color = Color::NUMBER);
    void DrawInColumn(int x, int y, int width);
    // Возвращает нижнюю координату прямоугольника
    int DrawInBoundedRectWithTransfers(int x, int y, int width, Color colorBack, Color colorRect, Color colorText);
    // Пишет текст с переносами
    int DrawInColumnWithTransfers(const int left, const int top, const int width, const Color color = Color::NUMBER);

    int Length() const;

    void Free();

    void Append(const char *str);

    void Append(const char *str, uint numSymbols);

    void Append(char symbol);
    // Удаляет numSymbols из начала строки
    void RemoveFromBegin(uint numSymbols);

    void RemoveFromEnd();

    uint Size() const;

    char &operator[](uint i);

private:

    char *buffer;

    bool Allocate(uint size);

    void Conversion(TypeConversionString::E conv);

    int DrawSubString(int x, int y, pString t);
    int DrawSpaces(int x, int y, pString t, int *numSymbols);
    // Возвращает высоту экрана, которую займёт текст text, при выводе от left до right в переменной height. Если bool == false, то текст не влезет на экран 
    bool GetHeightTextWithTransfers(int left, int top, int right, int *height);
};
