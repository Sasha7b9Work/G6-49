#include "defines.h"
#include "Canvas.h"
#include "Editor.h"
#include "Form.h"
#include "Dialogs/Dialog.h"
#include "Dialogs/SpinControl.h"


enum
{
    ID_SPINCTRL_DONW,
    ID_SPINCTRL_UP,
    ID_BUTTON_OK,
    ID_BUTTON_CANCEL,
    ID_RADIOBUTTON_DIRECT,
    ID_RADIOBUTTON_BACK
};


uint16 Dialog::data[Point::NUM_POINTS];

std::vector<Point> Dialog::points;


Dialog::Dialog(const wxString &title) : wxDialog(nullptr, wxID_ANY, title)
{
    Connect(wxEVT_MOVE, wxMoveEventHandler(Dialog::OnMove));

    wxButton *btnOk = new wxButton(this, ID_BUTTON_OK, wxT("Принять"), wxDefaultPosition, BUTTON_SIZE);
    Connect(ID_BUTTON_OK, wxEVT_BUTTON, wxCommandEventHandler(Dialog::OnButtonApply));
    wxButton *btnCancel = new wxButton(this, ID_BUTTON_CANCEL, wxT("Отменить"), wxDefaultPosition, BUTTON_SIZE);
    Connect(ID_BUTTON_CANCEL, wxEVT_BUTTON, wxCommandEventHandler(Dialog::OnButtonCancel));

    Bind(wxEVT_KEY_DOWN, &Dialog::OnKeyDown, this);
    Bind(wxEVT_KEY_UP, &Dialog::OnKeyDown, this);

    Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(Dialog::OnKeyDown));
    Connect(wxEVT_KEY_UP, wxKeyEventHandler(Dialog::OnKeyDown));

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


wxPanel *Dialog::CreatePanelLevels()
{
    wxPanel *panel = new wxPanel(this);

    new wxStaticBox(panel, wxID_ANY, wxT("Уровни"), wxDefaultPosition, wxSize(130, 75));

    int y = 20, x = 10;

    scLevelUp = new SpinControl(panel, ID_SPINCTRL_UP, wxT("100"), wxPoint(x, y), wxSize(50, 20), -100, 100, 100, this, wxCommandEventHandler(Dialog::OnControlEvent), wxT("Верхний, %"));
    scLevelDown = new SpinControl(panel, ID_SPINCTRL_DONW, wxT("-100"), wxPoint(x, y + 26), wxSize(50, 20), -100, 100, -100, this, wxCommandEventHandler(Dialog::OnControlEvent), wxT("Нижний, %"));

    return panel;
}


wxPanel *Dialog::CreatePanelPolarity()
{
    wxPanel *panel = new wxPanel(this);
    new wxStaticBox(panel, wxID_ANY, wxT("Полярность"), wxDefaultPosition, wxSize(90, 75));

    int y = 25, x = 5;

    rbPolarityDirect = new wxRadioButton(panel, ID_RADIOBUTTON_DIRECT, wxT("Прямая"), wxPoint(x, y));
    Connect(ID_RADIOBUTTON_DIRECT, wxEVT_RADIOBUTTON, wxCommandEventHandler(Dialog::OnControlEvent));
    rbPolarityDirect->SetValue(true);

    rbPolarityBack = new wxRadioButton(panel, ID_RADIOBUTTON_BACK, wxT("Обратная"), wxPoint(x, y + 25));
    Connect(ID_RADIOBUTTON_BACK, wxEVT_RADIOBUTTON, wxCommandEventHandler(Dialog::OnControlEvent));

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
            data[x] = static_cast<uint16>(y1 + (x - x1) * k + 0.5F);
        }
    }
    else
    {
        for(int x = x1; x <= x2; x++)
        {
            data[x] = static_cast<uint16>(y1 - (x - x1) * k + 0.5F);
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


void Dialog::OnKeyDown(wxKeyEvent &event)
{
    std::cout << event.m_keyCode << std::endl;
}
