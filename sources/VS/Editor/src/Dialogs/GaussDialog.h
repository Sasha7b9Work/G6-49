#pragma once
#include "Dialogs/Dialog.h"


class SpinControl;


class GaussDialog : public Dialog
{
public:
    GaussDialog();

protected:

    virtual void SendAdditionForm() override;

    virtual void SaveValues() override;

    SpinControl *scWidth = nullptr;

    wxPanel *CreatePanelWidth(wxDialog *dlg);
};
