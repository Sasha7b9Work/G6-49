#pragma once
#include "Dialogs/Dialog.h"


class SpinControl;


class ExponentDialog : public Dialog
{
public:
    ExponentDialog();

protected:

    virtual void SendAdditionForm() override;

    wxPanel *CreatePanelPower();

    wxPanel *CreatePanelType();

    SpinControl *scPower = nullptr;
};
