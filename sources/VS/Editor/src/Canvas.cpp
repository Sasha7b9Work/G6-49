#include "Canvas.h"
#pragma warning(push, 0)
#include <SDL.h>
#include <wx/wx.h>
#pragma warning(pop)


Canvas *TheCanvas = nullptr;


wxButton *button = nullptr;

static SDL_Renderer *renderer = nullptr;
static SDL_Texture *texture = nullptr;


Canvas::Canvas(wxWindow *parent, int width, int height)
{
    button = new wxButton(parent, wxID_ANY, "", wxDefaultPosition, { width, height } );
    Resize({ width, height });

    wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);

    sizer->Add(button);

    parent->SetSizer(sizer);

    SDL_Window *window = SDL_CreateWindowFrom((HANDLE)button->GetHandle());

    if (window == nullptr)
    {
        std::cout << "SDL_CreateWindowFrom() Error: " << SDL_GetError() << std::endl;
    }
    else
    {
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    }
}


void Canvas::Resize(const wxSize &size)
{
    if (button)
    {
        button->SetMaxSize(size);
        button->SetSize(size);
        button->SetMinSize(size);
    }
}


void Canvas::BeginScene()
{
    wxSize size = button->GetSize();

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_RENDERER_ACCELERATED, size.x, size.y);
    
    SDL_SetRenderTarget(renderer, texture);
    SetColor(Color::BLACK);
    SDL_RenderClear(renderer);
}


void Canvas::EndScene()
{
    SDL_SetRenderTarget(renderer, NULL);

    wxSize size = button->GetSize();

    SDL_Rect rect = { 0, 0, size.x, size.y };

    SDL_RenderCopy(renderer, texture, NULL, &rect);

    SDL_RenderPresent(renderer);
}


void Canvas::SetColor(Color &color)
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

