#include "defines.h"
#include "Controls/SpinControl.h"
#include "Dialogs/TriangleDialog.h"
#include "Editor/Form.h"
#include "Editor/History.h"

#pragma warning(push, 0)
#include <wx/spinctrl.h>
#include <wx/statline.h>
#pragma warning(pop)


enum
{
    ID_SPINCTRL_CENTER,
    ID_SPINCTRL_DELAY
};


static int center = 0;
static int delay = 0;
static bool polarityDirect = true;
static bool polarityBack = false;
static int levelUp = 100;
static int levelDown = -100;


wxPanel *TriangleDialog::CreatePanelOffsets()
{
    wxPanel *panel = new wxPanel(this);

    int y = 20, x = 10;

    new wxStaticBox(panel, wxID_ANY, wxT("Смещения"), wxDefaultPosition, wxSize(Dialog::WIDTH_PANEL, 75));

    scCenter = new SpinControl(panel, ID_SPINCTRL_CENTER, wxPoint(x, y), wxSize(51, 20), -100, 100, center, this, wxCommandEventHandler(TriangleDialog::OnControlEvent), wxT("Центр, %"), this);
    scDelay = new SpinControl(panel, ID_SPINCTRL_DELAY, wxPoint(x, y + 26), wxSize(51, 20), 0, Point::AMOUNT, delay, this, wxCommandEventHandler(TriangleDialog::OnControlEvent), wxT("Задержка, точки"), this);

    return panel;
}


TriangleDialog::TriangleDialog() : Dialog(wxT("Параметры треугольного сигнала"), false)
{
    wxBoxSizer *vBox = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *hBoxPanels = new wxBoxSizer(wxHORIZONTAL);

    hBoxPanels->Add(CreatePanelPolarity(polarityDirect, polarityBack));
    hBoxPanels->AddStretchSpacer();
    hBoxPanels->Add(CreatePanelLevels(levelUp, levelDown));
    vBox->Add(hBoxPanels);
    vBox->Add(CreatePanelOffsets());
    
    SetBoxSizer(vBox, { 221, 150 });
}


void TriangleDialog::SendAdditionForm()
{
    int start = scDelay->GetValue();

    float pointsInTriangle = static_cast<float>(Point::AMOUNT - start);

    int top = static_cast<int>(static_cast<float>(start) + pointsInTriangle / 2 + pointsInTriangle / 2.0F * static_cast<float>(scCenter->GetValue()) / 100.0F);

    int levelHI = static_cast<int>(Point::AVE + (Point::MAX + Point::MIN) / 2.0F * static_cast<float>(scLevelUp->GetValue()) / 100.0F); //-V2007
    int levelLOW = static_cast<int>(Point::AVE + (Point::MAX + Point::MIN) / 2.0F * static_cast<float>(scLevelDown->GetValue()) / 100.0F); //-V2007

    int min = levelLOW;
    int max = levelHI;

    if (rbPolarityBack->GetValue())
    {
        min = levelHI;
        max = levelLOW;
    }

    DrawLine(0, min, start, min);

    DrawLine(start, min, top, max);

    DrawLine(top, max, Point::AMOUNT - 1, min);

    TheForm->SetAdditionForm(data);

    points.clear();

    points.push_back(Point::FromData(static_cast<uint16>(start), static_cast<uint16>(min))); //-V823
    points.push_back(Point::FromData(static_cast<uint16>(top), static_cast<uint16>(max))); //-V823
}


void TriangleDialog::SaveValues()
{
    center = scCenter->GetValue();
    delay = scDelay->GetValue();
    polarityDirect = rbPolarityDirect->GetValue();
    polarityBack = rbPolarityBack->GetValue();
    levelUp = scLevelUp->GetValue();
    levelDown = scLevelDown->GetValue();
}
