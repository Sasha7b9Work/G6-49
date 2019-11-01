#pragma once
#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)


enum
{
    ID_SPINCTRL_DONW,
    ID_SPINCTRL_UP,
    ID_RADIOBUTTON_DIRECT,
    ID_RADIOBUTTON_BACK
};


class TriangleDialog : public wxDialog
{
public:
    TriangleDialog();
    ~TriangleDialog();

    /// Послать форму
    static void SendForm();
};
