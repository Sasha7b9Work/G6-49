#include "defines.h"
#include "Form.h"
#include "Utils.h"
#include "Dialogs/ExponentDialog.h"
#include "Controls/SpinControl.h"
#pragma warning(push, 0)
#include <wx/spinctrl.h>
#include <wx/statline.h>
#pragma warning(pop)
#include <vector>


enum
{
    ID_TEXTCTRL_POWER
};


wxPanel *ExponentDialog::CreatePanelPower()
{
    wxPanel *panel = new wxPanel(this);

    new wxStaticBox(panel, wxID_ANY, wxT("Постоянная времени"), wxDefaultPosition, wxSize(130, 75));

    scPower = new SpinControl(panel, wxID_ANY, wxT(""), { 20, 20 }, { 100, 20 }, 0, 8191, 2000, this, wxCommandEventHandler(Dialog::OnControlEvent), wxT(""));

    return panel;
}


ExponentDialog::ExponentDialog() : Dialog(wxT("Параметры экспоненциального сигнала"), true)
{
    wxBoxSizer *vBox = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *hBoxPanels = new wxBoxSizer(wxHORIZONTAL);

    hBoxPanels->Add(CreatePanelPolarity());
    hBoxPanels->AddStretchSpacer();
    hBoxPanels->Add(CreatePanelPower());
    vBox->Add(hBoxPanels);

    SetBoxSizer(vBox, { 221, 80 });
}


void ExponentDialog::SendAdditionForm()
{
    double power = scPower->GetValue();

    for (int i = 0; i < Point::NUM_POINTS; i++)
    {
        double value = std::exp(power * i);

        value = Math::Limitation<double>(value, Point::MIN, Point::MAX);
    }
}

/*
void ExponentDialog::SendAdditionForm()
{
    double power = scPower->GetValue();

    int first = 0;

    if (std::log(power * 1e-3) < 0.0)
    {
        while (std::log(power * first) < 0.0)
        {
            first++;

            if (first > Point::NUM_POINTS)
            {
                return;
            }
        }
    }

    double k = Point::AVE / std::log(power * (Point::NUM_POINTS - 1 + first));

    for (int i = 0; i < Point::NUM_POINTS; i++)
    {
        double value = std::log(power * (i + first));
        if (value < 0.0)
        {
            value = 0.0;
        }

        if (rbPolarityDirect->GetValue())
        {
            data[i] = static_cast<uint16>(Point::AVE + static_cast<uint16>(value * k));
        }
        else
        {
            data[i] = static_cast<uint16>(Point::MAX - static_cast<uint16>(value * k));
        }
    }

    TheForm->SetAdditionForm(data);

    points.clear();
}
*/
