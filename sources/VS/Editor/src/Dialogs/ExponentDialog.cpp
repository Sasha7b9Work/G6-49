#include "defines.h"
#include "Form.h"
#include "Utils.h"
#include "Dialogs/ExponentDialog.h"
#include "Controls/SpinControl.h"
#pragma warning(push, 0)
#include <wx/spinctrl.h>
#include <wx/statline.h>
#pragma warning(pop)
#include <vector>


enum
{
    ID_SPINCTRL_FRONT_DELAY,
    ID_SPINCTRL_FRONT_TIME,
    ID_TEXTCTRL_FRONT_K,
    ID_TEXTCTRL_BACK_K
};


static double oldFrontK = 1.0;
static double oldBackK = 1.0;


static SpinControl *scDelay = nullptr;          // Задержка перед началом экспоненциального импульса
static SpinControl *scFrontTime = nullptr;      // Время от начала экспоненциального импульса до начала спада
static wxTextCtrl *tcFrontK = nullptr;          // Коэффициент экспоненты
static wxTextCtrl *tcBackK = nullptr;


static wxPanel *CreatePanelParameters(wxDialog *dlg)
{
    wxPanel *panel = new wxPanel(dlg);

    new wxStaticBox(panel, wxID_ANY, wxT("Параметры"), wxDefaultPosition, wxSize(Dialog::WIDTH_PANEL, 75 + 26 * 2));

    int y = 20, x = 10, dY = 26;

    scDelay = new SpinControl(panel, ID_SPINCTRL_FRONT_DELAY, wxT("0"), wxPoint(x, y), wxSize(50, 20), 0, Point::NUM_POINTS, 0, dlg, wxCommandEventHandler(ExponentDialog::OnControlEvent), wxT("Задержка, точки"));
    scFrontTime = new SpinControl(panel, ID_SPINCTRL_FRONT_TIME, wxT("4095"), wxPoint(x, y + dY), wxSize(50, 20), 0, Point::NUM_POINTS, Point::NUM_POINTS / 2, dlg, wxCommandEventHandler(ExponentDialog::OnControlEvent), wxT("Время нарастания, точки"));

    tcFrontK = new wxTextCtrl(panel, ID_TEXTCTRL_FRONT_K, Utils::DoubleToString(oldFrontK), wxPoint(x, y + 2 * dY), wxSize(75, 20));
    dlg->Connect(ID_TEXTCTRL_FRONT_K, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(ExponentDialog::OnControlEvent));

    tcBackK = new wxTextCtrl(panel, ID_TEXTCTRL_BACK_K, Utils::DoubleToString(oldBackK), wxPoint(x, y + 3 * dY), wxSize(75, 20));
    dlg->Connect(ID_TEXTCTRL_BACK_K, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(ExponentDialog::OnControlEvent));

    return panel;
}


ExponentDialog::ExponentDialog() : Dialog(wxT("Параметры экспоненциального сигнала"))
{
    wxBoxSizer *vBox = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *hBoxPanels = new wxBoxSizer(wxHORIZONTAL);

    hBoxPanels->Add(CreatePanelPolarity());
    hBoxPanels->AddStretchSpacer();
    hBoxPanels->Add(CreatePanelLevels());
    vBox->Add(hBoxPanels);
    vBox->Add(CreatePanelParameters(this));

    SetBoxSizer(vBox, { 221, 203 });
}


void ExponentDialog::SendAdditionForm()
{
    double frontK = Utils::StringToDouble(tcFrontK->GetValue());

    if(frontK == 0.0) //-V2550 //-V550
    {
        frontK = oldFrontK;
    }

    double backK = Utils::StringToDouble(tcBackK->GetValue());
    
    if(backK == 0.0) //-V2550 //-V550
    {
        backK = oldBackK;
    }

    int levelHI = static_cast<int>(Point::AVE + (Point::MAX + Point::MIN) / 2.0F * static_cast<float>(scLevelUp->GetValue()) / 100.0F); //-V2007
    int levelLOW = static_cast<int>(Point::AVE + (Point::MAX + Point::MIN) / 2.0F * static_cast<float>(scLevelDown->GetValue()) / 100.0F); //-V2007

    int min = levelLOW;
    int max = levelHI;

    if(rbPolarityBack->GetValue())
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

        double d = min + std::log(param) * 500 * frontK;

        if(d < min)
        {
            d = static_cast<double>(min);
        }

        if(d > max)
        {
            d = static_cast<double>(max);
        }

        uint16 value = static_cast<uint16>(d);

        data[i] = value;
    }

    TheForm->SetAdditionForm(data);

    points.clear();
}
