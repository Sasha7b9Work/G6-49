#include "defines.h"
#include "Form.h"
#include "SpinControl.h"
#include "TriangleDialog.h"
#pragma warning(push, 0)
#include <wx/spinctrl.h>
#include <wx/statline.h>
#pragma warning(pop)


enum
{
    ID_SPINCTRL_DONW,
    ID_SPINCTRL_UP,
    ID_SPINCTRL_CENTER,
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
static SpinControl *scCenter = nullptr;


/// Послать форму для ознакомительной отрисовки
static void SendForm();

static uint16 data[Point::NUM_POINTS];

static Point point(static_cast<uint16>(0), 0U);


static wxPanel *CreatePanelPolarity(wxDialog *dlg)
{
    wxPanel *panel = new wxPanel(dlg);
    sb = new wxStaticBox(panel, wxID_ANY, wxT("Полярность"), wxDefaultPosition, wxSize(90, 75));

    int y = 25, x = 5;

    rbDirect = new wxRadioButton(panel, ID_RADIOBUTTON_DIRECT, wxT("Прямая"), wxPoint(x, y));
    dlg->Connect(ID_RADIOBUTTON_DIRECT, wxEVT_RADIOBUTTON, wxCommandEventHandler(TriangleDialog::OnControlEvent));
    rbDirect->SetValue(true);

    rbBack = new wxRadioButton(panel, ID_RADIOBUTTON_BACK, wxT("Обратная"), wxPoint(x, y + 25));
    dlg->Connect(ID_RADIOBUTTON_BACK, wxEVT_RADIOBUTTON, wxCommandEventHandler(TriangleDialog::OnControlEvent));

    return panel;
}


static wxPanel *CreatePanelLevels(wxDialog *dlg)
{
    wxPanel *panel = new wxPanel(dlg);

    sb = new wxStaticBox(panel, wxID_ANY, wxT("Уровни"), wxDefaultPosition, wxSize(125, 75));

    int y = 20, x = 10, dY = 2, dX = 55;

    scUp = new SpinControl(panel, ID_SPINCTRL_UP, wxT("100"), wxPoint(x, y), wxSize(50, 20), -100, 100, 100, dlg, wxCommandEventHandler(TriangleDialog::OnControlEvent));
    text = new wxStaticText(panel, wxID_ANY, wxT("Верхний"), wxPoint(x + dX, y + dY), wxDefaultSize, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);

    scDown = new SpinControl(panel, ID_SPINCTRL_DONW, wxT("-100"), wxPoint(x, y + 26), wxSize(50, 20), -100, 100, -100, dlg, wxCommandEventHandler(TriangleDialog::OnControlEvent));
    text = new wxStaticText(panel, wxID_ANY, wxT("Нижний"), wxPoint(x + dX, y + 26 + dY), wxDefaultSize, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);

    return panel;
}

static wxPanel *CreatePanelCenter(wxDialog *dlg)
{
    wxPanel *panel = new wxPanel(dlg);

    int y = 20, x = 10;

    sb = new wxStaticBox(panel, wxID_ANY, wxT("Центр"), wxDefaultPosition, wxSize(216, 50));

    scCenter = new SpinControl(panel, ID_SPINCTRL_CENTER, wxT("0"), wxPoint(x, y), wxSize(50, 20), -100, 100, 0, dlg, wxCommandEventHandler(TriangleDialog::OnControlEvent));
    text = new wxStaticText(panel, wxID_ANY, wxT("Смещение"), wxPoint(x + 55, y + 2), wxDefaultSize, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);

    return panel;
}


TriangleDialog::TriangleDialog() : wxDialog(nullptr, -1, wxT("Параметры треугольного сигнала"), wxDefaultPosition, wxSize(225, 185))
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
    vBox->Add(CreatePanelCenter(this));
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
    int center = static_cast<int>(Point::NUM_POINTS / 2 + Point::NUM_POINTS / 2.0F * scCenter->GetValue() / 100.0F);

    int levelHI = static_cast<int>(Point::AVE_VALUE - (Point::MAX_VALUE + Point::MIN_VALUE) / 2.0F * scUp->GetValue() / 100.0F); //-V2007
    int levelLOW = static_cast<int>(Point::AVE_VALUE - (Point::MAX_VALUE + Point::MIN_VALUE) / 2.0F * scDown->GetValue() / 100.0F); //-V2007

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

    point.pos = static_cast<uint16>(center);
    point.data = static_cast<uint16>(max);
}


void TriangleDialog::OnControlEvent(wxCommandEvent &)
{
    SendForm();
}


void TriangleDialog::OnButtonOk(wxCommandEvent &)
{
    TheForm->SetMainForm(data);
    TheForm->SetPoint(point);
    Destroy();
}


void TriangleDialog::OnButtonCancel(wxCommandEvent &)
{
    Destroy();
}
