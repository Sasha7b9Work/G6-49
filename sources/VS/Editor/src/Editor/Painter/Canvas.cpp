#include "defines.h"
#include "Editor/Editor.h"
#include "Editor/Form.h"
#include "Editor/History.h"
#include "Editor/Selector.h"
#include "Editor/Settings.h"
#include "Editor/Painter/Canvas.h"
#include "Editor/Painter/Painter.h"
#include "Editor/Painter/Zoomer.h"

#include <ctime>
#pragma warning(push, 0)
#include <wx/msw/private.h>
#pragma warning(pop)


Canvas *TheCanvas = nullptr;
static bool needRedraw = true;


Canvas::Canvas(wxWindow *p) : wxPanel(p, wxID_ANY), parent(p)
{
    Painter::Init(p);

    SetDoubleBuffered(true);
    Bind(wxEVT_PAINT,      &Canvas::OnPaint,          this);
    Bind(wxEVT_SIZE,       &Canvas::OnResize,         this);
    Bind(wxEVT_MOTION,     &Canvas::OnMouseMove,      this);
    Bind(wxEVT_LEFT_DOWN,  &Canvas::OnMouseLeftDown,  this);
    Bind(wxEVT_RIGHT_DOWN, &Canvas::OnMouseRightDown, this);
    Bind(wxEVT_LEFT_UP,    &Canvas::OnMouseLeftUp,    this);
    Bind(wxEVT_RIGHT_UP,   &Canvas::OnMouseRightUp,   this);
}


Canvas::~Canvas()
{
    Painter::DeInit();
}


void Canvas::OnPaint(wxPaintEvent &)
{
    wxPaintDC dc(this);
    dc.DrawBitmap(*Painter::GetBitmap(), 0, 0);
}


void Canvas::OnResize(wxSizeEvent &event) //-V2009
{
    Painter::OnResizeEvent(event);

    Refresh();
}


void Canvas::BeginScene()
{
    Painter::BegineScene();
    Selector::DrawRegion();
    Grid::Draw();
}

void Canvas::EndScene()
{
    Painter::EndScene();

    Refresh();
}


void Canvas::Draw()
{
    static clock_t time = 0;

    if(clock() - time > 1000)
    {
        needRedraw = true;
    }

    if(needRedraw)
    {
        BeginScene();

        Zoomer::Draw();

        TheForm->Draw();

        Selector::DrawCursors();

        EndScene();

        needRedraw = false;

        time = clock();
    }
}


void Canvas::Redraw()
{
    needRedraw = true;
}


void Canvas::OnMouseMove(wxMouseEvent &event) //-V2009
{
    event.GetPosition(&mouseX, &mouseY);

    if (mouseIsDown)
    {
        if (Zoomer::MoveWindow(mouseX))
        {
            SetMouseCursor();
            return;
        }
    }

    if (Zoomer::UnderMouse(mouseX, mouseY))
    {
        SetMouseCursor();
        return;
    }

    switch (ModeButtonLeft::Get())
    {
    case ModeButtonLeft::EditLines:
        {
            if (mouseIsDown)
            {
                TheForm->MoveMarker(mouseX, mouseY);
            }
    
            SetMouseCursor();
    
            Redraw();
        }
        break;
    case ModeButtonLeft::EditPoints:
        break;
    case ModeButtonLeft::SelectZone:
        if (mouseIsDown)
        {
            Selector::MoveBorder(mouseX);
            Redraw();
        }
        SetMouseCursor();
        break;
    }
}


void Canvas::OnMouseLeftDown(wxMouseEvent &event) //-V2009
{
    event.GetPosition(&mouseX, &mouseY);

    if (Zoomer::Grab(mouseX, mouseY))
    {
        mouseIsDown = true;
        SetMouseCursor();
        return;
    }

    switch (ModeButtonLeft::Get())
    {
    case ModeButtonLeft::EditLines:
        {
            if (TheForm->ExistMarker(mouseX, mouseY, false))         // Если в позиции мыши нахдится маркер
            {
                mouseIsDown = true;
            }
            else
            {
                TheForm->SetMarkerInMouseCoord(mouseX, mouseY);     // Если маркера нет - устанавливаем его
            }
            SetMouseCursor();
        }
        break;
    case ModeButtonLeft::EditPoints:
        break;
    case ModeButtonLeft::SelectZone:
        mouseIsDown = true;
        Selector::BeginGrab(mouseX);
        SetMouseCursor();
        Redraw();
        break;
    default:
        break;
    }

    event.Skip();
}


void Canvas::OnMouseRightDown(wxMouseEvent &event) //-V2009
{
    event.GetPosition(&mouseX, &mouseY);

    mouseIsDown = false;

    (static_cast<Frame* >(parent))->ShowContextMenu({ mouseX, mouseY });

    SetMouseCursor();
}


void Canvas::OnMouseRightUp(wxMouseEvent &event) //-V2009
{
    event.GetPosition(&mouseX, &mouseY);

    mouseIsDown = false;
    History::Add(TheForm);

    SetMouseCursor();
}


void Canvas::OnMouseLeftUp(wxMouseEvent &event)
{
    event.GetPosition(&mouseX, &mouseY);

    mouseIsDown = false;

    if (Zoomer::UnGrab())
    {
        return;
    }

    switch (ModeButtonLeft::Get())
    {
    case ModeButtonLeft::EditLines:
        break;

    case ModeButtonLeft::EditPoints:
        break;

    case ModeButtonLeft::SelectZone:
        Selector::EndGrab();
        Redraw();
        break;
    }

    SetMouseCursor();
}


void Canvas::SetMouseCursor()
{
    if (Zoomer::UnderMouse(mouseX, mouseY))
    {
        HCURSOR cursor = LoadCursor(NULL, IDC_HAND);
        ::SetCursor(cursor);
        ::ShowCursor(true);
        return;
    }

    switch (ModeButtonLeft::Get())
    {
    case ModeButtonLeft::EditPoints:
        break;

    case ModeButtonLeft::EditLines:
        if (TheForm->ExistMarker(mouseX, mouseY, mouseIsDown))
        {
            HCURSOR cursor = LoadCursor(NULL, IDC_HAND);
            ::SetCursor(cursor);
            ::ShowCursor(true);
        }
        break;

    case ModeButtonLeft::SelectZone:
        if (Selector::CursorOverBorder(mouseX))
        {
            HCURSOR cursor = LoadCursor(NULL, IDC_SIZEWE);
            ::SetCursor(cursor);
            ::ShowCursor(true);
        }
        break;
    }
}


int Grid::X()
{
    return 0;
}


int Grid::Y()
{
    return Zoomer::NoScaling() ? 0 : Zoomer::Height();
}


int Grid::Width()
{
    return TheCanvas->GetSize().x;
}


int Grid::Height()
{
    return Zoomer::NoScaling() ? TheCanvas->GetSize().y : (TheCanvas->GetSize().y - Zoomer::Height());
}


int Grid::Bottom()
{
    return Y() + Width();
}


int Grid::Right()
{
    return X() + Width();
}


void Grid::Draw()
{
    int width = Width();
    int height = Height();

    float stepX = static_cast<float>(width) / 20.0F;
    float stepY = static_cast<float>(height) / 20.0F;

    float x = stepX;
    float y = static_cast<float>(Y()) + stepY;

    for (int i = 0; i < 19; i++)
    {
        Painter::DrawLine(static_cast<int>(x + 0.5F), Y(), static_cast<int>(x + 0.5F), height + Y(), Color::GRAY_2F);
        Painter::DrawLine(0, static_cast<int>(y + 0.5F), width, static_cast<int>(y + 0.5F));

        x += stepX;
        y += stepY;
    }

    Painter::DrawLine(width / 2, Y(), width / 2, height + Y(), Color::GRAY_4F);
    Painter::DrawLine(0, height / 2 + Y(), width, height / 2 + Y());
}
