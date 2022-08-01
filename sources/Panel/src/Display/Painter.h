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
    class Point
    {
    public:
        void Draw(int x, int y);
    };


    class Line
    {
    public:
        static void Draw(int x0, int y0, int x1, int y1, const Color & = Color::NUMBER);
    };


    class VLine
    {
    public:
        static void Draw(int x, int y0, int y1, const Color & = Color::NUMBER);
    };


    class HLine
    {
    public:
        static void Draw(int y, int x0, int x1, const Color & = Color::NUMBER);
    };


    class Rectangle
    {
    public:
        Rectangle(int w, int h) : width(w), height(h) { };
        void Draw(int x, int y, const Color & = Color::NUMBER);
        void Fill(int x, int y, const Color & = Color::NUMBER);
        void DrawFilled(int x, int y, Color colorFill, Color colorRect);

    private:
        int width;
        int height;
    };

}
