#include "defines.h"
#include "Form.h"
#include "Utils.h"
#include "Dialogs/ExponentDialog.h"
#include "Controls/SpinControl.h"
#include "Controls/TextControl.h"
#pragma warning(push, 0)
#include <wx/spinctrl.h>
#include <wx/statline.h>
#pragma warning(pop)
#include <vector>


enum
{
    ID_TEXTCTRL_POWER
};


static TextControlFloat *tcPower = nullptr;


static wxPanel *CreatePanelPower(Dialog *dialog)
{
    wxPanel *panel = new wxPanel(dialog);

    new wxStaticBox(panel, wxID_ANY, wxT("Степень"), wxDefaultPosition, wxSize(130, 75));
    tcPower = new TextControlFloat(dialog, 0, 1000000, panel, ID_TEXT_CONTROL_POWER, "1", { 20, 30 }, { 100, 20 });

    return panel;
}


ExponentDialog::ExponentDialog() : Dialog(wxT("Параметры экспоненциального сигнала"), true)
{
    wxBoxSizer *vBox = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *hBoxPanels = new wxBoxSizer(wxHORIZONTAL);

    hBoxPanels->Add(CreatePanelPolarity());
    hBoxPanels->AddStretchSpacer();
    hBoxPanels->Add(CreatePanelPower(this));
    vBox->Add(hBoxPanels);

    SetBoxSizer(vBox, { 221, 80 });
}


void ExponentDialog::SendAdditionForm()
{
    wxString stringValue = tcPower->GetValue();

    double power = std::atof(stringValue.c_str());

    double k = Point::AVE / std::log(power * (Point::NUM_POINTS - 1));

    for (int i = 0; i < Point::NUM_POINTS; i++)
    {
        double value = std::log(power * i);
        data[i] = Point::AVE + static_cast<uint16>(value * k);
    }

    TheForm->SetAdditionForm(data);

    points.clear();
}
