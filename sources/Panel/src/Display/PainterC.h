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

/** @}  @}
 */
