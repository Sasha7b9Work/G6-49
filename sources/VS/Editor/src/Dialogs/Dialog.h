#pragma once
#include "Form.h"
#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)


class Dialog : public wxDialog
{
public:
    Dialog(const wxString &title, const wxSize &size);
    virtual ~Dialog();

    void OnControlEvent(wxCommandEvent &);

protected:
    /// ѕослать форму дл€ ознакомительной отрисовки
    virtual void SendAdditionForm() {};

    void SetBoxSizer(wxBoxSizer *sizer, wxSize size);

    static uint16 data[Point::NUM_POINTS];

    static std::vector<Point> points;

private:
    wxBoxSizer *panelBox = nullptr;

    void OnMove(wxMoveEvent &);

    void OnButtonOk(wxCommandEvent &);

    void OnButtonCancel(wxCommandEvent &);
};
