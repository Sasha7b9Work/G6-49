#include "defines.h"
#include "SpinControl.h"


SpinControl::SpinControl(wxWindow *window, wxWindowID id, const wxPoint &position, const wxSize &size, int min, int max, const wxString &initial,
    wxDialog *dlg, wxEventFunction handler, const wxString &label) :
    wxSpinCtrl(window, id, initial, position, size, wxSP_ARROW_KEYS, min, max)
{
    dlg->Connect(id, wxEVT_COMMAND_SPINCTRL_UPDATED, handler);
    new wxStaticText(window, wxID_ANY, label, { position.x + 55, position.y + 2 }, wxDefaultSize, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
}