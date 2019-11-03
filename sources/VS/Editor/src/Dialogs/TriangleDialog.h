#pragma once
#include "Dialogs/Dialog.h"


class TriangleDialog : public Dialog
{
public:
    TriangleDialog();

    void OnControlEvent(wxCommandEvent &);

    void OnButtonOk(wxCommandEvent &);

    void OnButtonCancel(wxCommandEvent &);
};
