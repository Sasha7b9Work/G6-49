// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Editor/Painter/Canvas.h"
#include "Editor/Painter/Colors.h"

#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)


struct Grid
{
    static int X();
    static int Y();
    static int Width();
    static int Height();
    static int Bottom();
    static int Right();
    static void Draw();
    static void ChangeTypeGrid();
    
    // ���������� true, ���� ����� ����� ��������� � ���������
    static bool TypeIsPercents() { return typeIsPercents; }
    
    // ���������� ���������� ����� ������������� ������� � ������ ��� ���������
    static wxString GetScale();

private:
    static bool typeIsPercents;
    static wxString sScale;
    static int deltaPoints;
    static int deltaPercents;
    static void DrawTypePercents();
    static void DrawTypePoints();
    static void DrawHorizontalLines();

    // ������������ ������ ����� � ������
    static void CalculateDeltaPoints();

    // ������������ ������ ����� � ���������. ������ ������� - ���� �������� ��������. 1% == 1000
    static void CalculateDeltaPercents();

    // 
    static void CalculateScale();
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
