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

    void InvertPoint(uint16 *point);

    SpinControl *scPower = nullptr;
    wxRadioButton *rbType1 = nullptr;
    wxRadioButton *rbType2 = nullptr;
    wxRadioButton *rbType3 = nullptr;
    wxRadioButton *rbType4 = nullptr;
};
