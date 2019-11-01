#pragma once
#pragma warning(push, 0)
#include <wx/dialog.h>
#include <wx/spinctrl.h>
#include <wx/wx.h>
#include <wx/event.h>
#pragma warning(pop)


class SpinControl : public wxSpinCtrl
{
public:
    SpinControl(wxWindow *window, wxWindowID id, const wxString &text, const wxPoint &position, const wxSize &size, int min, int max, int initial,
        wxDialog *dlg, wxEventFunction handler);
};
