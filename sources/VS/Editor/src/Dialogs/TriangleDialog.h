#pragma once
#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)


class TriangleDialog : public wxDialog
{
public:
    TriangleDialog();
    ~TriangleDialog();

    void OnControlEvent(wxCommandEvent &);

    void OnButtonOk(wxCommandEvent &);

    void OnButtonCancel(wxCommandEvent &);
};
