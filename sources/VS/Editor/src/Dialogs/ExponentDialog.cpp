#include "defines.h"
#include "Form.h"
#include "Utils.h"
#include "Dialogs/ExponentDialog.h"
#include "Controls/SpinControl.h"
#pragma warning(push, 0)
#include <wx/spinctrl.h>
#include <wx/statline.h>
#pragma warning(pop)


enum
{
    ID_TEXTCTRL_POWER
};


wxPanel *ExponentDialog::CreatePanelPower()
{
    wxPanel *panel = new wxPanel(this);

    new wxStaticBox(panel, wxID_ANY, wxT("Постоянная времени"), wxDefaultPosition, wxSize(130, 75));

    scPower = new SpinControl(panel, wxID_ANY, wxT(""), { 20, 20 }, { 100, 20 }, 0, std::numeric_limits<int>::max(), 2000, this, wxCommandEventHandler(Dialog::OnControlEvent), wxT(""));

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

    scPower->SetValue(1);
}


void ExponentDialog::SendAdditionForm()
{
    double y1 = Point::AVE * (1.0 - 1.0 / scPower->GetValue());

    double k = (std::log(Point::AVE) - std::log(y1)) / ((Point::NUM_POINTS - 1) - (Point::NUM_POINTS - 2));

    int start = static_cast<int>(std::log(Point::AVE) / k - (Point::NUM_POINTS - 1));

    for (int i = 0; i < Point::NUM_POINTS; i++)
    {
        double value = std::exp(k * (i + start));

        uint16 uValue = static_cast<uint16>(Point::AVE + value);

        uValue = Math::Limitation<uint16>(uValue, Point::MIN, Point::MAX);

        if (i == Point::NUM_POINTS - 1 || i == Point::NUM_POINTS / 2)
        {
            uValue = uValue;
        }

        data[i] = uValue;
    }

    TheForm->SetAdditionForm(data);

    points.clear();
}
