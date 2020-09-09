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
#include <mutex>
#pragma warning(push, 0)
#include <wx/msw/private.h>
#pragma warning(pop)


bool Grid::typeIsPercents = true;
int Grid::deltaPoints = 1024;
int Grid::deltaPercents = 5000;
wxString Grid::sScale;


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
    Bind(wxEVT_MOUSEWHEEL, &Canvas::OnMouseWheel,     this);
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

        TheForm->Draw();

        Zoomer::Draw();

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
    }

    if (Selector::IsEnabled())
    {
        if (mouseIsDown)
        {
            Selector::MoveBorder(mouseX);
            Redraw();
        }
        SetMouseCursor();
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

    if (Selector::IsEnabled())
    {
        mouseIsDown = true;
        Selector::BeginGrab(mouseX);
        SetMouseCursor();
        Redraw();
    }
    else
    {
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
        }
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


void Canvas::OnMouseWheel(wxMouseEvent &event)
{
    if (event.m_wheelRotation > 0)
    {
        Zoomer::Increase();
    }
    else
    {
        Zoomer::Decrease();
    }
}


void Canvas::OnMouseLeftUp(wxMouseEvent &event)
{
    event.GetPosition(&mouseX, &mouseY);

    mouseIsDown = false;

    if (Zoomer::UnGrab())
    {
        return;
    }

    if (Selector::IsEnabled())
    {
        Selector::EndGrab();
        Redraw();
    }
    else if(ModeButtonLeft::Get() == ModeButtonLeft::EditLines)
    {

    }
    else if (ModeButtonLeft::Get() == ModeButtonLeft::EditPoints)
    {

    }

    SetMouseCursor();
}


void Canvas::SetMouseCursor()
{
    static std::mutex mutex;
    mutex.lock();

    if (Zoomer::UnderMouse(mouseX, mouseY))
    {
        HCURSOR cursor = LoadCursor(NULL, IDC_HAND);
        ::SetCursor(cursor);
        ::ShowCursor(true);
    }
    else if (Selector::IsEnabled())
    {
        if (Selector::CursorOverBorder(mouseX))
        {
            HCURSOR cursor = LoadCursor(NULL, IDC_SIZEWE);
            ::SetCursor(cursor);
            ::ShowCursor(true);
        }
    }
    else if (ModeButtonLeft::Get() == ModeButtonLeft::EditPoints)
    {

    }
    else if (ModeButtonLeft::Get() == ModeButtonLeft::EditLines)
    {
        if (TheForm->ExistMarker(mouseX, mouseY, mouseIsDown))
        {
            HCURSOR cursor = LoadCursor(NULL, IDC_HAND);
            ::SetCursor(cursor);
            ::ShowCursor(true);
        }
    }

    mutex.unlock();
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
    return Y() + Height();
}


int Grid::Right()
{
    return X() + Width();
}


void Grid::Draw()
{
    TypeIsPercents() ? DrawTypePercents() : DrawTypePoints();
    DrawHorizontalLines();
}


void Grid::DrawHorizontalLines()
{
    float stepY = Height() / 20.0F;

    for (int i = 1; i < 20; i++)
    {
        Painter::DrawHLine(X(), static_cast<int>(Y() + i * stepY + 0.5F), Right(), (i == 10) ? Color::GRAY_5F : Color::GRAY_2F);
    }
}


void Grid::DrawTypePercents()
{
    float stepX = Point::AMOUNT / static_cast<float>((100.0F / (static_cast<float>(deltaPercents) / 1000)));

    for (int i = 0; i < Point::AMOUNT / 2; i++)
    {
        Painter::DrawVLine(Point::FromData(Math::Round<uint16>(Point::AMOUNT / 2 + stepX * i), 0).CanvasX(), Grid::Y(), Grid::Bottom(), ((i % 10) == 0) ? Color::GRAY_5F : Color::GRAY_2F);
        Painter::DrawVLine(Point::FromData(Math::Round<uint16>(Point::AMOUNT / 2 - stepX * i), 0).CanvasX(), Grid::Y(), Grid::Bottom(), ((i % 10) == 0) ? Color::GRAY_5F : Color::GRAY_2F);
    }
}


void Grid::DrawTypePoints()
{
    for (int i = 0; i < Point::AMOUNT / 2 / deltaPoints; i++)
    {
        Painter::DrawVLine(Point::FromData(static_cast<uint16>(Point::AMOUNT / 2 + deltaPoints * i), 0).CanvasX(), Y(), Bottom(), ((i % 10) == 0) ? Color::GRAY_5F : Color::GRAY_2F);
        Painter::DrawVLine(Point::FromData(static_cast<uint16>(Point::AMOUNT / 2 - deltaPoints * i), 0).CanvasX(), Y(), Bottom(), ((i % 10) == 0) ? Color::GRAY_5F : Color::GRAY_2F);
    }
}


void Grid::CalculateDeltaPoints()
{
    int scale = Zoomer::Scale();

    if (scale < 200)        { deltaPoints = 512; }
    else if (scale < 300)   { deltaPoints = 512; }
    else if (scale < 500)   { deltaPoints = 256; }
    else if (scale < 1000)  { deltaPoints = 128; }
    else if (scale < 2000)  { deltaPoints = 64;  }
    else if (scale < 3000)  { deltaPoints = 32;  }
    else if (scale < 5000)  { deltaPoints = 32;  }
    else if (scale < 10000) { deltaPoints = 16;  }
    else                    { deltaPoints = 8;   }
}


void Grid::CalculateDeltaPercents()
{
    int scale = Zoomer::Scale();

    if (scale < 200)        { deltaPercents = 5000; }
    else if (scale < 300)   { deltaPercents = 2500; }
    else if (scale < 500)   { deltaPercents = 1250; }
    else if (scale < 1000)  { deltaPercents = 500;  }
    else if (scale < 2000)  { deltaPercents = 250;  }
    else if (scale < 3000)  { deltaPercents = 250;  }
    else if (scale < 5000)  { deltaPercents = 250;  }
    else if (scale < 10000) { deltaPercents = 250;  }
    else                    { deltaPercents = 250;  }
}


void Grid::CalculateScale()
{
    if (TypeIsPercents())
    {
        CalculateDeltaPercents();
        sScale = wxString::Format(wxT("%.2f%%"), deltaPercents / 1000.0F);
    }
    else
    {
        CalculateDeltaPoints();
        sScale = wxString::Format(wxT("%d т/div"), deltaPoints);
    }
}


wxString Grid::GetScale()
{
    CalculateScale();
    return sScale;
}


void Grid::ChangeTypeGrid()
{
    typeIsPercents = !typeIsPercents;
    TheFrame->UpdateStatusBar();
}
