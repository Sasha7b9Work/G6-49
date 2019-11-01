#pragma once
#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)


class ExponentDialog : public wxDialog
{
public:
    ExponentDialog();
    ~ExponentDialog();

    void OnControlEvent(wxCommandEvent &);

    void OnButtonOk(wxCommandEvent &);

    void OnButtonCancel(wxCommandEvent &);
};
