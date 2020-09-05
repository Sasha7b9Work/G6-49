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

    // Сместить точки вверх до упора
    void ShiftToUp();

    wxRadioButton *rbType1 = nullptr;
    wxRadioButton *rbType2 = nullptr;
    wxRadioButton *rbType3 = nullptr;
    wxRadioButton *rbType4 = nullptr;
};
