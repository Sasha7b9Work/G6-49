// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Dialogs/Dialog.h"


class SpinControl;


class TrapezeDialog : public Dialog
{
public:
    TrapezeDialog();

protected:
    
    virtual void SendAdditionForm() override;

    virtual void SaveValues() override;

    wxPanel *CreatePanelOffsets();

    SpinControl *scDelay = nullptr;
    SpinControl *scVertex1 = nullptr;
    SpinControl *scVertex2 = nullptr;
};
