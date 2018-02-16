#pragma once
#include "Painter.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @addtogroup Painter
 *  @{
 *  @defgroup PainterC
 *  @brief Макросы для добавления цвета в функции рисования
 *  @{
 */

class PainterC
{
public:
    static void FillRegion(int x, int y, int width, int height, Color color);
    static void DrawRectangle(int x, int y, int width, int height, Color color);
};

#define Painter_SetPointC(x, y, color)                                      \
    Painter::SetColor(color);                                                \
    Painter::SetPoint(x, y);

#define Painter_DrawVLineC(x, y0, y1, color)                                \
    Painter::SetColor(color);                                                \
    Painter::DrawVLine(x, y0, y1);

#define Painter_DrawHLineC(y, x0, x1, color)                                \
    Painter::SetColor(color);                                                \
    Painter::DrawHLine(y, x0, x1);

#define Painter_DrawLineC(x0, y0, x1, y1, color)                            \
    Painter::SetColor(color);                                                \
    Painter::DrawLine(x0, y0, x1, y1);

#define Painter_DrawDashedHLineC(y, x0, x1, dFill, dEmpty, dStart, color);  \
    Painter::SetColor(color);                                                \
    Painter::DrawDashedHLine(y, x0, x1, dFill, dEmpty, dStart);

#define Painter_DrawDashedVLineC(x, y0, y1, dFill, dEmpty, dStart, color);  \
    Painter::SetColor(color);                                                \
    Painter::DrawDashedVLine(x, y0, y1, dFill, dEmpty, dStart);

#define Painter_Draw4SymbolsInRectC(x, y, eChar, color)                     \
    Painter::SetColor(color);                                                \
    Painter::Draw4SymbolsInRect(x, y, eChar);

#define Painter_DrawTextInRectC(x, y, width, text, color)                   \
    Painter::SetColor(color);                                                \
    Painter::DrawTextInRect(x, y, width, text);

#define Painter_DrawTextRelativelyRightC(x, y, text, color)                 \
    Painter::SetColor(color);                                                \
    Painter::DrawTextRelativelyRight(x, y, text);

/** @}  @}
 */
