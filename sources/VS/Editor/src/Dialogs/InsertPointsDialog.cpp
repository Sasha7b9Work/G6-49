// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/SpinControl.h"
#include "Dialogs/InsertPointsDialog.h"
#include "Editor/Form.h"
#include "Editor/History.h"

#pragma warning(push, 0)
#include <wx/spinctrl.h>
#include <wx/statline.h>
#pragma warning(pop)


enum
{
    ID_SPINCTRL_DELTA
};


static SpinControl *scDelta = nullptr;


InsertPointsDialog::InsertPointsDialog() : wxDialog(nullptr, -1, wxT("Вставить маркеры"), wxDefaultPosition, wxSize(225, 185))
{
    wxButton *btnOk = new wxButton(this, ID_BUTTON_OK, wxT("Ok"), wxDefaultPosition, BUTTON_SIZE);
    Connect(ID_BUTTON_OK, wxEVT_BUTTON, wxCommandEventHandler(InsertPointsDialog::OnButtonOk));

    wxButton *btnClose = new wxButton(this, ID_BUTTON_CANCEL, wxT("Отмена"), wxDefaultPosition, BUTTON_SIZE);
    Connect(ID_BUTTON_CANCEL, wxEVT_BUTTON, wxCommandEventHandler(InsertPointsDialog::OnButtonCancel));

    scDelta = new SpinControl(this, ID_SPINCTRL_DELTA, wxDefaultPosition, wxSize(50, 20), 1, Point::AMOUNT / 2, 512,
        this, wxCommandEventHandler(InsertPointsDialog::OnControlNumPoints), wxT("Расстояние между точками"), nullptr);

    wxBoxSizer *vBox = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *hBoxButtons = new wxBoxSizer(wxHORIZONTAL);

    hBoxButtons->Add(btnOk);
    hBoxButtons->Add(btnClose);
    vBox->AddSpacer(10);
    vBox->Add(scDelta);
    vBox->AddSpacer(10);
    vBox->Add(hBoxButtons);

    SetSizer(vBox);

    Centre();
}


void InsertPointsDialog::OnControlNumPoints(wxCommandEvent &)
{

}


void InsertPointsDialog::OnButtonOk(wxCommandEvent &)
{
    for(uint16 i = 0; i < Point::AMOUNT; i += (uint16)scDelta->GetValue())
    {
        TheForm->SetPointInPosition(i);
    }

    History::Add(TheForm);

    Destroy();
}


void InsertPointsDialog::OnButtonCancel(wxCommandEvent &)
{
    Destroy();
}
