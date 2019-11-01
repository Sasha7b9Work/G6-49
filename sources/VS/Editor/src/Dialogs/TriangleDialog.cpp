#include "defines.h"
#include "TriangleDialog.h"


TriangleDialog::TriangleDialog() : wxDialog(nullptr, -1, wxT("Параметры треугольного сигнала"))
{
    wxPanel *panel = new wxPanel(this, -1);
    
    wxBoxSizer *vBox = new wxBoxSizer(wxVERTICAL);
    
    volatile wxStaticBox *st = new wxStaticBox(panel, -1, wxT("Полярность"), wxPoint(5, 5), wxSize(240, 150));
    
    wxRadioButton *rbDirect = new wxRadioButton(panel, -1, wxT("Прямая"), wxPoint(15, 55));
    wxRadioButton *rbBack = new wxRadioButton(panel, -1, wxT("Обратная"), wxPoint(15, 80));
    
    wxButton *btnOk = new wxButton(this, -1, wxT("Ok"), wxDefaultPosition, BUTTON_SIZE);
    wxButton *btnClose = new wxButton(this, -1, wxT("Закрыть"), wxDefaultPosition, BUTTON_SIZE);
    
    vBox->Add(panel);
    vBox->Add(btnOk);
    vBox->Add(btnClose);
    
    SetSizer(vBox);
    
    Centre();
}
