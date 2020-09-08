#include "defines.h"
#include "Controls/SpinControl.h"
#include "Dialogs/TrapezeDialog.h"
#include "Editor/Form.h"

#pragma warning(push, 0)
#include <wx/spinctrl.h>
#include <wx/statline.h>
#pragma warning(pop)
#include <vector>


enum
{
    ID_SPINCTRL_DELAY,
    ID_SPINCTRL_VERTEX_1,
    ID_SPINCTRL_VERTEX_2
};


static int delay = 0;
static int vertex1 = -50;
static int vertex2 = 50;
static bool polarityDirect = true;
static bool polarityBack = false;
static int levelUp = 100;
static int levelDown = -100;


wxPanel *TrapezeDialog::CreatePanelOffsets()
{
    wxPanel *panel = new wxPanel(this);

    int y = 20, x = 10;

    new wxStaticBox(panel, wxID_ANY, wxT("Смещения"), wxDefaultPosition, { Dialog::WIDTH_PANEL, 73 + 26 });

    scDelay = new SpinControl(panel, ID_SPINCTRL_DELAY, { x, y }, { 50, 20 }, 0, Point::AMOUNT, delay,
                                this, wxCommandEventHandler(TrapezeDialog::OnControlEvent), wxT("Задержка, точки"), this);
    scVertex1 = new SpinControl(panel, ID_SPINCTRL_VERTEX_1, { x, y + 26 }, { 50, 20 }, -100, 100, vertex1,
                                this, wxCommandEventHandler(TrapezeDialog::OnControlEvent), wxT("Левая вершина, %"), this);
    scVertex2 = new SpinControl(panel, ID_SPINCTRL_VERTEX_2, { x, y + 26 * 2 }, { 50, 20 }, -100, 100, vertex2,
                                this, wxCommandEventHandler(TrapezeDialog::OnControlEvent), wxT("Правая вершина, %"), this);

    return panel;
}


TrapezeDialog::TrapezeDialog() : Dialog(wxT("Параметры трапециевидного сигнала"), false)
{
    wxBoxSizer *vBox = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *hBoxPanels = new wxBoxSizer(wxHORIZONTAL);

    hBoxPanels->Add(CreatePanelPolarity(polarityDirect, polarityBack));
    hBoxPanels->AddStretchSpacer();
    hBoxPanels->Add(CreatePanelLevels(levelUp, levelDown));
    vBox->Add(hBoxPanels);
    vBox->Add(CreatePanelOffsets());

    SetBoxSizer(vBox, { 221, 175 });
}


void TrapezeDialog::SendAdditionForm()
{
    int start = scDelay->GetValue();

    float center = static_cast<float>(start) + static_cast<float>(Point::AMOUNT - start) / 2.0F;

    float pointsInTrapeze = static_cast<float>(Point::AMOUNT - start);

    int left = static_cast<int>(center + pointsInTrapeze / 2.0F * static_cast<float>(scVertex1->GetValue()) / 100.0F);
    int right = static_cast<int>(center + pointsInTrapeze / 2.0F * static_cast<float>(scVertex2->GetValue()) / 100.0F);

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

    DrawLine(start, min, left, max);

    DrawLine(left, max, right, max);

    DrawLine(right, max, Point::AMOUNT - 1, min);

    TheForm->SetAdditionForm(data);

    points.clear();

    points.push_back(Point::FromData(static_cast<uint16>(start), static_cast<uint16>(min)));
    points.push_back(Point::FromData(static_cast<uint16>(left),  static_cast<uint16>(max)));
    points.push_back(Point::FromData(static_cast<uint16>(right), static_cast<uint16>(max)));
}


void TrapezeDialog::SaveValues()
{
    delay = scDelay->GetValue();

    vertex1 = scVertex1->GetValue();
    vertex2 = scVertex2->GetValue();

    levelUp = scLevelUp->GetValue();
    levelDown = scLevelDown->GetValue();

    polarityDirect = rbPolarityDirect->GetValue();
    polarityBack = rbPolarityBack->GetValue();
}
