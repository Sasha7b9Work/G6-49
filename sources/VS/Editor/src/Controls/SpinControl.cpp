// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/SpinControl.h"
#include "Dialogs/Dialog.h"
#include <wx/event.h>


SpinControl::SpinControl(wxWindow *window, wxWindowID id, const wxPoint &position, const wxSize &size, int min, int max, const int initial,
    wxDialog *dlg, wxEventFunction handler, const wxString &label, Dialog *_dialog) :
    wxSpinCtrl(window, id, _itoa(initial, buffer, 10), position, size, wxSP_ARROW_KEYS, min, max),
    dialog(_dialog)
{
    dlg->Connect(id, wxEVT_COMMAND_SPINCTRL_UPDATED, handler);
    
    Bind(wxEVT_KEY_UP, &SpinControl::OnKeyUp, this);
   
    new wxStaticText(window, wxID_ANY, label, { position.x + 55, position.y + 2 }, wxDefaultSize, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);

    prevValue = wxSpinCtrl::GetValue();
}


void SpinControl::OnKeyUp(wxKeyEvent &event)
{
    if (dialog)
    {
        int value = GetValue();

        if (value < GetMin() || value > GetMax())
        {
            SetValue(prevValue);
            event.Skip(false);
        }
        else
        {
            prevValue = value;
            dialog->OnKeyUp(event);
        }
    }
}
