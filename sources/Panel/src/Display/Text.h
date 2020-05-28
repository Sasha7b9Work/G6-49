#pragma once
#include "Display/Colors.h"
#include "Display/DisplayTypes.h"
#include "Display/Font/Font.h"


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

    static bool IsLetter(char symbol);

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

