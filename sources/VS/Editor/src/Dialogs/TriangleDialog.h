#pragma once
#include "Dialogs/Dialog.h"


class SpinControl;


class TriangleDialog : public Dialog
{
public:
    TriangleDialog();

protected:

    virtual void SendAdditionForm() override;

    virtual void SaveValues() override;

    wxPanel *CreatePanelOffsets();

    SpinControl *scCenter = nullptr;
    SpinControl *scDelay = nullptr;
};
