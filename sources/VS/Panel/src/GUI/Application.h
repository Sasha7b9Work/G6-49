// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
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

    void OnKeyDown(wxCommandEvent &);
    void OnKeyUp(wxCommandEvent &);

private:
    wxTimer timer;

    void DrawFPS();
    void HandlerEvents();

    void OnQuit(wxCommandEvent &);
    void OnTimer(wxTimerEvent &);
    void OnSCPI(wxCommandEvent &);
    void OnClose(wxCloseEvent &);

};
