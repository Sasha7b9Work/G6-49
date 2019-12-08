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
    Frame(const wxString& title);

    void OnQuit(wxCommandEvent &);
    void OnAbout(wxCommandEvent &);
    void OnTimer(wxTimerEvent &);

private:
    wxTimer timer;
    wxDECLARE_EVENT_TABLE(); //-V2504

    void DrawFPS();
    void HandlerEvents();

public:
    void OnKeyDown(wxCommandEvent &);
    void OnKeyUp(wxCommandEvent &);
};
