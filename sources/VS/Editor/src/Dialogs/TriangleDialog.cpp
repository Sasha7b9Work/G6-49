// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
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

int TriangleDialog::center = 0;
int TriangleDialog::delay = 0;
bool TriangleDialog::polarityDirect = true;
bool TriangleDialog::polarityBack = false;
int TriangleDialog::levelUp = 100;
int TriangleDialog::levelDown = -100;


wxPanel *TriangleDialog::CreatePanelOffsets()
{
    wxPanel *panel = new wxPanel(this);

    int y = 20, x = 10;

    new wxStaticBox(panel, wxID_ANY, wxT("��������"), wxDefaultPosition, wxSize(Dialog::WIDTH_PANEL, 75));

    scCenter = new SpinControl(panel, ID_SPINCTRL_CENTER, wxPoint(x, y), wxSize(51, 20), -100, 100, center, this, wxCommandEventHandler(TriangleDialog::OnControlEvent), wxT("�����, %"), this);
    scDelay = new SpinControl(panel, ID_SPINCTRL_DELAY, wxPoint(x, y + 26), wxSize(51, 20), 0, Point::AMOUNT, delay, this, wxCommandEventHandler(TriangleDialog::OnControlEvent), wxT("��������, �����"), this);

    return panel;
}


TriangleDialog::TriangleDialog() : Dialog(wxT("��������� ������������ �������"), false)
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

    float pointsInTriangle = (float)(Point::AMOUNT - start);

    int top = (int)((float)start + pointsInTriangle / 2 + pointsInTriangle / 2.0F * (float)scCenter->GetValue() / 100.0F);

    int levelHI = (int)(Point::AVE + (Point::MAX + Point::MIN) / 2.0F * (float)scLevelUp->GetValue() / 100.0F); //-V2007
    int levelLOW = (int)(Point::AVE + (Point::MAX + Point::MIN) / 2.0F * (float)scLevelDown->GetValue() / 100.0F); //-V2007

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

    points.push_back(Point::FromData((uint16)start, (uint16)min)); //-V823
    points.push_back(Point::FromData((uint16)top, (uint16)max)); //-V823
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
