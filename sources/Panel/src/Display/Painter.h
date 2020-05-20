#pragma once
#include "Display/Colors.h"
#include "Display/DisplayTypes.h"
#include "Display/Font/Font.h"


struct Painter
{
    // Вызывается в начале отрисовки каждого кадра. Заполняет буфер цветом color
    static void BeginScene(Color color);
    // Вызывается в конце отрисовки каждого кадра. Переносит содержимое буфера на экран
    static void EndScene();
    // Загружает в дисплей все цвета
    static void LoadPalette();
    // Нарисовать точку текущим цветом
    static void SetPoint(int x, int y);
    // Нарисовать горизонтальную линию от x0 до x1 точками через каждые delta пикселей
    static void DrawHPointLine(int y, int x0, int x1, float delta);
    // Нарисовать вертикальную линию от y0 до y1 точками через каждые delta пикселей
    static void DrawVPointLine(int x, int y0, int y1, float delta);
    // Нарисовать горизонтальную линию
    static void DrawHLine(int y, int x0, int x1, Color color = Color::NUMBER);
    // Нарисовать вертикальную линию
    static void DrawVLine(int x, int y0, int y1, Color color = Color::NUMBER);
    // Нарисовать произвольную линию
    static void DrawLine(int x0, int y0, int x1, int y1, Color color = Color::NUMBER);
    // \brief Рисует прерывистую горизонтальную линию. dFill - длина штриха, dEmpty - расст. между штрихами. Линия всегда начинается со штриха. 
    // dStart указывает смещение первой рисуемой точки относительно начала штриха.
    static void DrawDashedHLine(int y, int x0, int x1, int dFill, int dEmpty, int dStart);
    // Рисует прерывистую вертикальную линию.
    static void DrawDashedVLine(int x, int y0, int y1, int dFill, int dEmpty, int dStart);

    static void DrawRectangle(int x, int y, int width, int height, Color color = Color::NUMBER);

    static void DrawFilledRectangle(int x, int y, int width, int height, Color colorFill, Color colorRect);

    static void DrawVolumeButton(int x, int y, int width, int height, int thickness, Color normal, Color bright, Color dark, bool isPressed, bool isShade);
};


class Region
{
public:
    Region(int w, int h) : width(w), height(h) { };
    void Fill(int x, int y, Color color = Color::NUMBER);

private:
    int width;
    int height;
};
