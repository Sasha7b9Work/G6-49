#pragma once
#include "Dialogs/Dialog.h"


class TrapezeDialog : public Dialog
{
public:
    TrapezeDialog();

    void OnControlEvent(wxCommandEvent &);

    void OnButtonOk(wxCommandEvent &);

    void OnButtonCancel(wxCommandEvent &);
};
