#include "defines.h"
#include "Form.h"
#include "Dialogs/ExponentDialog.h"
#include "Dialogs/SpinControl.h"
#pragma warning(push, 0)
#include <wx/spinctrl.h>
#include <wx/statline.h>
#pragma warning(pop)
#include <vector>
#include <iomanip>
#include <sstream>


enum
{
    ID_SPINCTRL_DONW,
    ID_SPINCTRL_UP,
    ID_SPINCTRL_FRONT_DELAY,
    ID_SPINCTRL_FRONT_TIME,
    ID_TEXTCTRL_FRONT_K,
    ID_TEXTCTRL_BACK_K,
    ID_RADIOBUTTON_DIRECT,
    ID_RADIOBUTTON_BACK,
    ID_BUTTON_OK,
    ID_BUTTON_CANCEL
};


static double oldFrontK = 1.0;
static double oldBackK = 1.0;


static wxRadioButton *rbDirect = nullptr;
static wxRadioButton *rbBack = nullptr;
static SpinControl *scUp = nullptr;
static SpinControl *scDown = nullptr;

static SpinControl *scDelay = nullptr;          // Задержка перед началом экспоненциального импульса
static SpinControl *scFrontTime = nullptr;      // Время от начала экспоненциального импульса до начала спада
static wxTextCtrl *tcFrontK = nullptr;          // Коэффициент экспоненты
static wxTextCtrl *tcBackK = nullptr;


/// Послать форму для ознакомительной отрисовки
static void SendAdditionForm();

static uint16 data[Point::NUM_POINTS];


static std::vector<Point> points;


static wxPanel *CreatePanelPolarity(wxDialog *dlg)
{
    wxPanel *panel = new wxPanel(dlg);
    new wxStaticBox(panel, wxID_ANY, wxT("Полярность"), wxDefaultPosition, wxSize(90, 75));

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

    new wxStaticBox(panel, wxID_ANY, wxT("Уровни"), wxDefaultPosition, wxSize(125, 75));

    int y = 20, x = 10;

    scUp = new SpinControl(panel, ID_SPINCTRL_UP, wxT("100"), wxPoint(x, y), wxSize(50, 20), -100, 100, 100, dlg, wxCommandEventHandler(ExponentDialog::OnControlEvent), wxT("Верхний, %"));
    scDown = new SpinControl(panel, ID_SPINCTRL_DONW, wxT("-100"), wxPoint(x, y + 26), wxSize(50, 20), -100, 100, -100, dlg, wxCommandEventHandler(ExponentDialog::OnControlEvent), wxT("Нижний, %"));

    return panel;
}


static wxPanel *CreatePanelParameters(wxDialog *dlg)
{
    wxPanel *panel = new wxPanel(dlg);

    new wxStaticBox(panel, wxID_ANY, wxT("Параметры"), wxDefaultPosition, wxSize(125, 75 + 26 * 2));

    int y = 20, x = 10, dY = 26;

    scDelay = new SpinControl(panel, ID_SPINCTRL_FRONT_DELAY, wxT("0"), wxPoint(x, y), wxSize(50, 20), 0, Point::NUM_POINTS, 0, dlg, wxCommandEventHandler(ExponentDialog::OnControlEvent), wxT("Задержка, точки"));
    scFrontTime = new SpinControl(panel, ID_SPINCTRL_FRONT_TIME, wxT("4095"), wxPoint(x, y + dY), wxSize(50, 20), 0, Point::NUM_POINTS, Point::NUM_POINTS / 2, dlg, wxCommandEventHandler(ExponentDialog::OnControlEvent), wxT("Время нарастания, точки"));

    std::stringstream streamFront;
    streamFront << std::fixed << std::setprecision(5) << oldFrontK;
    tcFrontK = new wxTextCtrl(panel, ID_TEXTCTRL_FRONT_K, streamFront.str().c_str(), wxPoint(x, y + 2 * dY), wxSize(75, 20));
    dlg->Connect(ID_TEXTCTRL_FRONT_K, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(ExponentDialog::OnControlEvent));

    std::stringstream streamBack;
    streamBack << std::fixed << std::setprecision(5) << oldBackK;
    tcBackK = new wxTextCtrl(panel, ID_TEXTCTRL_BACK_K, streamBack.str().c_str(), wxPoint(x, y + 3 * dY), wxSize(75, 20));
    dlg->Connect(ID_TEXTCTRL_BACK_K, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(ExponentDialog::OnControlEvent));

    return panel;
}


ExponentDialog::ExponentDialog() : wxDialog(nullptr, -1, wxT("Параметры треугольного сигнала"), wxDefaultPosition, wxSize(225, 252))
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
    vBox->Add(CreatePanelParameters(this));
    vBox->Add(hBoxButtons);
    
    SetSizer(vBox);
    
    Centre();

    SendAdditionForm();
}


ExponentDialog::~ExponentDialog()
{
    TheForm->SetAdditionForm(nullptr);
}


static void DrawLine(int x1, int y1, int x2, int y2)
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


static void SendAdditionForm()
{
    double frontK = std::atof(tcFrontK->GetValue());

    if(frontK == 0.0)
    {
        frontK = oldFrontK;
    }

    double backK = std::atof(tcFrontK->GetValue());
    
    if(backK == 0.0)
    {
        backK = oldBackK;
    }

    int levelHI = static_cast<int>(Point::AVE_VALUE - (Point::MAX_VALUE + Point::MIN_VALUE) / 2.0F * scUp->GetValue() / 100.0F); //-V2007
    int levelLOW = static_cast<int>(Point::AVE_VALUE - (Point::MAX_VALUE + Point::MIN_VALUE) / 2.0F * scDown->GetValue() / 100.0F); //-V2007

    int min = levelLOW;
    int max = levelHI;

    if(rbBack->GetValue())
    {
        min = levelHI;
        max = levelLOW;
    }

    DrawLine(0, min, scDelay->GetValue(), min);

    int start = scDelay->GetValue();

    int topX = static_cast<int>(start + scFrontTime->GetValue());

    for(int i = start + 1; i < topX; i++)
    {
        double param = (i - start) * frontK;

        double d = min - std::log(param) * 500 * frontK;

        if(d < Point::MIN_VALUE)
        {
            d = static_cast<double>(Point::MIN_VALUE);
        }

        if(d > Point::MAX_VALUE)
        {
            d = static_cast<double>(Point::MAX_VALUE);
        }

        uint16 value = static_cast<uint16>(d);

        data[i] = value;
    }

    TheForm->SetAdditionForm(data);
}


void ExponentDialog::OnControlEvent(wxCommandEvent &)
{
    SendAdditionForm();
}


void ExponentDialog::OnButtonOk(wxCommandEvent &)
{
    TheForm->SetMainForm(data, &points);
    
    Destroy();
}


void ExponentDialog::OnButtonCancel(wxCommandEvent &)
{
    Destroy();
}
