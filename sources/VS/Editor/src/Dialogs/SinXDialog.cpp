// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Dialogs/SinXDialog.h"
#include "Controls/SpinControl.h"


static int numPeriods = 1;


wxPanel *SinXDialog::CreatePanelNumPeriods(wxDialog *dlg)
{
    wxPanel *panel = new wxPanel(dlg);

    int x = 40;
    int y = 20;

    scNumPeriods = new SpinControl(panel, ID_SPINCTRL_NUMBER_PERIODS, wxPoint(x, y), wxSize(50, 20), 1, 1000, numPeriods, this, wxCommandEventHandler(Dialog::OnControlEvent),
        wxT("Число периодов"), this);

    return panel;
}


SinXDialog::SinXDialog() : Dialog(wxT("Параметры сигнала sin(x)/x"), true)
{
    wxBoxSizer *vBox = new wxBoxSizer(wxVERTICAL);

    vBox->Add(CreatePanelNumPeriods(this));

    vBox->AddSpacer(10);

    SetBoxSizer(vBox, { 200, 55 });
}


void SinXDialog::SendAdditionForm()
{ 
    float T = (float)(Point::AMOUNT / 2) / ((float)scNumPeriods->GetValue() + 0.25F);

    int i0 = Point::AMOUNT / 2;

    for (int i = 0; i < Point::AMOUNT / 2; i++)
    {
        float x = (float)i / T * 2.0F * (float)M_PI;

        float sinx_x = std::sinf(x) / x;

        int curLess = i0 + i;
        int curMore = i0 - i;

        uint16 value = (uint16)(Point::AVE + sinx_x * Point::AVE);

        if (i == 0)
        {
            value = 4095;
        }

        data[curLess] = value;
        data[curMore] = value;
    }

    data[0] = data[1];

    TheForm->SetAdditionForm(data);

    points.clear();
}


void SinXDialog::SaveValues()
{
    numPeriods = scNumPeriods->GetValue();
}
