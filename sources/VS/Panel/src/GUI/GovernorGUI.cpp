#include "defines.h"
#include "GUI/GovernorGUI.h"


//#define SHOW_NAME_FUNC()   std::cout << __FUNCTION__ << std::endl


GovernorGUI::GovernorGUI(wxWindow *parent, const wxPoint &position) : wxPanel(parent, wxID_ANY, position), timer(this, 1)
{
    angle = static_cast<float>(rand());

    cursor = { false, {0, 0}, 0 };

    SetSize({ radius * 2 + 1, radius * 2 + 1 });
    SetDoubleBuffered(true);
    Bind(wxEVT_PAINT, &GovernorGUI::OnPaint, this);
    Bind(wxEVT_LEFT_DOWN, &GovernorGUI::OnMouseLeftDown, this);
    Bind(wxEVT_MOTION, &GovernorGUI::OnMouseMove, this);
    Bind(wxEVT_TIMER, &GovernorGUI::OnTimer, this);

    timer.Start(0);
}


void GovernorGUI::OnPaint(wxPaintEvent &)
{
    wxPaintDC dc(this);

    wxBrush brush({ 0, 0, 0 }, wxTRANSPARENT);
    dc.SetBrush(brush);
   
    dc.DrawCircle(radius, radius, radius);

    float r = radius * 0.6F;

    float x = radius + Sin(angle) * r;
    float y = radius + Cos(angle) * r;

    dc.DrawCircle(static_cast<int>(x), static_cast<int>(y), radius / 5);
}


void GovernorGUI::OnMouseLeftDown(wxMouseEvent &event)
{
    if(MouseOnGovernor(event))
    {
        cursor.leftIsDown = true;

        ::GetCursorPos(&cursor.position);

        cursor.state = ::GetKeyState(VK_LBUTTON);

        ::SetCursor(LoadCursor(NULL, IDC_HAND));
    }
}


void GovernorGUI::OnMouseMove(wxMouseEvent &event)
{
    if(MouseOnGovernor(event))
    {
        ::SetCursor(LoadCursor(NULL, IDC_HAND));
    }
}


bool GovernorGUI::MouseOnGovernor(wxMouseEvent &event) //-V2009
{
    int mouseX = 0;
    int mouseY = 0;

    event.GetPosition(&mouseX, &mouseY);

    return radius * radius >= (mouseX - radius) * (mouseX - radius) + (mouseY - radius) * (mouseY - radius);
}


float GovernorGUI::Sin(float grad)
{
    return sinf(grad * 3.1415926F / 180.0F);
}


float GovernorGUI::Cos(float grad)
{
    return cosf(grad * 3.1415926F / 180.0F);
}


void GovernorGUI::OnTimer(wxTimerEvent &)
{
    if(!cursor.LeftIsDown())
    {
        return;
    }

    ::SetCursor(LoadCursor(NULL, IDC_HAND));

    POINT position;

    if(cursor.PositionEqualsCurrent(&position))
    {
        return;
    }

    cursor.CalculateDeltas(&position);

    angle += 2.5F * (cursor.dY - cursor.dX);

    cursor.position = position;

    Refresh();
}


bool GovernorGUI::StructCursor::LeftIsDown()
{
    if(::GetKeyState(VK_LBUTTON) != state)
    {
        leftIsDown = false;
    }

    return leftIsDown;
}


bool GovernorGUI::StructCursor::PositionEqualsCurrent(POINT *newPosition)
{
    ::GetCursorPos(newPosition);

    return (newPosition->x == position.x) && (newPosition->y == position.y);
}


void GovernorGUI::StructCursor::CalculateDeltas(POINT *newPosition)
{
    dX = newPosition->x - position.x;
    dY = newPosition->y - position.y;
}
