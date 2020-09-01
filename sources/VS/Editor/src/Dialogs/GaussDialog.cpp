#include "defines.h"
#include "Controls/TextControl.h"
#include "Dialogs/GaussDialog.h"


GaussDialog::GaussDialog() : Dialog(wxT("Параметры гауссовой функции"), true)
{
    wxPoint pos(20, 30);
    wxSize size(100, 20);

    wxBoxSizer *vBox = new wxBoxSizer(wxVERTICAL);

    tcA = new TextControlFloat(this, -100.0F, 100.0F, this, wxID_ANY, wxT("0"), pos, size);
    tcB = new TextControlFloat(this, -100.0F, 100.0F, this, wxID_ANY, wxT("0"), pos, size);
    tcC = new TextControlFloat(this, -100.0F, 100.0F, this, wxID_ANY, wxT("0"), pos, size);

    wxSizerFlags flags;
    flags.Expand().Border(wxLEFT, 45);

    vBox->AddSpacer(15);
    vBox->Add(tcA, flags);
    vBox->AddSpacer(15);
    vBox->Add(tcB, flags);
    vBox->AddSpacer(15);
    vBox->Add(tcC, flags);
    vBox->AddSpacer(10);

    SetBoxSizer(vBox, { 175, 120 });

    new wxStaticText(this, wxID_ANY, wxT("A"), { 25, 18 });
    new wxStaticText(this, wxID_ANY, wxT("B"), { 25, 52 });
    new wxStaticText(this, wxID_ANY, wxT("C"), { 25, 88 });
}


void GaussDialog::SendAdditionForm()
{

}
