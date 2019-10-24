#include "Canvas.h"
#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)


Canvas *TheCanvas = nullptr;


Canvas::Canvas(wxWindow *parent, int width, int height)
{
    wxButton *button = new wxButton(parent, wxID_ANY, "", { 10, 10 }, { 320, 240 });
    button->SetMaxSize({ width, height });

    wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);

    sizer->Add(button);

    parent->SetSizer(sizer);
}


void Canvas::BeginScene()
{

}


void Canvas::EndScene()
{

}

