#pragma once
#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)


class Dialog : public wxDialog
{
public:
    Dialog(const wxString &title, const wxSize &size);
    virtual ~Dialog();

private:
    void OnMove(wxMoveEvent &);
};
