#pragma once
#include "defines.h"
#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)


class TextControl : public wxTextCtrl
{
public:
    TextControl(wxDialog *dlg, wxWindowID id, const wxString &value, const wxPoint &position, const wxSize &size);
private:
};
