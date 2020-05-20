#pragma once
#include "Display/Colors.h"
#include "Display/DisplayTypes.h"
#include "Display/Font/Font.h"


struct Text
{
    // Устанавливает верхний регистр выводимых символов
    static void SetUpperCase(bool upper);

    // Вывод форматированного текста
    static int DrawFormat(int x, int y, pString text, ...);
    
    static void Draw4SymbolsInRect(int x, int y, char eChar, Color color = Color::NUMBER);

    static void DrawBigText(int x, int y, int size, pString text, Color color = Color::NUMBER);

    static void DrawTextRelativelyRight(int xRight, int y, pString text, Color color = Color::NUMBER);
    
    /*************** Вывод текста в области экрана **************************/

    static void DrawTextInColumn(int x, int y, int width, pString text);

    // Возвращает нижнюю координату прямоугольника
    static int DrawTextInBoundedRectWithTransfers(int x, int y, int width, pString text, Color colorBack, Color colorRect, Color colorText);

    // Пишет текст с переносами
    static int DrawTextInColumnWithTransfers(const int left, const int top, const int width, pString text, const Color color = Color::NUMBER);

    static int DrawTextInColumnWithTransfersDiffColors(const int left, const int top, const int width, pString text, const Color colorDif, const Color = Color::NUMBER);

    static int DrawFormatTextInColumnWithTransfers(int x, int y, int width, pString text, ...);

    // Выводит форматированный текст в столбик шириной width. При этом слов, заключённое в двойные кавычки, выводится цветом color
    static int DrawFormatTextInColumnWithTransfersDiffColors(int x, int y, int width, Color color, pString text, ...);

    static int DrawStringInCenterRect(int x, int y, int width, int height, pString text, Color color = Color::NUMBER);

    static int DrawFormatStringInCenterRect(int x, int y, int width, int height, pString text, ...);
    
    // Пишет строку текста в центре области(x, y, width, height)цветом ColorText на прямоугольнике с шириной бордюра widthBorder цвета colorBackground
    static void DrawStringInCenterRectOnBackground(int x, int y, int width, int height, pString text, Color colorText, int widthBorder, Color colorBackground);
    
    static int DrawStringInCenterRectAndBoundIt(int x, int y, int width, int height, pString text, Color colorBackground, Color colorFill);
    
    // Если true, то все буквы выводятся в верхнем регистре
    static bool IsUpperCase();

    static int Draw(int x, int y, pString text, Color color = Color::NUMBER);

private:
    
    // Возвращает высоту экрана, которую займёт текст text, при выводе от left до right в переменной height. Если bool == false, то текст не влезет на экран 
    static bool GetHeightTextWithTransfers(int left, int top, int right, pString text, int *height);

    static char *GetWord(const char *firstSymbol, int *length, char buffer[20]);

    static bool IsLetter(char symbol);
    
    // Если draw == false, то рисовать символ не надо, фунция используется только для вычислений
    static int DrawPartWord(char *word, int x, int y, int xRight, bool draw);

    static uint *BreakWord(char *word);
    
    // Возвращает часть слова до слога numSyllable(включительн) вместе со знаком переноса
    static char *PartWordForTransfer(const char *word, const uint *lengthSyllables, int numSyllable, char buffer[30]);
    
    // \brief Находит следующий перенос. C letters начинается часть слово, где нужно найти перенос, в lettersInSyllable будет записано число букв в 
    // найденном слоге. Если слово закончилось, функция возвращает false
    static bool FindNextTransfer(const char *letters, uint *lettersInSyllable);

    static bool IsConsonant(char symbol);

    static bool CompareArrays(const bool *array1, const bool *array2, int numElems);

    static int GetLenghtSubString(pString text);

    static int DrawSubString(int x, int y, pString text);

    static int DrawSpaces(int x, int y, pString text, int *numSymbols);

    static bool upperCase;
};


class Char
{
public:
    Char(char s) : symbol(s) { }
    int Draw(int x, int y, Color color = Color::NUMBER);
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

