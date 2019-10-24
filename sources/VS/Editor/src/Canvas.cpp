#include "Canvas.h"
#pragma warning(push, 0)
#include <SDL.h>
#include <wx/wx.h>
#pragma warning(pop)


Canvas *TheCanvas = nullptr;


wxButton *button = nullptr;

static SDL_Window *window = nullptr;

SDL_Renderer *renderer = nullptr;


Canvas::Canvas(wxWindow *parent, int width, int height)
{
    button = new wxButton(parent, wxID_ANY, "", wxDefaultPosition, { width, height } );
    Resize({ width, height });

    wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);

    sizer->Add(button);

    parent->SetSizer(sizer);

    window = SDL_CreateWindowFrom((HANDLE)button->GetHandle());

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

}


void Canvas::EndScene()
{

}

