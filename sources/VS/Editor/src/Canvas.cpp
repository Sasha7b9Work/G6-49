#include "Canvas.h"
#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)


Canvas *TheCanvas = nullptr;


wxButton *button = nullptr;


Canvas::Canvas(wxWindow *parent, int width, int height)
{
    button = new wxButton(parent, wxID_ANY, "", wxDefaultPosition, { width, height } );
    Resize({ width, height });

    wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);

    sizer->Add(button);

    parent->SetSizer(sizer);
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

