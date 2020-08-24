#pragma once
#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)


class ParametersDialog : public wxDialog
{
public:
    ParametersDialog();
private:
    
    wxTextCtrl *tcPosition;      // Здесь будем вводить индекс точки

    wxTextCtrl *tcValue;         // Здесь будем вводить числовое значение точки

    void OnButtonApply(wxCommandEvent &);

    void OnButtonCancel(wxCommandEvent &);
};
