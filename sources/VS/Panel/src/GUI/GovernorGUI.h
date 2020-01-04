#pragma once
#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)


class GovernorGUI : public wxPanel
{
public:
    GovernorGUI(wxWindow *parent, const wxPoint &position);
private:
    const int radius = 20;

    bool leftIsDown = false;

    POINT positionDown;
    /// ”гол поворота ручки
    float angle = 0.0F;

    void OnPaint(wxPaintEvent &);
    void OnMouseMove(wxMouseEvent &);
    void OnMouseLeftDown(wxMouseEvent &);
    void OnMouseLeftUp(wxMouseEvent &);
    void OnMouseLeaveEnter(wxMouseEvent &);
    /// ”становить курсор в виде руки
    void SetMouseCursorHand();
    /// ¬озвращает true, если курсор мыши находитс€ над изображением ручки
    bool MouseOnGovernor(wxMouseEvent &);
};
