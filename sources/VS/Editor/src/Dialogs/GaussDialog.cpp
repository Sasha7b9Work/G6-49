// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/SpinControl.h"
#include "Dialogs/GaussDialog.h"
#include <cmath>


static int width = 500;


wxPanel *GaussDialog::CreatePanelWidth(wxDialog *dlg)
{
    wxPanel *panel = new wxPanel(dlg);

    int x = 40;
    int y = 20;

    scWidth = new SpinControl(panel, ID_SPINCTRL_NUMBER_PERIODS, wxPoint(x, y), wxSize(50, 20), 1, 1500, width, this, wxCommandEventHandler(Dialog::OnControlEvent), wxT("Шмирина"), this);

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

    for (int i = 0; i < Point::AMOUNT / 2; i++)
    {
        uint16 g = static_cast<uint16>(Point::AVE + Point::AVE * std::expf(-static_cast<float>(i * i) / (2 * c * c)));

        if (g < Point::AVE)      { g = Point::AVE; }
        else if (g > Point::MAX) { g = Point::MAX; }

        data[Point::AMOUNT / 2 - i] = g;
        data[Point::AMOUNT / 2 + i] = g;
    }

    data[0] = data[1];

    TheForm->SetAdditionForm(data);

    points.clear();
}


void GaussDialog::SaveValues()
{
    width = scWidth->GetValue();
}
