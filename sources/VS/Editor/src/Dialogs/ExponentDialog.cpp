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


//static TextControl *tcPower = nullptr;


static wxPanel *CreatePanelPower(wxDialog *dialog)
{
    wxPanel *panel = new wxPanel(dialog);

    new wxStaticBox(panel, wxID_ANY, wxT("Степень"), wxDefaultPosition, wxSize(130, 75));
    //tcPower = new TextControl()

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
