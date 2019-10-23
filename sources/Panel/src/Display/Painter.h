#pragma once
#include "defines.h"
#include "Display/Colors.h"
#include "Display/DisplayTypes.h"
#include "Display/Font/Font.h"




/** @addtogroup Display
 *  @{
 *  @defgroup Painter
 *  @brief Функции рисования
 *  @{
 */

namespace Painter
{
    /// Вызывается в начале отрисовки каждого кадра. Заполняет буфер цветом color
    void BeginScene(Color color);
    /// Вызывается в конце отрисовки каждого кадра. Переносит содержимое буфера на экран
    void EndScene();
    /// Установить цвет рисования
    void SetColor(Color color);
    /// Возвращает текущий цвет рисования
    Color GetColor();
    /// Устанавливает цвету заданное значение. Загружается в дисплей LoadPalette() или SetPalette()
    void SetColorValue(Color color, col_val value);
    /// Загружает в дисплей все цвета
    void LoadPalette();
    /// Нарисовать точку текущим цветом
    void SetPoint(int x, int y);
    /// Нарисовать горизонтальную линию от x0 до x1 точками через каждые delta пикселей
    void DrawHPointLine(int y, int x0, int x1, float delta);
    /// Нарисовать вертикальную линию от y0 до y1 точками через каждые delta пикселей
    void DrawVPointLine(int x, int y0, int y1, float delta);
    /// Нарисовать горизонтальную линию
    void DrawHLine(int y, int x0, int x1, Color color = Color::NUMBER);
    /// Нарисовать вертикальную линию
    void DrawVLine(int x, int y0, int y1, Color color = Color::NUMBER);
    /// Нарисовать произвольную линию
    void DrawLine(int x0, int y0, int x1, int y1, Color color = Color::NUMBER);
    /// \brief Рисует прерывистую горизонтальную линию. dFill - длина штриха, dEmpty - расст. между штрихами. Линия всегда начинается со штриха. 
    /// dStart указывает смещение первой рисуемой точки относительно начала штриха.
    void DrawDashedHLine(int y, int x0, int x1, int dFill, int dEmpty, int dStart);
    /// Рисует прерывистую вертикальную линию.
    void DrawDashedVLine(int x, int y0, int y1, int dFill, int dEmpty, int dStart);

    void DrawRectangle(int x, int y, int width, int height, Color color = Color::NUMBER);

    void DrawFilledRectangle(int x, int y, int width, int height, Color colorFill, Color colorRect);

    void FillRegion(int x, int y, int width, int height, Color color = Color::NUMBER);

    void DrawVolumeButton(int x, int y, int width, int height, int thickness, Color normal, Color bright, Color dark, bool isPressed, bool isShade);

    col_val ReduceBrightness(col_val colorValue, float newBrightness);

    extern Color currentColor;

    extern Font::Type currentTypeFont;
};

/** @} @}
 */
