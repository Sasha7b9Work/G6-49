#pragma once
#pragma warning(push, 0)
#include "wx/wx.h"
#pragma warning(pop)


class Application : public wxApp
{
public:
    virtual bool OnInit() wxOVERRIDE;
};


class Frame : public wxFrame
{
public:
    Frame(const wxString &title);

    void OnQuit(wxCommandEvent &);
    void OnTimer(wxTimerEvent &);
    void OnResize(wxSizeEvent &);

private:
    wxTimer timer;
    wxDECLARE_EVENT_TABLE(); //-V2504

    void HandlerEvents();

    void SetSizeAndPosition();

    wxRect GetMaxDisplay();

    void DrawFPS();

    void ShowContextMenu(const wxPoint &pos, bool underPoint);

    void CreateMenu();

    void OnDeletePoint(wxCommandEvent &);

    void OnClearForm(wxCommandEvent &);
};
