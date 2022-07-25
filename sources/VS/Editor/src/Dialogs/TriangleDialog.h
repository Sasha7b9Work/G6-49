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

private:

    static int center;
    static int delay;
    static bool polarityDirect;
    static bool polarityBack;
    static int levelUp;
    static int levelDown;
};
