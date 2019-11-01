#include "defines.h"
#include "SpinControl.h"


SpinControl::SpinControl(wxWindow *window, wxWindowID id, const wxString &text, const wxPoint &position, const wxSize &size, int min, int max, int initial,
    wxDialog *dlg, wxEventFunction handler) :
    wxSpinCtrl(window, id, text, position, size, wxSP_ARROW_KEYS, min, max, initial)
{
    dlg->Connect(id, wxEVT_COMMAND_SPINCTRL_UPDATED, handler);
}