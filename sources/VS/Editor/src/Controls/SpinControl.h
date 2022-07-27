// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#pragma warning(push, 0)
#include <wx/dialog.h>
#include <wx/spinctrl.h>
#include <wx/wx.h>
#include <wx/event.h>
#pragma warning(pop)


class Dialog;


enum
{
    ID_SPINCTRL_DONW,
    ID_SPINCTRL_UP,
    ID_SPINCTRL_NUMBER_PERIODS,
    ID_BUTTON_OK,
    ID_BUTTON_CANCEL,
    ID_RADIOBUTTON_DIRECT,
    ID_RADIOBUTTON_BACK,
    ID_TEXT_CONTROL_POSITION,
    ID_TEXT_CONTROL_VALUE,
    ID_TEXT_CONTROL_POWER
};


class SpinControl : public wxSpinCtrl
{
public:
    SpinControl(wxWindow *window, wxWindowID id, const wxPoint &position, const wxSize &size, int min, int max, const int initial,
        wxDialog *dlg, wxEventFunction handler, const wxString &label, Dialog *dialog);

private:
    Dialog *dialog = nullptr;

    int prevValue = 0;

    void OnKeyUp(wxKeyEvent &);

    char buffer[100];
};
