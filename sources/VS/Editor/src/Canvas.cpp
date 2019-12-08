#include "defines.h"
#include "Canvas.h"
#include "Form.h"
#include <ctime>


Canvas *TheCanvas = nullptr;

static bool needRedraw = true;
/// Эту кнопку используем для рисования
static wxBitmap bitmapButton(640, 480);
/// Контекст рисования
static wxMemoryDC memDC;


Canvas::Canvas(wxWindow *parent) : wxPanel(parent)
{
    SetMinSize({ 640, 480 });
    SetDoubleBuffered(true);
    Bind(wxEVT_PAINT, &Canvas::OnPaint, this);

    //button = new wxButton(parent, wxID_ANY, "", wxDefaultPosition, { width, height } );
    //Resize({ width, height });
    //
    //wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
    //
    //sizer->Add(button);
    //
    //parent->SetSizer(sizer);

}


void Canvas::OnPaint(wxPaintEvent &)
{
    wxPaintDC dc(this);
    wxImage image = bitmapButton.ConvertToImage();
    image = image.Rescale(GetSize().x, GetSize().y);
    wxBitmap bitmap(image);
    dc.DrawBitmap(bitmap, 0, 0);
}


void Canvas::Resize(const wxSize &)
{
    //if (button)
    //{
    //    button->SetMaxSize(size);
    //    button->SetSize(size);
    //    button->SetMinSize(size);
    //}

    needRedraw = true;
}


void Canvas::BeginScene()
{
    //wxSize size = button->GetSize();
    //
    //texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_RENDERER_ACCELERATED, size.x, size.y);
    //
    //SDL_SetRenderTarget(renderer, texture);
    //SetColor(Color::BLACK);
    //SDL_RenderClear(renderer);
    //
    //DrawGrid();

    memDC.SelectObject(bitmapButton);
    wxBrush brush({ 0, 0, 0 }, wxBRUSHSTYLE_TRANSPARENT);
    memDC.SetBrush(brush);
    memDC.Clear();
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


void Canvas::EndScene()
{
    memDC.SelectObject(wxNullBitmap);
    Refresh();

    //SDL_SetRenderTarget(renderer, NULL);
    //
    //wxSize size = button->GetSize();
    //
    //SDL_Rect rect = { 0, 0, size.x, size.y };
    //
    //if (texture)
    //{
    //    SDL_RenderCopy(renderer, texture, NULL, &rect);
    //}
    //
    //SDL_RenderPresent(renderer);
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
