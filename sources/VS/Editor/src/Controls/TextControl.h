// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "defines.h"
#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)


class Dialog;


class TextControlInt : public wxTextCtrl
{
public:
    TextControlInt(int min, int max, wxDialog *dlg, wxWindowID id, const wxString &value, const wxPoint &position, const wxSize &size);
private:
    int min = 0;
    int max = 0;
    void OnKeyDown(wxKeyEvent &);
};


class TextControlFloat : public wxTextCtrl
{
public:
    TextControlFloat(Dialog *dlg, float min, float max, wxWindow *parent, wxWindowID id, const wxString &value, const wxPoint &position, const wxSize &size);
private:
    float min = 0.0F;
    float max = 0.0F;
    Dialog *dialog = nullptr;
    void OnKeyDown(wxKeyEvent &);
};
