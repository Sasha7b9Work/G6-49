#pragma once
#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)


class ParametersDialog : public wxDialog
{
public:
    ParametersDialog(uint16 index, uint16 value);
private:
    
    wxTextCtrl *tcPosition;      // Здесь будем вводить индекс точки

    wxTextCtrl *tcValue;         // Здесь будем вводить числовое значение точки

    void OnButtonApply(wxCommandEvent &);

    void OnButtonCancel(wxCommandEvent &);
};
