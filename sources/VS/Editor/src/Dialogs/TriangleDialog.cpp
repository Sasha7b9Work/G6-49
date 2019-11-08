#include "defines.h"
#include "Form.h"
#include "History.h"
#include "SpinControl.h"
#include "TriangleDialog.h"
#pragma warning(push, 0)
#include <wx/spinctrl.h>
#include <wx/statline.h>
#pragma warning(pop)


enum
{
    ID_SPINCTRL_CENTER,
    ID_SPINCTRL_DELAY,
    ID_RADIOBUTTON_DIRECT,
    ID_RADIOBUTTON_BACK
};


static wxRadioButton *rbDirect = nullptr;
static wxRadioButton *rbBack = nullptr;
static SpinControl *scCenter = nullptr;
static SpinControl *scDelay = nullptr;


static wxPanel *CreatePanelPolarity(wxDialog *dlg)
{
    wxPanel *panel = new wxPanel(dlg);
    new wxStaticBox(panel, wxID_ANY, wxT("Полярность"), wxDefaultPosition, wxSize(90, 75));

    int y = 25, x = 5;

    rbDirect = new wxRadioButton(panel, ID_RADIOBUTTON_DIRECT, wxT("Прямая"), wxPoint(x, y));
    dlg->Connect(ID_RADIOBUTTON_DIRECT, wxEVT_RADIOBUTTON, wxCommandEventHandler(TriangleDialog::OnControlEvent));
    rbDirect->SetValue(true);

    rbBack = new wxRadioButton(panel, ID_RADIOBUTTON_BACK, wxT("Обратная"), wxPoint(x, y + 25));
    dlg->Connect(ID_RADIOBUTTON_BACK, wxEVT_RADIOBUTTON, wxCommandEventHandler(TriangleDialog::OnControlEvent));

    return panel;
}


static wxPanel *CreatePanelOffsets(wxDialog *dlg)
{
    wxPanel *panel = new wxPanel(dlg);

    int y = 20, x = 10;

    new wxStaticBox(panel, wxID_ANY, wxT("Смещения"), wxDefaultPosition, wxSize(220, 75));

    scCenter = new SpinControl(panel, ID_SPINCTRL_CENTER, wxT("0"), wxPoint(x, y), wxSize(50, 20), -100, 100, 0, dlg, wxCommandEventHandler(TriangleDialog::OnControlEvent), wxT("Центр, %"));
    scDelay = new SpinControl(panel, ID_SPINCTRL_DELAY, wxT("0"), wxPoint(x, y + 26), wxSize(50, 20), 0, Point::NUM_POINTS, 0, dlg, wxCommandEventHandler(TriangleDialog::OnControlEvent), wxT("Задержка, точки"));

    return panel;
}


TriangleDialog::TriangleDialog() : Dialog(wxT("Параметры треугольного сигнала"))
{
    wxBoxSizer *vBox = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *hBoxPanels = new wxBoxSizer(wxHORIZONTAL);

    hBoxPanels->Add(CreatePanelPolarity(this));
    hBoxPanels->AddStretchSpacer();
    hBoxPanels->Add(CreatePanelLevels());
    vBox->Add(hBoxPanels);
    vBox->Add(CreatePanelOffsets(this));
    
    SetBoxSizer(vBox, { 221, 150 });
}


void TriangleDialog::SendAdditionForm()
{
    int start = scDelay->GetValue();

    int pointsInTriangle = static_cast<int>(Point::NUM_POINTS) - start;

    int top = static_cast<int>(start + pointsInTriangle / 2 + pointsInTriangle / 2.0F * scCenter->GetValue() / 100.0F);

    int levelHI = static_cast<int>(Point::AVE_VALUE + (Point::MAX_VALUE + Point::MIN_VALUE) / 2.0F * scLevelUp->GetValue() / 100.0F); //-V2007
    int levelLOW = static_cast<int>(Point::AVE_VALUE + (Point::MAX_VALUE + Point::MIN_VALUE) / 2.0F * scLevelDown->GetValue() / 100.0F); //-V2007

    int min = levelLOW;
    int max = levelHI;

    if (rbBack->GetValue())
    {
        min = levelHI;
        max = levelLOW;
    }

    DrawLine(0, min, start, min);

    DrawLine(start, min, top, max);

    DrawLine(top, max, Point::NUM_POINTS - 1, min);

    TheForm->SetAdditionForm(data);

    points.clear();

    points.push_back(Point(static_cast<uint16>(start), static_cast<uint16>(min)));
    points.push_back(Point(static_cast<uint16>(top), static_cast<uint16>(max)));
}
