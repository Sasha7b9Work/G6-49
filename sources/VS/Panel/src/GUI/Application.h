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
    void OnTimer(wxTimerEvent &);
    void OnSCPI(wxCommandEvent &);

private:
    wxTimer timer;

    void DrawFPS();
    void HandlerEvents();

public:
    void OnKeyDown(wxCommandEvent &);
    void OnKeyUp(wxCommandEvent &);
};
