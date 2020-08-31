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


static wxPanel *CreatePanelPower(wxDialog *dialog)
{
    wxPanel *panel = new wxPanel(dialog);

    new wxStaticBox(panel, wxID_ANY, wxT("Степень"), wxDefaultPosition, wxSize(130, 75));
    tcPower = new TextControlFloat(0, 8 * 1024, panel, ID_TEXT_CONTROL_POWER, "1", { 20, 30 }, { 50, 20 });

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
    TheForm->SetAdditionForm(data);

    points.clear();
}
