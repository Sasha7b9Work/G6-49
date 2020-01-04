#include "defines.h"
#include "GUI/GovernorGUI.h"


GovernorGUI::GovernorGUI(wxWindow *parent, const wxPoint &position) : wxPanel(parent, wxID_ANY, position)
{
    SetSize({ radius * 2 + 1, radius * 2 + 1 });
    SetDoubleBuffered(true);
    Bind(wxEVT_PAINT, &GovernorGUI::OnPaint, this);
}


void GovernorGUI::OnPaint(wxPaintEvent &)
{
    wxPaintDC dc(this);

    wxBrush brush({ 0, 0, 0 }, wxTRANSPARENT);
    dc.SetBrush(brush);
   
    dc.DrawCircle(radius, radius, radius);
}
