#include "defines.h"
#include "Controls/SpinControl.h"
#include "Dialogs/GaussDialog.h"
#include <cmath>


wxPanel *GaussDialog::CreatePanelWidth(wxDialog *dlg)
{
    wxPanel *panel = new wxPanel(dlg);

    int x = 40;
    int y = 20;

    scWidth = new SpinControl(panel, ID_SPINCTRL_NUMBER_PERIODS, wxPoint(x, y), wxSize(50, 20), 1, 1500, wxT("500"), this, wxCommandEventHandler(Dialog::OnControlEvent), wxT("Шмирина"));

    scWidth->SetValue(500);

    return panel;
}


GaussDialog::GaussDialog() : Dialog(wxT("Параметры гауссовой функции"), true)
{
    wxBoxSizer *vBox = new wxBoxSizer(wxVERTICAL);

    vBox->Add(CreatePanelWidth(this));

    vBox->AddSpacer(10);

    SetBoxSizer(vBox, { 200, 55 });
}


void GaussDialog::SendAdditionForm()
{
    float c = static_cast<float>(scWidth->GetValue());

    for (int i = 0; i < Point::NUM_POINTS / 2; i++)
    {
        uint16 g = static_cast<uint16>(Point::AVE + Point::AVE * std::expf(-static_cast<float>(i * i) / (2 * c * c)));

        if (g < Point::AVE)      { g = Point::AVE; }
        else if (g > Point::MAX) { g = Point::MAX; }

        data[Point::NUM_POINTS / 2 - i] = g;
        data[Point::NUM_POINTS / 2 + i] = g;
    }

    data[0] = data[1];

    TheForm->SetAdditionForm(data);
}
