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
    /// Угол поворота ручки
    float angle = 0.0F;

    wxTimer timer;

    struct StructCursor
    {
        /// true, если левая кнопка нажата
        bool leftIsDown;
        /// Позиция курсора
        POINT position;
        /// Состояние VK_LBUTTON
        int state;

        int dX;
        int dY;
        /// Возвращает true, если position совпадает с текущими координатами курсора
        bool PositionEqualsCurrent(POINT *newPosition);
        /// Рассчитывает dX и dY между position и newPosition
        void CalculateDeltas(POINT *newPosition);
    } cursor;

    void OnPaint(wxPaintEvent &);
    void OnMouseLeftDown(wxMouseEvent &);
    void OnMouseMove(wxMouseEvent &);
    void OnTimer(wxTimerEvent &);

    /// Возвращает true, если курсор мыши находится над изображением ручки
    bool MouseOnGovernor(wxMouseEvent &);

    float Sin(float grad);
    float Cos(float grad);
};
