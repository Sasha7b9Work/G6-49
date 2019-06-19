#pragma once
#include "defines.h"
#include "Display/Colors.h"
#include "Display/DisplayTypes.h"
#include "Display/Font/Font.h"


namespace Text
{
    /// Устанавливает верхний регистр выводимых символов
    void SetUpperCase(bool upper);
    
    int DrawChar(int x, int y, char symbol, Color color = Color::NUMBER);

    void Draw4SymbolsInRect(int x, int y, char eChar, Color color = Color::NUMBER);

    int DrawBigChar(int eX, int eY, int size, char symbol);

    void DrawBigText(int x, int y, int size, pString text, Color color = Color::NUMBER);

    void DrawTextRelativelyRight(int xRight, int y, pString text, Color color = Color::NUMBER);
    
    void Draw2Symbols(int x, int y, char symbol1, char symbol2, Color color1, Color color2);

    void Draw10SymbolsInRect(int x, int y, char eChar);

    /***************** Вывод форматированного текста ***********************/
    /// Пишет строку в позиции x, y
    int DrawFormatText(int x, int y, pString text, ...);

    /*************** Вывод текста в области экрана **************************/

    void DrawTextInColumn(int x, int y, int width, pString text);
    /// Возвращает нижнюю координату прямоугольника
    int DrawTextInBoundedRectWithTransfers(int x, int y, int width, pString text, Color colorBack, Color colorRect, Color colorText);
    /// Пишет текст с переносами
    int DrawTextInColumnWithTransfers(const int left, const int top, const int width, pString text, const Color color = Color::NUMBER);

    int DrawTextInColumnWithTransfersDiffColors(const int left, const int top, const int width, pString text, const Color colorDif,
                                                       const Color = Color::NUMBER);

    int DrawFormatTextInColumnWithTransfers(int x, int y, int width, pString text, ...);
    /// Выводит форматированный текст в столбик шириной width. При этом слов, заключённое в двойные кавычки, выводится цветом color
    int DrawFormatTextInColumnWithTransfersDiffColors(int x, int y, int width, Color color, pString text, ...);

    int DrawStringInCenterRect(int x, int y, int width, int height, pString text, Color color = Color::NUMBER);

    int DrawFormatStringInCenterRect(int x, int y, int width, int height, pString text, ...);
    /// Пишет строку текста в центре области(x, y, width, height)цветом ColorText на прямоугольнике с шириной бордюра widthBorder цвета colorBackground
    void DrawStringInCenterRectOnBackground(int x, int y, int width, int height, pString text, Color colorText, int widthBorder,
                                                    Color colorBackground);
    int DrawStringInCenterRectAndBoundIt(int x, int y, int width, int height, pString text, Color colorBackground, Color colorFill);
    /// Если true, то все буквы выводятся в верхнем регистре
    bool IsUpperCase();

    int DrawText(int x, int y, pString text, Color color = Color::NUMBER);
};
