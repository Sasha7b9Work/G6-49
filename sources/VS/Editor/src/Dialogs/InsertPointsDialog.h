// (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#pragma warning(push, 0)
    #include <wx/wx.h>
#pragma warning(pop)


class InsertPointsDialog : public wxDialog
{
public:
    InsertPointsDialog();

    void OnButtonOk(wxCommandEvent &);

    void OnButtonCancel(wxCommandEvent &);

    void OnControlNumPoints(wxCommandEvent &);
};
