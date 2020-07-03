#pragma once
#include "Dialog.h"


class SpinControl;


class SinXDialog : public Dialog
{
public:
    SinXDialog();

protected:

    virtual void SendAdditionForm() override;

    // Здесь будем задавать количество периодов sin(x) на периоде сигнала
    SpinControl *scNumPeriods = nullptr;

    wxPanel *CreatePanelNumPeriods(wxDialog *dlg);
};

