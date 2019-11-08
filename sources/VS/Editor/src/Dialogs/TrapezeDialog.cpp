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
    ID_SPINCTRL_DELAY,
    ID_SPINCTRL_VERTEX_1,
    ID_SPINCTRL_VERTEX_2
};


static SpinControl *scDelay = nullptr;
static SpinControl *scVertex1 = nullptr;
static SpinControl *scVertex2 = nullptr;


static wxPanel *CreatePanelOffsets(wxDialog *dlg)
{
    wxPanel *panel = new wxPanel(dlg);

    int y = 20, x = 10;

    new wxStaticBox(panel, wxID_ANY, wxT("Смещения"), wxDefaultPosition, wxSize(Dialog::WIDTH_PANEL, 73 + 26));

    scDelay = new SpinControl(panel, ID_SPINCTRL_DELAY, wxT("0"), wxPoint(x, y), wxSize(50, 20), 0, Point::NUM_POINTS, 0, dlg, wxCommandEventHandler(TrapezeDialog::OnControlEvent), wxT("Задержка, точки"));
    scVertex1 = new SpinControl(panel, ID_SPINCTRL_VERTEX_1, wxT("-50"), wxPoint(x, y + 26), wxSize(50, 20), -100, 100, -50, dlg, wxCommandEventHandler(TrapezeDialog::OnControlEvent), wxT("Левая вершина, %"));
    scVertex2 = new SpinControl(panel, ID_SPINCTRL_VERTEX_2, wxT("50"), wxPoint(x, y + 26 * 2), wxSize(50, 20), -100, 100, -50, dlg, wxCommandEventHandler(TrapezeDialog::OnControlEvent), wxT("Правая вершина, %"));

    return panel;
}


TrapezeDialog::TrapezeDialog() : Dialog(wxT("Параметры трапециевидного сигнала"))
{
    wxBoxSizer *vBox = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *hBoxPanels = new wxBoxSizer(wxHORIZONTAL);

    hBoxPanels->Add(CreatePanelPolarity());
    hBoxPanels->AddStretchSpacer();
    hBoxPanels->Add(CreatePanelLevels());
    vBox->Add(hBoxPanels);
    vBox->Add(CreatePanelOffsets(this));

    SetBoxSizer(vBox, { 221, 175 });
}


void TrapezeDialog::SendAdditionForm()
{
    int delay = scDelay->GetValue();

    int center = delay + (static_cast<int>(Point::NUM_POINTS) - delay) / 2;

    int pointsInTrapeze = static_cast<int>(Point::NUM_POINTS) - delay;

    int vertex1 = static_cast<int>(center + pointsInTrapeze / 2.0F * scVertex1->GetValue() / 100.0F);
    int vertex2 = static_cast<int>(center + pointsInTrapeze / 2.0F * scVertex2->GetValue() / 100.0F);

    int levelHI = static_cast<int>(Point::AVE_VALUE + (Point::MAX_VALUE + Point::MIN_VALUE) / 2.0F * scLevelUp->GetValue() / 100.0F); //-V2007
    int levelLOW = static_cast<int>(Point::AVE_VALUE + (Point::MAX_VALUE + Point::MIN_VALUE) / 2.0F * scLevelDown->GetValue() / 100.0F); //-V2007

    int min = levelLOW;
    int max = levelHI;

    if (rbPolarityBack->GetValue())
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
