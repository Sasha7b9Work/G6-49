#include "Dialogs/TextControl.h"


TextControl::TextControl(int _min, int _max, wxDialog *dlg, wxWindowID id, const wxString &value, const wxPoint &position, const wxSize &size) : wxTextCtrl(dlg, id, value, position, size),
    min(_min), max(_max)
{
    Bind(wxEVT_KEY_DOWN, &TextControl::OnKeyDown, this);
}


void TextControl::OnKeyDown(wxKeyEvent &event)
{
    int code = event.GetKeyCode();

    if (code >= '0' && code <= '9')
    {
        char buffer[50];
        std::strcpy(buffer, GetValue().c_str());
        buffer[std::strlen(buffer)] = static_cast<char>(code);
        buffer[std::strlen(buffer) + 1] = static_cast<char>(code);
        int value = atoi(buffer);

        event.Skip(value >= min && value <= max);
    }
    else if (code == WXK_RETURN || code == WXK_BACK || code == WXK_LEFT || code == WXK_RIGHT || code == WXK_TAB || code == WXK_DELETE || code == WXK_INSERT)
    {
        event.Skip(true);
    }
    else
    {
        event.Skip(false);
    }
}
