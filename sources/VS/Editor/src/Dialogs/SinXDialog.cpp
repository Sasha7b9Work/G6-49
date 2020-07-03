#include "defines.h"
#include "Dialogs/SinXDialog.h"
#include "Dialogs/SpinControl.h"


wxPanel *SinXDialog::CreatePanelNumPeriods(wxDialog *dlg)
{
    wxPanel *panel = new wxPanel(dlg);

    int x = 40;
    int y = 20;

    scNumPeriods = new SpinControl(panel, ID_SPINCTRL_NUMBER_PERIODS, wxT("3"), wxPoint(x, y), wxSize(50, 20), -100, 100, 100, this, wxCommandEventHandler(Dialog::OnControlEvent), wxT("Число периодов"));

    return panel;
}


SinXDialog::SinXDialog() : Dialog(wxT("Параметры сигнала sin(x)/x"))
{
    wxBoxSizer *vBox = new wxBoxSizer(wxVERTICAL);

    vBox->Add(CreatePanelNumPeriods(this));

    vBox->AddSpacer(10);

    SetBoxSizer(vBox, { 200, 55 });
}


void SinXDialog::SendAdditionForm()
{

}
