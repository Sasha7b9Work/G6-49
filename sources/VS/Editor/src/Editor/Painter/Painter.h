// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Editor/Painter/Colors.h"

#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)


namespace Painter
{
    void Init(wxWindow *parent);

    void DeInit();

    wxBitmap *GetBitmap();

    void OnResizeEvent(wxSizeEvent &event);

    void BegineScene();

    void EndScene();

    void SetColor(const Color &color = Color::NUMBER);

    void DrawPoint(int x, int y, int size, const Color &color = Color::NUMBER);

    void DrawLine(int x0, int y0, int x1, int y1, const Color &color = Color::NUMBER);
    void DrawVLine(int x, int top, int bottom, const Color &color = Color::NUMBER);
    void DrawHLine(int y, int left, int right, const Color &color = Color::NUMBER);

    void FillRegion(int x, int y, int width, int height, const Color &color = Color::NUMBER);

    void DrawRectangle(int x, int y, int width, int height, const Color &color = Color::NUMBER);

    void DrawFilledRectangle(int x, int y, int width, int height, const Color &colorFill, const Color &colorBorder = Color::NUMBER);

    void DrawText(int x, int y, const wxString &text, const Color &color = Color::NUMBER);

    void DrawTextInZone(int x, int y, int widht, const wxString &text, const Color &color = Color::NUMBER);

    int GetLengthText(const wxString &text);
};
