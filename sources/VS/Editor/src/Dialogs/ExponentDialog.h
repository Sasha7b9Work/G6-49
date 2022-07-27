// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Dialogs/Dialog.h"


class SpinControl;


class ExponentDialog : public Dialog
{
public:
    ExponentDialog();

protected:

    virtual void SendAdditionForm() override;

    virtual void SaveValues() override;

    wxPanel *CreatePanelPower();

    wxPanel *CreatePanelType();

    void InvertPoint(uint16 *point);

    // Сместить точки вверх до упора
    void ShiftToUp();

    SpinControl *scPower = nullptr;
    wxRadioButton *rbType1 = nullptr;
    wxRadioButton *rbType2 = nullptr;
    wxRadioButton *rbType3 = nullptr;
    wxRadioButton *rbType4 = nullptr;
};
