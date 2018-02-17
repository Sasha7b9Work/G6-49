#pragma once
#include "defines.h"
#include "Colors.h"
#include "DisplayTypes.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern void CalculateCurrentColor();


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/** @addtogroup Display
 *  @{
 *  @defgroup Painter
 *  @brief Функции рисования
 *  @{
 */

class Painter
{
public:
    /// Вызывается в начале отрисовки каждого кадра. Заполняет буфер цветом color
    static void BeginScene(Color color);
    /// \brief Заставляет дисплей вывполнить ранее засланные в него команды, не дожидаясь завершающей отрисовку команды EndScene(). 
    /// Нужно вызывать, если команды отрисовки кадра превышают размер буфера команд дисплея. Например, когда отрисовывается много сигналов на экране в 
    /// режиме накопления.
    static void RunDisplay();
    /// Вызывается в конце отрисовки каждого кадра. Переносит содержимое буфера на экран
    static void EndScene();
    /// Установить цвет рисования
    static void SetColor(Color color);

    static void SetPoint(int x, int y);

    static void DrawHLine(int y, int x0, int x1);

    static void DrawVLine(int x, int y0, int y1);

    static void DrawLine(int x0, int y0, int x1, int y1);

    static void DrawRectangle(int x, int y, int width, int height);

    static void FillRegion(int x, int y, int width, int height);

    static void DrawBigText(int x, int y, int size, const char *text);

    static int DrawTextC(int x, int y, const char *text, Color color);

    static void FillRegionC(int x, int y, int width, int height, Color color);

    static void DrawRectangleC(int x, int y, int width, int height, Color color);

    static int DrawText(int x, int y, const char *text);

    static void SetFont(TypeFont typeFont);
    
    static int DrawChar(int x, int y, char symbol);
    
    static int DrawBigChar(int eX, int eY, int size, char symbol);

    static void DrawTextInRect(int x, int y, int width, const char *text);

    static void DrawTextRelativelyRight(int xRight, int y, const char *text);

    static void Draw4SymbolsInRect(int x, int y, char eChar);

private:

    static bool ByteFontNotEmpty(int eChar, int byte);

    static bool BitInFontIsExist(int eChar, int numByte, int bit);

    static int DrawSpaces(int x, int y, const char *text, int *numSymbols);

    static int DrawSubString(int x, int y, const char *text);

    static int GetLenghtSubString(const char *text);

    static uint8 color;
};

#define WRITE_BYTE(offset, value)   *(command + offset) = (uint8)value
#define WRITE_SHORT(offset, value)  *((uint16*)(command + offset)) = (uint16)value

/** @} @}
 */
