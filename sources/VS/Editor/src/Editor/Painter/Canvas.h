// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Editor/Painter/Canvas.h"
#include "Editor/Painter/Colors.h"

#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)


namespace Grid
{
    int X();
    int Y();
    int Width();
    int Height();
    int Bottom();
    int Right();
    void Draw();
    void ChangeTypeGrid();

    // ���������� true, ���� ����� ����� ��������� � ���������
    bool TypeIsPercents();

    // ���������� ���������� ����� ������������� ������� � ������ ��� ���������
    wxString GetScale();
};


class Canvas : public wxPanel
{
public:

    Canvas(wxWindow *parent);

    ~Canvas();

    void Draw();

    void Redraw();

private:

    bool mouseIsDown = false;       // true, ���� ��� ��������� � ������� ���������
    int mouseX = 0;
    int mouseY = 0;
    wxWindow *parent;

    void BeginScene();

    void EndScene();

    void OnPaint(wxPaintEvent &);

    void OnResize(wxSizeEvent &);

    void OnMouseWheel(wxMouseEvent &);

    void OnMouseMove(wxMouseEvent &);

    void OnMouseLeftDown(wxMouseEvent &);

    void OnMouseRightDown(wxMouseEvent &);

    void OnMouseLeftUp(wxMouseEvent &);

    void OnMouseRightUp(wxMouseEvent &);

    void OnMouseEnter(wxMouseEvent &);

    void OnMouseLeave(wxMouseEvent &);

    // ���������� ����������� ����� ������� ����
    void SetMouseCursor();

    bool mouseInWindow = false;
};


extern Canvas *TheCanvas;
