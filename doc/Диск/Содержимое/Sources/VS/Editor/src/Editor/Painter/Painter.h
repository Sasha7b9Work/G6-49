#pragma once
#include "Editor/Painter/Colors.h"

#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)


struct Painter
{
    static void Init(wxWindow *parent);

    static void DeInit();

    static wxBitmap *GetBitmap();

    static void OnResizeEvent(wxSizeEvent &event);

    static void BegineScene();

    static void EndScene();

    static void SetColor(const Color &color = Color::NUMBER);

    static void DrawPoint(int x, int y, int size, const Color &color = Color::NUMBER);

    static void DrawLine(int x0, int y0, int x1, int y1, const Color &color = Color::NUMBER);
    static void DrawVLine(int x, int top, int bottom, const Color &color = Color::NUMBER);
    static void DrawHLine(int y, int left, int right, const Color &color = Color::NUMBER);

    static void FillRegion(int x, int y, int width, int height, const Color &color = Color::NUMBER);

    static void DrawRectangle(int x, int y, int width, int height, const Color &color = Color::NUMBER);

    static void DrawFilledRectangle(int x, int y, int width, int height, const Color &colorFill, const Color &colorBorder = Color::NUMBER);

    static void DrawText(int x, int y, const wxString &text, const Color &color = Color::NUMBER);

    static void DrawTextInZone(int x, int y, int widht, const wxString &text, const Color &color = Color::NUMBER);

    static int GetLengthText(const wxString &text);

private:

    static void SetSolidBrush(const Color &color);

    static void SetTransparentBrush();

    static wxColour MakeColour(const Color &color);

    static Color currentColor;
    static wxWindow *parent;
    static wxBitmap *bitmap;        // Здесь рисуем
    static wxMemoryDC memDC;        // Контекст рисования
};
