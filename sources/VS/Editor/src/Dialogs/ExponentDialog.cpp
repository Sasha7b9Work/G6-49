#include "defines.h"
#include "Controls/SpinControl.h"
#include "Dialogs/ExponentDialog.h"
#include "Editor/Form.h"
#include "Utils/Utils.h"

#pragma warning(push, 0)
#include <wx/spinctrl.h>
#include <wx/statline.h>
#pragma warning(pop)


enum
{
    ID_SPINCTRL_POWER,
    ID_TYPE_1,
    ID_TYPE_2,
    ID_TYPE_3,
    ID_TYPE_4
};


static int power = 2000;
static bool type1 = true;
static bool type2 = false;
static bool type3 = false;
static bool type4 = false;


wxPanel *ExponentDialog::CreatePanelPower()
{
    wxPanel *panel = new wxPanel(this);

    new wxStaticBox(panel, wxID_ANY, wxT("���������� �������"), wxDefaultPosition, { 130, 75 });

    scPower = new SpinControl(panel, ID_SPINCTRL_POWER, { 20, 20 }, { 100, 20 }, 0, std::numeric_limits<int>::max(), power,
                              this, wxCommandEventHandler(ExponentDialog::OnControlEvent), wxT(""), this);
    return panel;
}


wxPanel *ExponentDialog::CreatePanelType()
{
    wxPanel *panel = new wxPanel(this);
    new wxStaticBox(panel, wxID_ANY, wxT("���"), wxDefaultPosition, wxSize(90, 75));

    int y = 25, x = 10, dY = 25, dX = 45;

    rbType1 = new wxRadioButton(panel, ID_TYPE_1, wxT("1"), { x, y });
    Connect(ID_TYPE_1, wxEVT_RADIOBUTTON, wxCommandEventHandler(Dialog::OnControlEvent));

    rbType2 = new wxRadioButton(panel, ID_TYPE_2, wxT("2"), { x, y + dY });
    Connect(ID_TYPE_2, wxEVT_RADIOBUTTON, wxCommandEventHandler(Dialog::OnControlEvent));

    rbType3 = new wxRadioButton(panel, ID_TYPE_3, wxT("3"), { x + dX, y });
    Connect(ID_TYPE_3, wxEVT_RADIOBUTTON, wxCommandEventHandler(Dialog::OnControlEvent));

    rbType4 = new wxRadioButton(panel, ID_TYPE_4, wxT("4"), { x + dX, y + dY });
    Connect(ID_TYPE_4, wxEVT_RADIOBUTTON, wxCommandEventHandler(Dialog::OnControlEvent));
  
    rbType1->SetValue(type1);
    rbType2->SetValue(type2);
    rbType3->SetValue(type3);
    rbType4->SetValue(type4);

    return panel;
}


ExponentDialog::ExponentDialog() : Dialog(wxT("��������� ����������������� �������"), true)
{
    wxBoxSizer *vBox = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *hBoxPanels = new wxBoxSizer(wxHORIZONTAL);

    hBoxPanels->Add(CreatePanelType());
    hBoxPanels->AddStretchSpacer();
    hBoxPanels->Add(CreatePanelPower());
    vBox->Add(hBoxPanels);

    SetBoxSizer(vBox, { 221, 80 });
}


void ExponentDialog::InvertPoint(uint16 *point)
{
    *point = static_cast<uint16>(static_cast<uint16>(Point::MAX - *point) + Point::AVE);
}


void ExponentDialog::ShiftToUp()
{
    uint16 max = 0;

    for (int i = 0; i < Point::AMOUNT; i++)
    {
        if (data[i] > max)
        {
            max = data[i];
        }
    }

    if (max != 4095)
    {
        uint16 delta = 4095U - max;

        for (int i = 0; i < Point::AMOUNT; i++)
        {
            data[i] += delta;
        }
    }
}


void ExponentDialog::SendAdditionForm()
{
    double tau = Math::LimitationBelow<double>(scPower->GetValue() + 1.0, 2.0);

    double x0 = Point::AMOUNT - 1;
    double y0 = Point::AVE;

    double x1 = x0 - 1.0;
    double y1 = y0 * (1.0 - 1.0 / tau);

    double k = (std::log(y0) - std::log(y1)) / (x0 - x1);

    int start = static_cast<int>(std::log(y0) / k - x0);

    for (int i = 0; i < Point::AMOUNT; i++)
    {
        double value = std::exp(k * (static_cast<double>(i) + start));

        uint16 uValue = static_cast<uint16>(Point::AVE + value);

        data[i] = Math::Limitation<uint16>(uValue, Point::MIN, Point::MAX);
    }

    if (rbType2->GetValue())
    {
        for (int i = 0; i < Point::AMOUNT / 2; i++)
        {
            Math::Swap<uint16>(&data[i], &data[Point::AMOUNT - 1 - i]);
        }
    }
    else if (rbType3->GetValue())
    {
        for (int i = 0; i < Point::AMOUNT; i++)
        {
            InvertPoint(&data[i]);
        }
    }
    else if (rbType4->GetValue())
    {
        for (int i = 0; i < Point::AMOUNT / 2; i++)
        {
            Math::Swap<uint16>(&data[i], &data[Point::AMOUNT - 1 - i]);
            InvertPoint(&data[i]);
            InvertPoint(&data[Point::AMOUNT - 1 - i]);
        }
    }

    ShiftToUp();

    TheForm->SetAdditionForm(data);

    points.clear();
}


void ExponentDialog::SaveValues()
{
    power = scPower->GetValue();

    type1 = rbType1->GetValue();
    type2 = rbType2->GetValue();
    type3 = rbType3->GetValue();
    type4 = rbType4->GetValue();
}
