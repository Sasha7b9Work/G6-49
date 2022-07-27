// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Dialog.h"


class SpinControl;


class SinXDialog : public Dialog
{
public:
    SinXDialog();

protected:

    virtual void SendAdditionForm() override;

    virtual void SaveValues() override;

    // ����� ����� �������� ���������� �������� sin(x) �� ������� �������
    SpinControl *scNumPeriods = nullptr;

    wxPanel *CreatePanelNumPeriods(wxDialog *dlg);
};

