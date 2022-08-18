#pragma once
#include "Display/Colors.h"
#include "Display/DisplayTypes.h"
#include "Display/Font/Font.h"


namespace Painter
{
    // Вызывается в начале отрисовки каждого кадра. Заполняет буфер цветом color
    void BeginScene(const Color &);
    
    // Вызывается в конце отрисовки каждого кадра. Переносит содержимое буфера на экран
    void EndScene();
    
    // Загружает в дисплей все цвета
    void LoadPalette();
};


namespace Primitives
{
    struct Point
    {
        void Draw(int x, int y);
    };


    struct Line
    {
        static void Draw(int x0, int y0, int x1, int y1, const Color & = Color::NUMBER);
    };


    struct VLine
    {
        static void Draw(int x, int y0, int y1, const Color & = Color::NUMBER);
    };


    struct HLine
    {
        static void Draw(int y, int x0, int x1, const Color & = Color::NUMBER);
    };


    struct Rectangle
    {
        Rectangle(int w, int h) : width(w), height(h) { };
        void Draw(int x, int y, const Color & = Color::NUMBER);
        void Fill(int x, int y, const Color & = Color::NUMBER);
        void DrawFilled(int x, int y, Color colorFill, Color colorRect);

    private:
        int width;
        int height;
    };

}
