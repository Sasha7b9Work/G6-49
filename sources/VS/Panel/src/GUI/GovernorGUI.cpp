#include "defines.h"
#include "GUI/GovernorGUI.h"


GovernorGUI::GovernorGUI(wxWindow *parent, const wxPoint &position) : wxPanel(parent, wxID_ANY, position)
{
    SetSize({ radius * 2 + 1, radius * 2 + 1 });
    SetDoubleBuffered(true);
    Bind(wxEVT_PAINT, &GovernorGUI::OnPaint, this);
    Bind(wxEVT_MOTION, &GovernorGUI::OnMouseMove, this);
    Bind(wxEVT_LEFT_DOWN, &GovernorGUI::OnMouseLeftDown, this);
    Bind(wxEVT_LEFT_UP, &GovernorGUI::OnMouseLeftUp, this);
    Bind(wxEVT_LEAVE_WINDOW, &GovernorGUI::OnMouseLeave, this);
}


void GovernorGUI::OnPaint(wxPaintEvent &)
{
    wxPaintDC dc(this);

    wxBrush brush({ 0, 0, 0 }, wxTRANSPARENT);
    dc.SetBrush(brush);
   
    dc.DrawCircle(radius, radius, radius);
}


void GovernorGUI::OnMouseMove(wxMouseEvent &event)
{
    if(leftIsDown)
    {
        ::SetCursorPos(positionDown.x, positionDown.y);
    }

    if(leftIsDown || MouseOnGovernor(event))
    {
        SetMouseCursorHand();
    }
}


void GovernorGUI::OnMouseLeftDown(wxMouseEvent &event)
{
    if(MouseOnGovernor(event))
    {
        leftIsDown = true;

        ::GetCursorPos(&positionDown);

        SetMouseCursorHand();
    }
}


void GovernorGUI::OnMouseLeftUp(wxMouseEvent &)
{
    leftIsDown = false;
    SetMouseCursorHand();
}


void GovernorGUI::OnMouseLeave(wxMouseEvent &)
{
    if(leftIsDown)
    {
        ::SetCursorPos(positionDown.x, positionDown.y);

        SetMouseCursorHand();
    }
}


void GovernorGUI::SetMouseCursorHand()
{
    HCURSOR cursor = LoadCursor(NULL, IDC_HAND);

    ::SetCursor(cursor);
    ::ShowCursor(true);
}


bool GovernorGUI::MouseOnGovernor(wxMouseEvent &event)
{
    int mouseX = 0;
    int mouseY = 0;

    event.GetPosition(&mouseX, &mouseY);

    return radius * radius >= (mouseX - radius) * (mouseX - radius) + (mouseY - radius) * (mouseY - radius);
}
