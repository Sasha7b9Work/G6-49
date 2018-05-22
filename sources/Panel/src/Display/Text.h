#pragma once
#include "defines.h"
#include "Display/Colors.h"
#include "Display/DisplayTypes.h"


class Text
{
public:
    /// Устанавливает верхний регистр выводимых символов
    static void SetUpperCase(bool upper);
    
    static void SetFont(TypeFont typeFont);

    static int DrawChar(int x, int y, char symbol, Color color = Color::NUMBER);

    static void Draw4SymbolsInRect(int x, int y, char eChar, Color color = Color::NUMBER);

    static int DrawText(int x, int y, const char *text, Color color = Color::NUMBER);

    static int DrawBigChar(int eX, int eY, int size, char symbol);

    static void DrawBigText(int x, int y, int size, const char *text, Color color = Color::NUMBER);

    static void DrawTextRelativelyRight(int xRight, int y, const char *text, Color color = Color::NUMBER);
    
    static void Draw2Symbols(int x, int y, char symbol1, char symbol2, Color color1, Color color2);

    static void Draw10SymbolsInRect(int x, int y, char eChar);

    /***************** Вывод форматированного текста ***********************/
    /// Пишет строку в позиции x, y
    static int DrawFormatText(int x, int y, pString text, ...);

    /*************** Вывод текста в области экрана **************************/

    static void DrawTextInColumn(int x, int y, int width, const char *text);
    /// Возвращает нижнюю координату прямоугольника
    static int DrawTextInBoundedRectWithTransfers(int x, int y, int width, const char *text, Color colorBack, Color colorRect, Color colorText);
    /// Пишет текст с переносами
    static int DrawTextInColumnWithTransfers(const int left, const int top, const int width, pString text, const Color color = Color::NUMBER);

    static int DrawTextInColumnWithTransfersDiffColors(const int left, const int top, const int width, pString text, const Color colorDif,
                                                       const Color = Color::NUMBER);

    static int DrawFormatTextInColumnWithTransfers(int x, int y, int width, pString text, ...);
    /// Выводит форматированный текст в столбик шириной width. При этом слов, заключённое в двойные кавычки, выводится цветом color
    static int DrawFormatTextInColumnWithTransfersDiffColors(int x, int y, int width, Color color, pString text, ...);

    static int DrawStringInCenterRect(int x, int y, int width, int height, const char *text, Color color = Color::NUMBER);

    static int DrawFormatStringInCenterRect(int x, int y, int width, int height, const char *text, ...);
    /// Пишет строку текста в центре области(x, y, width, height)цветом ColorText на прямоугольнике с шириной бордюра widthBorder цвета colorBackground
    static void DrawStringInCenterRectOnBackground(int x, int y, int width, int height, const char *text, Color colorText, int widthBorder,
                                                    Color colorBackground);
    static int DrawStringInCenterRectAndBoundIt(int x, int y, int width, int height, const char *text, Color colorBackground, Color colorFill);
    /// Если true, то все буквы выводятся в верхнем регистре
    static bool IsUpperCase();

private:

    static bool ByteFontNotEmpty(int eChar, int byte);

    static bool BitInFontIsExist(int eChar, int numByte, int bit);
    /// Возвращает высоту экрана, которую займёт текст text, при выводе от left до right в переменной height. Если bool == false, то текст не влезет на экран 
    static bool GetHeightTextWithTransfers(int left, int top, int right, const char *text, int *height);

    static char *GetWord(const char *firstSymbol, int *length, char buffer[20]);

    static bool IsLetter(char symbol);
    /// Если draw == false, то рисовать символ не надо, фунция используется только для вычислений
    static int DrawPartWord(char *word, int x, int y, int xRight, bool draw);

    static int8 *BreakWord(char *word);
    /// Возвращает часть слова до слога numSyllable(включительн) вместе со знаком переноса
    static char *PartWordForTransfer(char *word, int8 *lengthSyllables, int numSyllable, char buffer[30]);
    /// \brief Находит следующий перенос. C letters начинается часть слово, где нужно найти перенос, в lettersInSyllable будет записано число букв в 
    /// найденном слоге. Если слово закончилось, функция возвращает false
    static bool FindNextTransfer(const char *letters, int8 *lettersInSyllable);

    static bool IsConsonant(char symbol);

    static bool CompareArrays(const bool *array1, const bool *array2, int numElems);

    static int GetLenghtSubString(const char *text);

    static int DrawSubString(int x, int y, const char *text);

    static int DrawSpaces(int x, int y, const char *text, int *numSymbols);

    static bool upperCase;
};
