#include "defines.h"
#include "Dialogs/ParametersDialog.h"
#include "Dialogs/SpinControl.h"


#define SMALL_BUTTON_SIZE wxSize(70, 20)

#define ADD_SPACER(hwd) hwd->AddSpacer(20);


ParametersDialog::ParametersDialog(uint16 index, uint16 value) : wxDialog(nullptr, wxID_ANY, wxT("Параметры точки"))
{
    wxPoint pos(50, 0);
    wxSize  size(50, 20);
    char buffer[100];

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    wxStaticText *stPosition = new wxStaticText(this, wxID_ANY, wxT("Индекс"));
    tcPosition = new wxTextCtrl(this, ID_TEXT_CONTROL_POSITION, _itoa(index, buffer, 10), pos, size);

    wxBoxSizer *hBoxPosition = new wxBoxSizer(wxHORIZONTAL);

    ADD_SPACER(hBoxPosition);
    hBoxPosition->Add(stPosition, 1, wxALIGN_CENTER);
    ADD_SPACER(hBoxPosition);
    hBoxPosition->Add(tcPosition, 1, wxALIGN_CENTER);
    ADD_SPACER(hBoxPosition);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    wxStaticText *stValue = new wxStaticText(this, wxID_ANY, wxT("Значение"));
    tcValue = new wxTextCtrl(this, ID_TEXT_CONTROL_VALUE, _itoa(value, buffer, 10), pos, size);

    wxBoxSizer *hBoxValue = new wxBoxSizer(wxHORIZONTAL);

    ADD_SPACER(hBoxValue);
    hBoxValue->Add(stValue, 1, wxALIGN_CENTER);
    ADD_SPACER(hBoxValue);
    hBoxValue->Add(tcValue, 1, wxALIGN_CENTER);
    ADD_SPACER(hBoxValue);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    wxButton *btnOk = new wxButton(this, ID_BUTTON_OK, wxT("Принять"), wxDefaultPosition, SMALL_BUTTON_SIZE);
    Connect(ID_BUTTON_OK, wxEVT_BUTTON, wxCommandEventHandler(ParametersDialog::OnButtonApply));

    wxButton *btnCancel = new wxButton(this, ID_BUTTON_CANCEL, wxT("Отменить"), wxDefaultPosition, SMALL_BUTTON_SIZE);
    Connect(ID_BUTTON_CANCEL, wxEVT_BUTTON, wxCommandEventHandler(ParametersDialog::OnButtonCancel));

    wxBoxSizer *hBoxButtons = new wxBoxSizer(wxHORIZONTAL);

    hBoxButtons->Add(btnOk, 1, wxALIGN_CENTER);
    hBoxButtons->AddSpacer(20);
    hBoxButtons->Add(btnCancel, 1, wxALIGN_CENTER);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    wxBoxSizer *vBox = new wxBoxSizer(wxVERTICAL);          // Главный бокс

    vBox->AddSpacer(10);
    vBox->Add(hBoxPosition, 0, wxALIGN_CENTER);
    vBox->AddSpacer(10);
    vBox->Add(hBoxValue, 0, wxALIGN_CENTER);
    vBox->AddSpacer(10);
    vBox->Add(hBoxButtons, 0, wxALIGN_CENTER);

    SetSizer(vBox);

    SetClientSize({ 170, 100 });

    Centre();

    SetFocus();
}


void ParametersDialog::OnButtonApply(wxCommandEvent &)
{

}


void ParametersDialog::OnButtonCancel(wxCommandEvent &)
{

}
