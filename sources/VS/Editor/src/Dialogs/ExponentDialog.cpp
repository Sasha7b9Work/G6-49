#include "defines.h"
#include "Form.h"
#include "Dialogs/ExponentDialog.h"
#include "Dialogs/SpinControl.h"
#pragma warning(push, 0)
#include <wx/spinctrl.h>
#include <wx/statline.h>
#pragma warning(pop)
#include <vector>


enum
{
    ID_SPINCTRL_DONW,
    ID_SPINCTRL_UP,
    ID_SPINCTRL_FRONT_DELAY,
    ID_SPINCTRL_FRONT_TIME,
    ID_SPINCTRL_FRONT_K,
    ID_RADIOBUTTON_DIRECT,
    ID_RADIOBUTTON_BACK,
    ID_BUTTON_OK,
    ID_BUTTON_CANCEL
};


static wxStaticBox *sb = nullptr;
static wxStaticText *text = nullptr;

static wxRadioButton *rbDirect = nullptr;
static wxRadioButton *rbBack = nullptr;
static SpinControl *scUp = nullptr;
static SpinControl *scDown = nullptr;

static SpinControl *scFrontDelay = nullptr;      // Задержка перед началом экспоненциального импульса
static SpinControl *scFrontTime = nullptr;       // Время от начала экспоненциального импульса до начала спада
static wxTextCtrl *tcFrontK = nullptr;          // Коэффициент экспоненты


/// Послать форму для ознакомительной отрисовки
static void SendForm();

static uint16 data[Point::NUM_POINTS];


static std::vector<Point> points;


static wxPanel *CreatePanelPolarity(wxDialog *dlg)
{
    wxPanel *panel = new wxPanel(dlg);
    sb = new wxStaticBox(panel, wxID_ANY, wxT("Полярность"), wxDefaultPosition, wxSize(90, 75));

    int y = 25, x = 5;

    rbDirect = new wxRadioButton(panel, ID_RADIOBUTTON_DIRECT, wxT("Прямая"), wxPoint(x, y));
    dlg->Connect(ID_RADIOBUTTON_DIRECT, wxEVT_RADIOBUTTON, wxCommandEventHandler(ExponentDialog::OnControlEvent));
    rbDirect->SetValue(true);

    rbBack = new wxRadioButton(panel, ID_RADIOBUTTON_BACK, wxT("Обратная"), wxPoint(x, y + 25));
    dlg->Connect(ID_RADIOBUTTON_BACK, wxEVT_RADIOBUTTON, wxCommandEventHandler(ExponentDialog::OnControlEvent));

    return panel;
}


static wxPanel *CreatePanelLevels(wxDialog *dlg)
{
    wxPanel *panel = new wxPanel(dlg);

    sb = new wxStaticBox(panel, wxID_ANY, wxT("Уровни"), wxDefaultPosition, wxSize(125, 75));

    int y = 20, x = 10;

    scUp = new SpinControl(panel, ID_SPINCTRL_UP, wxT("100"), wxPoint(x, y), wxSize(50, 20), -100, 100, 100, dlg, wxCommandEventHandler(ExponentDialog::OnControlEvent), wxT("Верхний"));
    scDown = new SpinControl(panel, ID_SPINCTRL_DONW, wxT("-100"), wxPoint(x, y + 26), wxSize(50, 20), -100, 100, -100, dlg, wxCommandEventHandler(ExponentDialog::OnControlEvent), wxT("Нижний"));

    return panel;
}


static wxPanel *CreatePanelFront(wxDialog *dlg)
{
    wxPanel *panel = new wxPanel(dlg);

    sb = new wxStaticBox(panel, wxID_ANY, wxT("Нарастание"), wxDefaultPosition, wxSize(125, 75));

    int y = 20, x = 10;

    scFrontDelay = new SpinControl(panel, ID_SPINCTRL_FRONT_DELAY, wxT("-100"), wxPoint(x, y), wxSize(50, 20), -100, 100, -100, dlg, wxCommandEventHandler(ExponentDialog::OnControlEvent), wxT("Задержка"));

    return panel;
}


ExponentDialog::ExponentDialog() : wxDialog(nullptr, -1, wxT("Параметры треугольного сигнала"), wxDefaultPosition, wxSize(225, 200))
{
    wxButton *btnOk = new wxButton(this, ID_BUTTON_OK, wxT("Ok"), wxDefaultPosition, BUTTON_SIZE);
    Connect(ID_BUTTON_OK, wxEVT_BUTTON, wxCommandEventHandler(ExponentDialog::OnButtonOk));
    wxButton *btnClose = new wxButton(this, ID_BUTTON_CANCEL, wxT("Отмена"), wxDefaultPosition, BUTTON_SIZE);
    Connect(ID_BUTTON_CANCEL, wxEVT_BUTTON, wxCommandEventHandler(ExponentDialog::OnButtonCancel));

    wxBoxSizer *vBox = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *hBoxButtons = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *hBoxPanels = new wxBoxSizer(wxHORIZONTAL);

    hBoxPanels->Add(CreatePanelPolarity(this));
    hBoxPanels->AddStretchSpacer();
    hBoxPanels->Add(CreatePanelLevels(this));
    hBoxButtons->Add(btnOk);
    hBoxButtons->Add(btnClose);
    vBox->Add(hBoxPanels);
    vBox->Add(CreatePanelFront(this));
    vBox->Add(hBoxButtons);
    
    SetSizer(vBox);
    
    Centre();

    SendForm();
}


ExponentDialog::~ExponentDialog()
{
    TheForm->SetAdditionForm(nullptr);
}


static void SendForm()
{

}


void ExponentDialog::OnControlEvent(wxCommandEvent &)
{
    SendForm();
}


void ExponentDialog::OnButtonOk(wxCommandEvent &)
{
    TheForm->SetMainForm(data);
    
    for (Point point : points)
    {
        TheForm->SetPoint(point);
    }

    Destroy();
}


void ExponentDialog::OnButtonCancel(wxCommandEvent &)
{
    Destroy();
}
