#include "defines.h"
#include "Dialogs/SinXDialog.h"
#include "Controls/SpinControl.h"


wxPanel *SinXDialog::CreatePanelNumPeriods(wxDialog *dlg)
{
    wxPanel *panel = new wxPanel(dlg);

    int x = 40;
    int y = 20;

    scNumPeriods = new SpinControl(panel, ID_SPINCTRL_NUMBER_PERIODS, wxPoint(x, y), wxSize(50, 20), 1, 100, wxT("1"), this, wxCommandEventHandler(Dialog::OnControlEvent),
        wxT("Число периодов"));

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
    float T = static_cast<float>(Point::NUM_POINTS / 2) / (static_cast<float>(scNumPeriods->GetValue()) + 0.25F);

    int i0 = Point::NUM_POINTS / 2;

    for (int i = 0; i < Point::NUM_POINTS / 2; i++)
    {
        float x = static_cast<float>(i) / T * 2.0F * static_cast<float>(M_PI);

        float sinx_x = std::sinf(x) / x;

        int curLess = i0 + i;
        int curMore = i0 - i;

        uint16 value = static_cast<uint16>(Point::AVE + sinx_x * Point::AVE);

        if (i == 0)
        {
            value = 4095;
        }

        data[curLess] = value;
        data[curMore] = value;
    }

    data[0] = data[1];

    TheForm->SetAdditionForm(data);
}
