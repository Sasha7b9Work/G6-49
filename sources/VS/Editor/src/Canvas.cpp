#include "defines.h"
#include "Canvas.h"
#include "Form.h"
#pragma warning(push, 0)
#include <SDL.h>
#include <wx/wx.h>
#pragma warning(pop)


Canvas *TheCanvas = nullptr;

static bool needRedraw = true;

static wxButton *button = nullptr;
static SDL_Renderer *renderer = nullptr;
static SDL_Texture *texture = nullptr;


static void DrawGrid();


Canvas::Canvas(wxWindow *parent, int width, int height)
{
    button = new wxButton(parent, wxID_ANY, "", wxDefaultPosition, { width, height } );
    Resize({ width, height });

    wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);

    sizer->Add(button);

    parent->SetSizer(sizer);

    SDL_Window *window = SDL_CreateWindowFrom(button->GetHandle());

    if (window == nullptr)
    {
        std::cout << "SDL_CreateWindowFrom() Error: " << SDL_GetError() << std::endl;
    }
    else
    {
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    }
}

Canvas::~Canvas()
{
    delete button;
}


void Canvas::Resize(const wxSize &size)
{
    if (button)
    {
        button->SetMaxSize(size);
        button->SetSize(size);
        button->SetMinSize(size);
    }

    needRedraw = true;
}


void Canvas::BeginScene()
{
    wxSize size = button->GetSize();

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_RENDERER_ACCELERATED, size.x, size.y);
    
    SDL_SetRenderTarget(renderer, texture);
    SetColor(Color::BLACK);
    SDL_RenderClear(renderer);

    DrawGrid();
}


void Canvas::SetPoint(int x, int y, const Color &color)
{
    SetColor(color);
    SDL_RenderDrawPoint(renderer, x, y);
}


void Canvas::SetPoint(int x, int y, int size, const Color &color)
{
    SetColor(color);

    SDL_Rect rect = { x - size / 2, y - size / 2, size, size };

    SDL_RenderFillRect(renderer, &rect);
}


void Canvas::DrawLine(int x0, int y0, int x1, int y1, const Color &color)
{
    SetColor(color);

    SDL_RenderDrawLine(renderer, x0, y0, x1, y1);
}


void Canvas::Draw()
{
    if(needRedraw)
    {
        BeginScene();

        TheForm->Draw();

        EndScene();

        needRedraw = false;
    }
}


void Canvas::EndScene()
{
    SDL_SetRenderTarget(renderer, NULL);

    wxSize size = button->GetSize();

    SDL_Rect rect = { 0, 0, size.x, size.y };

    if (texture)
    {
        SDL_RenderCopy(renderer, texture, NULL, &rect);
    }

    SDL_RenderPresent(renderer);
}


void Canvas::SetColor(const Color &color)
{
    if (color != Color::NUMBER)
    {
        uint value = COLOR(color.value);
        uint8 blue = static_cast<uint8>(value);
        uint8 green = static_cast<uint8>(value >> 8);
        uint8 red = static_cast<uint8>(value >> 16);
        SDL_SetRenderDrawColor(renderer, red, green, blue, 0x00);
    }
}


const wxSize Canvas::GetSize() const
{
    return button->GetSize();
}


static void DrawGrid()
{
    int width = TheCanvas->GetSize().x;
    int height = TheCanvas->GetSize().y;

    float stepX = width / 20.0F;
    float stepY = height / 20.0F;

    float x = stepX;
    float y = stepY;

    for (int i = 0; i < 19; i++)
    {
        TheCanvas->DrawLine(static_cast<int>(x + 0.5F), 0, static_cast<int>(x + 0.5F), height, Color::DARK_GREEN_1F);
        TheCanvas->DrawLine(0, static_cast<int>(y + 0.5F), width, static_cast<int>(y + 0.5F));

        x += stepX;
        y += stepY;
    }

    TheCanvas->DrawLine(width / 2, 0, width / 2, height, Color::DARK_GREEN_3F);
    TheCanvas->DrawLine(0, height / 2, width, height / 2);
}


void Canvas::Redraw()
{
    needRedraw = true;
}
