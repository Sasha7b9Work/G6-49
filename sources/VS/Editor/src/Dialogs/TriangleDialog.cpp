#include "defines.h"
#include "Form.h"
#include "TriangleDialog.h"
#pragma warning(push, 0)
#include <wx/spinctrl.h>
#include <wx/statline.h>
#pragma warning(pop)


enum
{
    ID_SPINCTRL_DONW,
    ID_SPINCTRL_UP,
    ID_RADIOBUTTON_DIRECT,
    ID_RADIOBUTTON_BACK,
    ID_BUTTON_OK,
    ID_BUTTON_CANCEL
};



static wxRadioButton *rbDirect = nullptr;
static wxRadioButton *rbBack = nullptr;
static wxSpinCtrl *scUp = nullptr;
static wxSpinCtrl *scDown = nullptr;


/// Послать форму для ознакомительной отрисовки
static void SendForm();

static uint16 data[Point::NUM_POINTS];


static wxPanel *CreatePanelPolarity(wxDialog *dlg)
{
    wxPanel *panel = new wxPanel(dlg);
    wxStaticBox *sb = new wxStaticBox(panel, wxID_ANY, wxT("Полярность"), wxDefaultPosition, wxSize(90, 75));

    int y = 25;
    int x = 5;

    rbDirect = new wxRadioButton(panel, ID_RADIOBUTTON_DIRECT, wxT("Прямая"), wxPoint(x, y));
    dlg->Connect(ID_RADIOBUTTON_DIRECT, wxEVT_RADIOBUTTON, wxCommandEventHandler(TriangleDialog::OnControlEvent));
    rbDirect->SetValue(true);

    rbBack = new wxRadioButton(panel, ID_RADIOBUTTON_BACK, wxT("Обратная"), wxPoint(x, y + 25));
    dlg->Connect(ID_RADIOBUTTON_BACK, wxEVT_RADIOBUTTON, wxCommandEventHandler(TriangleDialog::OnControlEvent));

    sb = sb;

    return panel;
}


static wxPanel *CreatePanelLevels(wxDialog *dlg)
{
    wxPanel *panel = new wxPanel(dlg);

    wxStaticBox *sb = new wxStaticBox(panel, wxID_ANY, wxT("Уровни"), wxDefaultPosition, wxSize(125, 75));

    int y = 20;
    int x = 10;

    int dY = 2;
    int dX = 55;

    scUp = new wxSpinCtrl(panel, ID_SPINCTRL_UP, wxT("100"), wxPoint(x, y), wxSize(50, 20), wxSP_ARROW_KEYS, -100, 100, 100);
    dlg->Connect(ID_SPINCTRL_UP, wxEVT_COMMAND_SPINCTRL_UPDATED, wxCommandEventHandler(TriangleDialog::OnControlEvent));
    wxStaticText *textUp = new wxStaticText(panel, wxID_ANY, wxT("Верхний"), wxPoint(x + dX, y + dY), wxDefaultSize, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
    scDown = new wxSpinCtrl(panel, ID_SPINCTRL_DONW, wxT("-100"), wxPoint(x, y + 26), wxSize(50, 20), wxSP_ARROW_KEYS, -100, 100, -100);
    dlg->Connect(ID_SPINCTRL_DONW, wxEVT_COMMAND_SPINCTRL_UPDATED, wxCommandEventHandler(TriangleDialog::OnControlEvent));
    wxStaticText *textDown = new wxStaticText(panel, wxID_ANY, wxT("Нижний"), wxPoint(x + dX, y + 26 + dY), wxDefaultSize, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);

    sb = sb;
    textUp = textUp;
    textDown = textDown;

    return panel;
}


TriangleDialog::TriangleDialog() : wxDialog(nullptr, -1, wxT("Параметры треугольного сигнала"), wxDefaultPosition, wxSize(225, 135))
{
    wxButton *btnOk = new wxButton(this, ID_BUTTON_OK, wxT("Ok"), wxDefaultPosition, BUTTON_SIZE);
    Connect(ID_BUTTON_OK, wxEVT_BUTTON, wxCommandEventHandler(TriangleDialog::OnButtonOk));
    wxButton *btnClose = new wxButton(this, ID_BUTTON_CANCEL, wxT("Отмена"), wxDefaultPosition, BUTTON_SIZE);
    Connect(ID_BUTTON_CANCEL, wxEVT_BUTTON, wxCommandEventHandler(TriangleDialog::OnButtonCancel));

    wxBoxSizer *vBox = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *hBoxButtons = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *hBoxPanels = new wxBoxSizer(wxHORIZONTAL);

    hBoxPanels->Add(CreatePanelPolarity(this));
    hBoxPanels->AddStretchSpacer();
    hBoxPanels->Add(CreatePanelLevels(this));
    hBoxButtons->Add(btnOk);
    hBoxButtons->Add(btnClose);
    vBox->Add(hBoxPanels);
    vBox->Add(hBoxButtons);
    
    SetSizer(vBox);
    
    Centre();

    SendForm();
}


TriangleDialog::~TriangleDialog()
{
    TheForm->SetAdditionForm(nullptr);
}


static void DrawLine(int x1, int y1, int x2, int y2)
{
    float dX = static_cast<float>(x2 - x1);

    float dY = std::fabsf(static_cast<float>(y2 - y1));

    float k = dY / dX;

    if (y2 > y1)
    {
        for (int x = x1; x <= x2; x++)
        {
            data[x] = static_cast<uint16>(y1 + (x - x1) * k + 0.5F);
        }
    }
    else
    {       
        for (int x = x1; x <= x2; x++)
        {
            data[x] = static_cast<uint16>(y1 - (x - x1) * k + 0.5F);
        }
    }
}


static void SendForm()
{
    int center = Point::NUM_POINTS / 2;

    int levelHI = static_cast<int>(Point::AVE_VALUE - (Point::MAX_VALUE + Point::MIN_VALUE) / 2.0F * scUp->GetValue() / 100.0F);
    int levelLOW = static_cast<int>(Point::AVE_VALUE - (Point::MAX_VALUE + Point::MIN_VALUE) / 2.0F * scDown->GetValue() / 100.0F);

    int min = levelLOW;
    int max = levelHI;

    if (rbBack->GetValue())
    {
        min = levelHI;
        max = levelLOW;
    }

    DrawLine(0, min, center, max);

    DrawLine(center, max, Point::NUM_POINTS - 1, min);

    TheForm->SetAdditionForm(data);
}


void TriangleDialog::OnControlEvent(wxCommandEvent &)
{
    SendForm();
}


void TriangleDialog::OnButtonOk(wxCommandEvent &)
{
    TheForm->SetMainForm(data);
    Destroy();
}


void TriangleDialog::OnButtonCancel(wxCommandEvent &)
{
    Destroy();
}
