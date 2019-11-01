#include "defines.h"
#include "TriangleDialog.h"
#pragma warning(push, 0)
#include <wx/spinctrl.h>
#include <wx/statline.h>
#pragma warning(pop)


enum
{
    ID_SPINCTRL_DONW = wxID_HIGHEST + 1,
    ID_SPINCTRL_UP
};


static wxPanel *CreatePanelPolarity(wxDialog *dlg)
{
    wxPanel *panel = new wxPanel(dlg);
    wxStaticBox *sb = new wxStaticBox(panel, wxID_ANY, wxT("Полярность"), wxDefaultPosition, wxSize(90, 75));

    int y = 25;
    int x = 5;

    wxRadioButton *rbDirect = new wxRadioButton(panel, wxID_ANY, wxT("Прямая"), wxPoint(x, y));
    rbDirect->SetValue(true);
    wxRadioButton *rbBack = new wxRadioButton(panel, wxID_ANY, wxT("Обратная"), wxPoint(x, y + 25));

    sb = sb;
    rbDirect = rbDirect;
    rbBack = rbBack;

    return panel;
}


static wxPanel *CreatePanelLevels(wxDialog *dlg)
{
    wxPanel *panel = new wxPanel(dlg);

    wxStaticBox *sb = new wxStaticBox(panel, wxID_ANY, wxT("Уровни"), wxDefaultPosition, wxSize(125, 75));

    int y = 20;
    int x = 10;

    int dY = 2;
    int dX = 55;

    wxSpinCtrl *scUp = new wxSpinCtrl(panel, ID_SPINCTRL_UP, wxT("100"), wxPoint(x, y), wxSize(50, 20), wxSP_ARROW_KEYS, -100, 100, 100);
    wxStaticText *textUp = new wxStaticText(panel, wxID_ANY, wxT("Верхний"), wxPoint(x + dX, y + dY), wxDefaultSize, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
    wxSpinCtrl *scDown = new wxSpinCtrl(panel, ID_SPINCTRL_DONW, wxT("-100"), wxPoint(x, y + 26), wxSize(50, 20), wxSP_ARROW_KEYS, -100, 100, -100);
    wxStaticText *textDown = new wxStaticText(panel, wxID_ANY, wxT("Нижний"), wxPoint(x + dX, y + 26 + dY), wxDefaultSize, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);

    sb = sb;
    scUp = scUp;
    scDown = scDown;
    textUp = textUp;
    textDown = textDown;

    return panel;
}


TriangleDialog::TriangleDialog() : wxDialog(nullptr, -1, wxT("Параметры треугольного сигнала"), wxDefaultPosition, wxSize(225, 135))
{
    wxButton *btnOk = new wxButton(this, -1, wxT("Ok"), wxDefaultPosition, BUTTON_SIZE);
    wxButton *btnClose = new wxButton(this, -1, wxT("Закрыть"), wxDefaultPosition, BUTTON_SIZE);

    wxBoxSizer *vBox = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *hBoxButtons = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *hBoxPanels = new wxBoxSizer(wxHORIZONTAL);

    hBoxPanels->Add(CreatePanelPolarity(this));
    hBoxPanels->AddStretchSpacer();
    hBoxPanels->Add(CreatePanelLevels(this));
    hBoxButtons->Add(btnOk);
    hBoxButtons->Add(btnClose);
    vBox->Add(hBoxPanels);
    vBox->Add(hBoxButtons);
    
    SetSizer(vBox);
    
    Centre();
}
