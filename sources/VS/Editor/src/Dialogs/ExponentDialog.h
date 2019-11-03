#pragma once
#include "Dialog.h"


class ExponentDialog : public Dialog
{
public:
    ExponentDialog();

    void OnControlEvent(wxCommandEvent &);

    void OnButtonOk(wxCommandEvent &);

    void OnButtonCancel(wxCommandEvent &);
};
