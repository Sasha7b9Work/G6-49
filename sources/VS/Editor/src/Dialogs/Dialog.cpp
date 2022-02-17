#include "defines.h"
#include "Controls/SpinControl.h"
#include "Dialogs/Dialog.h"
#include "Editor/Editor.h"
#include "Editor/Form.h"
#include "Editor/Painter/Canvas.h"


Dialog::Dialog(const wxString &title, bool blockingCanvas) : wxDialog(nullptr, wxID_ANY, title), isBlockingCanvas(blockingCanvas)
{
    Connect(wxEVT_MOVE, wxMoveEventHandler(Dialog::OnMove));

    wxButton *btnOk = new wxButton(this, ID_BUTTON_OK, wxT("�������"), wxDefaultPosition, BUTTON_SIZE);
    Connect(ID_BUTTON_OK, wxEVT_BUTTON, wxCommandEventHandler(Dialog::OnButtonApply));
    wxButton *btnCancel = new wxButton(this, ID_BUTTON_CANCEL, wxT("��������"), wxDefaultPosition, BUTTON_SIZE);
    Connect(ID_BUTTON_CANCEL, wxEVT_BUTTON, wxCommandEventHandler(Dialog::OnButtonCancel));

    Bind(wxEVT_KEY_UP, &Dialog::OnKeyUp, this);

    Connect(wxEVT_KEY_UP, wxKeyEventHandler(Dialog::OnKeyUp));

    wxBoxSizer *vBox = new wxBoxSizer(wxVERTICAL);
    panelBox = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *hBox = new wxBoxSizer(wxHORIZONTAL);

    vBox->Add(panelBox);
    hBox->Add(btnOk, 1, wxALIGN_CENTER);
    hBox->AddSpacer(20);
    hBox->Add(btnCancel, 1, wxALIGN_CENTER);
    vBox->AddSpacer(10);
    vBox->Add(hBox, 0, wxALIGN_CENTER);

    SetSizer(vBox);
}


Dialog::~Dialog()
{
    TheForm->SetAdditionForm(nullptr);
}


wxPanel *Dialog::CreatePanelLevels(int levelUp, int levelDown)
{
    wxPanel *panel = new wxPanel(this);

    new wxStaticBox(panel, wxID_ANY, wxT("������"), wxDefaultPosition, wxSize(130, 75));

    int y = 20, x = 10;

    scLevelUp = new SpinControl(panel, ID_SPINCTRL_UP, wxPoint(x, y), wxSize(51, 20), -100, 100, levelUp, this, wxCommandEventHandler(Dialog::OnControlEvent), wxT("�������, %"), this);
    scLevelDown = new SpinControl(panel, ID_SPINCTRL_DONW, wxPoint(x, y + 26), wxSize(51, 20), -100, 100, levelDown, this, wxCommandEventHandler(Dialog::OnControlEvent), wxT("������, %"), this);

    return panel;
}


wxPanel *Dialog::CreatePanelPolarity(bool polarityDirect, bool polarityBack)
{
    wxPanel *panel = new wxPanel(this);
    new wxStaticBox(panel, wxID_ANY, wxT("����������"), wxDefaultPosition, wxSize(90, 75));

    int y = 25, x = 5;

    rbPolarityDirect = new wxRadioButton(panel, ID_RADIOBUTTON_DIRECT, wxT("������"), wxPoint(x, y));
    Connect(ID_RADIOBUTTON_DIRECT, wxEVT_RADIOBUTTON, wxCommandEventHandler(Dialog::OnControlEvent));
    rbPolarityDirect->SetValue(true);

    rbPolarityBack = new wxRadioButton(panel, ID_RADIOBUTTON_BACK, wxT("��������"), wxPoint(x, y + 25));
    Connect(ID_RADIOBUTTON_BACK, wxEVT_RADIOBUTTON, wxCommandEventHandler(Dialog::OnControlEvent));

    rbPolarityDirect->SetValue(polarityDirect);
    rbPolarityBack->SetValue(polarityBack);

    return panel;
}


void Dialog::DrawLine(int x1, int y1, int x2, int y2)
{
    float dX = static_cast<float>(x2 - x1);

    float dY = std::fabsf(static_cast<float>(y2 - y1));

    float k = dY / dX;

    if(y2 > y1)
    {
        for(int x = x1; x <= x2; x++)
        {
            data[x] = static_cast<uint16>(static_cast<float>(y1) + static_cast<float>(x - x1) * k + 0.5F);
        }
    }
    else
    {
        for(int x = x1; x <= x2; x++)
        {
            data[x] = static_cast<uint16>(static_cast<float>(y1) - static_cast<float>(x - x1) * k + 0.5F);
        }
    }
}


void Dialog::SetBoxSizer(wxBoxSizer *sizer, wxSize size)
{
    panelBox->Add(sizer);

    size.y += 50;

    SetClientSize(size);

    Centre();

    SendAdditionForm();

    SetFocus();
}


void Dialog::OnMove(wxMoveEvent &)
{
    TheCanvas->Redraw();
}


void Dialog::OnButtonApply(wxCommandEvent &)
{
    TheFrame->SetBlockingCanvas(isBlockingCanvas);

    TheForm->SetMainForm(data, &points);

    SaveValues();

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


void Dialog::OnKeyUp(wxKeyEvent &event)
{
    SendAdditionForm();

    event.Skip();
}
