#include "defines.h"
#include "Form.h"
#include "Dialogs/SpinControl.h"
#include "Dialogs/TrapezeDialog.h"
#pragma warning(push, 0)
#include <wx/spinctrl.h>
#include <wx/statline.h>
#pragma warning(pop)
#include <vector>


enum
{
    ID_SPINCTRL_DONW,
    ID_SPINCTRL_UP,
    ID_SPINCTRL_DELAY,
    ID_SPINCTRL_VERTEX_1,
    ID_SPINCTRL_VERTEX_2,
    ID_RADIOBUTTON_DIRECT,
    ID_RADIOBUTTON_BACK,
    ID_BUTTON_OK,
    ID_BUTTON_CANCEL
};


static wxRadioButton *rbDirect = nullptr;
static wxRadioButton *rbBack = nullptr;
static SpinControl *scUp = nullptr;
static SpinControl *scDown = nullptr;
static SpinControl *scDelay = nullptr;
static SpinControl *scVertex1 = nullptr;
static SpinControl *scVertex2 = nullptr;


/// Послать форму для ознакомительной отрисовки
static void SendForm();

static uint16 data[Point::NUM_POINTS];


static std::vector<Point> points;


static wxPanel *CreatePanelPolarity(wxDialog *dlg)
{
    wxPanel *panel = new wxPanel(dlg);
    new wxStaticBox(panel, wxID_ANY, wxT("Полярность"), wxDefaultPosition, wxSize(90, 75));

    int y = 25, x = 5;

    rbDirect = new wxRadioButton(panel, ID_RADIOBUTTON_DIRECT, wxT("Прямая"), wxPoint(x, y));
    dlg->Connect(ID_RADIOBUTTON_DIRECT, wxEVT_RADIOBUTTON, wxCommandEventHandler(TrapezeDialog::OnControlEvent));
    rbDirect->SetValue(true);

    rbBack = new wxRadioButton(panel, ID_RADIOBUTTON_BACK, wxT("Обратная"), wxPoint(x, y + 25));
    dlg->Connect(ID_RADIOBUTTON_BACK, wxEVT_RADIOBUTTON, wxCommandEventHandler(TrapezeDialog::OnControlEvent));

    return panel;
}


static wxPanel *CreatePanelLevels(wxDialog *dlg)
{
    wxPanel *panel = new wxPanel(dlg);

    new wxStaticBox(panel, wxID_ANY, wxT("Уровни"), wxDefaultPosition, wxSize(125, 75));

    int y = 20, x = 10;

    scUp = new SpinControl(panel, ID_SPINCTRL_UP, wxT("100"), wxPoint(x, y), wxSize(50, 20), -100, 100, 100, dlg, wxCommandEventHandler(TrapezeDialog::OnControlEvent), wxT("Верхний, %"));
    scDown = new SpinControl(panel, ID_SPINCTRL_DONW, wxT("-100"), wxPoint(x, y + 26), wxSize(50, 20), -100, 100, -100, dlg, wxCommandEventHandler(TrapezeDialog::OnControlEvent), wxT("Нижний, %"));

    return panel;
}

static wxPanel *CreatePanelOffsets(wxDialog *dlg)
{
    wxPanel *panel = new wxPanel(dlg);

    int y = 20, x = 10;

    new wxStaticBox(panel, wxID_ANY, wxT("Смещения"), wxDefaultPosition, wxSize(216, 73 + 26));

    scDelay = new SpinControl(panel, ID_SPINCTRL_DELAY, wxT("0"), wxPoint(x, y), wxSize(50, 20), 0, Point::NUM_POINTS, 0, dlg, wxCommandEventHandler(TrapezeDialog::OnControlEvent), wxT("Задержка, точки"));
    scVertex1 = new SpinControl(panel, ID_SPINCTRL_VERTEX_1, wxT("-50"), wxPoint(x, y + 26), wxSize(50, 20), -100, 100, -50, dlg, wxCommandEventHandler(TrapezeDialog::OnControlEvent), wxT("Левая вершина, %"));
    scVertex2 = new SpinControl(panel, ID_SPINCTRL_VERTEX_2, wxT("50"), wxPoint(x, y + 26 * 2), wxSize(50, 20), -100, 100, -50, dlg, wxCommandEventHandler(TrapezeDialog::OnControlEvent), wxT("Правая вершина, %"));

    return panel;
}


TrapezeDialog::TrapezeDialog() : wxDialog(nullptr, -1, wxT("Параметры треугольного сигнала"), wxDefaultPosition, wxSize(225, 226))
{
    wxButton *btnOk = new wxButton(this, ID_BUTTON_OK, wxT("Ok"), wxDefaultPosition, BUTTON_SIZE);
    Connect(ID_BUTTON_OK, wxEVT_BUTTON, wxCommandEventHandler(TrapezeDialog::OnButtonOk));
    wxButton *btnClose = new wxButton(this, ID_BUTTON_CANCEL, wxT("Отмена"), wxDefaultPosition, BUTTON_SIZE);
    Connect(ID_BUTTON_CANCEL, wxEVT_BUTTON, wxCommandEventHandler(TrapezeDialog::OnButtonCancel));

    wxBoxSizer *vBox = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *hBoxButtons = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *hBoxPanels = new wxBoxSizer(wxHORIZONTAL);

    hBoxPanels->Add(CreatePanelPolarity(this));
    hBoxPanels->AddStretchSpacer();
    hBoxPanels->Add(CreatePanelLevels(this));
    hBoxButtons->Add(btnOk);
    hBoxButtons->Add(btnClose);
    vBox->Add(hBoxPanels);
    vBox->Add(CreatePanelOffsets(this));
    vBox->Add(hBoxButtons);
    
    SetSizer(vBox);
    
    Centre();

    SendForm();
}


TrapezeDialog::~TrapezeDialog()
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
    int delay = scDelay->GetValue();

    int center = delay + (static_cast<int>(Point::NUM_POINTS) - delay) / 2;

    int pointsInTrapeze = static_cast<int>(Point::NUM_POINTS) - delay;

    int vertex1 = center + pointsInTrapeze / 2.0F * scVertex1->GetValue() / 100.0F;
    int vertex2 = center + pointsInTrapeze / 2.0F * scVertex2->GetValue() / 100.0F;

    int levelHI = static_cast<int>(Point::AVE_VALUE - (Point::MAX_VALUE + Point::MIN_VALUE) / 2.0F * scUp->GetValue() / 100.0F); //-V2007
    int levelLOW = static_cast<int>(Point::AVE_VALUE - (Point::MAX_VALUE + Point::MIN_VALUE) / 2.0F * scDown->GetValue() / 100.0F); //-V2007

    int min = levelLOW;
    int max = levelHI;

    if (rbBack->GetValue())
    {
        min = levelHI;
        max = levelLOW;
    }

    DrawLine(0, min, delay, min);

    DrawLine(delay, min, vertex1, max);

    DrawLine(vertex1, max, vertex2, max);

    DrawLine(vertex2, max, Point::NUM_POINTS - 1, min);

    TheForm->SetAdditionForm(data);

    points.clear();

    points.push_back(Point(static_cast<uint16>(delay), static_cast<uint16>(min)));
    points.push_back(Point(static_cast<uint16>(vertex1), static_cast<uint16>(max)));
    points.push_back(Point(static_cast<uint16>(vertex2), static_cast<uint16>(max)));
}


void TrapezeDialog::OnControlEvent(wxCommandEvent &)
{
    SendForm();
}


void TrapezeDialog::OnButtonOk(wxCommandEvent &)
{
    TheForm->SetMainForm(data, &points);
    
    Destroy();
}


void TrapezeDialog::OnButtonCancel(wxCommandEvent &)
{
    Destroy();
}
