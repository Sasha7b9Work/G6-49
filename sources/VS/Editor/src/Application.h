#pragma once
#include "wx/wx.h"


class Application : public wxApp
{
public:
    virtual bool OnInit() wxOVERRIDE;
};


class Frame : public wxFrame
{
public:
    Frame(const wxString &title);

private:
    wxDECLARE_EVENT_TABLE();
};
