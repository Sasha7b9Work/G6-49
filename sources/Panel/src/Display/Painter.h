#pragma once
#include "defines.h"
#include "Colors.h"
#include "DisplayTypes.h"
#include "PainterText.h"


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
};

#define WRITE_BYTE(offset, value)   *(command + offset) = (uint8)value
#define WRITE_SHORT(offset, value)  *((uint16*)(command + offset)) = (uint16)value

/** @} @}
 */
