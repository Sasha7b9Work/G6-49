#include "defines.h"
#include "Canvas.h"
#include "Editor.h"
#include "Form.h"
#include "History.h"
#include <ctime>
#pragma warning(push, 0)
#include <wx/msw/private.h>
#pragma warning(pop)


Canvas *TheCanvas = nullptr;

static bool needRedraw = true;
/// Здесь рисуем
static wxBitmap *bitmapButton = nullptr;
/// Контекст рисования
static wxMemoryDC memDC;


Canvas::Canvas(wxWindow *p) : wxPanel(p, wxID_ANY), parent(p)
{
    bitmapButton = new wxBitmap(parent->GetClientSize());
    SetDoubleBuffered(true);
    Bind(wxEVT_PAINT,      &Canvas::OnPaint,          this);
    Bind(wxEVT_SIZE,       &Canvas::OnResize,         this);
    Bind(wxEVT_MOTION,     &Canvas::OnMouseMove,      this);
    Bind(wxEVT_LEFT_DOWN,  &Canvas::OnMouseLeftDown,  this);
    Bind(wxEVT_RIGHT_DOWN, &Canvas::OnMouseRightDown, this);
    Bind(wxEVT_LEFT_UP,    &Canvas::OnMouseUp,        this);
    Bind(wxEVT_RIGHT_UP,   &Canvas::OnMouseUp,        this);
}


Canvas::~Canvas()
{
    delete bitmapButton;
}


void Canvas::OnPaint(wxPaintEvent &)
{
    wxPaintDC dc(this);
    dc.DrawBitmap(*bitmapButton, 0, 0);
}


void Canvas::OnResize(wxSizeEvent &event) //-V2009
{
    delete bitmapButton;
    bitmapButton = new wxBitmap(event.GetSize());
    Refresh();
}


void Canvas::BeginScene()
{
    memDC.SelectObject(*bitmapButton);
    wxBrush brush(*wxBLACK, wxBRUSHSTYLE_SOLID);
    memDC.SetBackground(brush);
    memDC.Clear();
    DrawGrid();
}

void Canvas::EndScene()
{
    memDC.SelectObject(wxNullBitmap);
    Refresh();
}


void Canvas::SetPoint(int x, int y, const Color &color)
{
    SetColor(color);
    memDC.DrawPoint({ x, y });
}


void Canvas::SetPoint(int x, int y, int size, const Color &color)
{
    SetColor(color);

    x -= size / 2;
    y -= size / 2;

    memDC.DrawRectangle({ x, y, size, size });
}


void Canvas::DrawLine(int x0, int y0, int x1, int y1, const Color &color)
{
    SetColor(color);

    memDC.DrawLine(x0, y0, x1, y1);
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

        EndScene();

        needRedraw = false;

        time = clock();
    }
}

void Canvas::SetColor(const Color &color)
{
    if (color != Color::NUMBER)
    {
        uint value = COLOR(color.value);
        uint8 b = static_cast<uint8>(value);
        uint8 g = static_cast<uint8>(value >> 8);
        uint8 r = static_cast<uint8>(value >> 16);

        wxColour colorDraw = wxColour(r, g, b);

        memDC.SetPen(wxPen(colorDraw));
    }
}


void Canvas::DrawGrid()
{
    int width = GetSize().x;
    int height = GetSize().y;

    float stepX = width / 20.0F;
    float stepY = height / 20.0F;

    float x = stepX;
    float y = stepY;

    for (int i = 0; i < 19; i++)
    {
        DrawLine(static_cast<int>(x + 0.5F), 0, static_cast<int>(x + 0.5F), height, Color::DARK_GREEN_1F);
        DrawLine(0, static_cast<int>(y + 0.5F), width, static_cast<int>(y + 0.5F));

        x += stepX;
        y += stepY;
    }

    DrawLine(width / 2, 0, width / 2, height, Color::DARK_GREEN_3F);
    DrawLine(0, height / 2, width, height / 2);
}


void Canvas::Redraw()
{
    needRedraw = true;
}


void Canvas::OnMouseMove(wxMouseEvent &event) //-V2009
{
    event.GetPosition(&mouseX, &mouseY);
    
    if(mouseIsDown)
    {
        TheForm->MovePoint(mouseX, mouseY);
    }

    SetMouseCursor();

    Redraw();
}


void Canvas::OnMouseLeftDown(wxMouseEvent &event) //-V2009
{
    event.GetPosition(&mouseX, &mouseY);

    if(TheForm->ExistPoint(mouseX, mouseY, false))
    {
        mouseIsDown = true;
    }
    else
    {
        TheForm->SetPoint(mouseX, mouseY);
    }

    SetMouseCursor();
}


void Canvas::OnMouseRightDown(wxMouseEvent &event) //-V2009
{
    event.GetPosition(&mouseX, &mouseY);

    mouseIsDown = false;

    (static_cast<Frame* >(parent))->ShowContextMenu({ mouseX, mouseY }, TheForm->ExistPoint(mouseX, mouseY, false));

    SetMouseCursor();
}


void Canvas::OnMouseUp(wxMouseEvent &event)
{
    event.GetPosition(&mouseX, &mouseY);

    mouseIsDown = false;
    History::Add(TheForm);

    SetMouseCursor();
}


void Canvas::SetMouseCursor()
{
    if(TheForm->ExistPoint(mouseX, mouseY, mouseIsDown))
    {
        HCURSOR cursor = LoadCursor(NULL, IDC_HAND);
        ::SetCursor(cursor);
        ::ShowCursor(true);
    }
}
