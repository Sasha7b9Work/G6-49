#include "defines.h"
#include "Canvas.h"
#include "Dialog.h"
#include "Form.h"


enum
{
    ID_BUTTON_OK,
    ID_BUTTON_CANCEL
};


uint16 Dialog::data[Point::NUM_POINTS];

std::vector<Point> Dialog::points;


Dialog::Dialog(const wxString &title, const wxSize &) : wxDialog(nullptr, wxID_ANY, title)
{
    Connect(wxEVT_MOVE, wxMoveEventHandler(Dialog::OnMove));

    wxButton *btnOk = new wxButton(this, ID_BUTTON_OK, wxT("Принять"), wxDefaultPosition, BUTTON_SIZE);
    Connect(ID_BUTTON_OK, wxEVT_BUTTON, wxCommandEventHandler(Dialog::OnButtonOk));
    wxButton *btnCancel = new wxButton(this, ID_BUTTON_CANCEL, wxT("Отменить"), wxDefaultPosition, BUTTON_SIZE);
    Connect(ID_BUTTON_CANCEL, wxEVT_BUTTON, wxCommandEventHandler(Dialog::OnButtonCancel));

    wxBoxSizer *vBox = new wxBoxSizer(wxVERTICAL);
    panelBox = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *hBox = new wxBoxSizer(wxHORIZONTAL);

    vBox->Add(panelBox);
    hBox->Add(btnOk, 1, wxALIGN_CENTER);
    hBox->AddSpacer(20);
    hBox->Add(btnCancel, 1, wxALIGN_CENTER);
    vBox->AddSpacer(10);
    vBox->Add(hBox, 0, wxALIGN_CENTER | wxRIGHT | wxBOTTOM, 10);

    SetSizer(vBox);
}


Dialog::~Dialog()
{
    TheForm->SetAdditionForm(nullptr);
}


void Dialog::SetBoxSizer(wxBoxSizer *sizer, wxSize size)
{
    panelBox->Add(sizer);

    size.y += 50;

    SetClientSize(size);

    Centre();

    SendAdditionForm();
}


void Dialog::OnMove(wxMoveEvent &)
{
    TheCanvas->Redraw();
}


void Dialog::OnButtonOk(wxCommandEvent &)
{
    TheForm->SetMainForm(data, &points);

    Destroy();
}


void Dialog::OnButtonCancel(wxCommandEvent &)
{
    Destroy();
}


void Dialog::OnControlEvent(wxCommandEvent &)
{
    SendAdditionForm();
}
