#include "defines.h"
#include "Canvas.h"
#include "Dialog.h"
#include "Form.h"


Dialog::Dialog(const wxString &title, const wxSize &size) : wxDialog(nullptr, wxID_ANY, title, wxDefaultPosition, size)
{
    Connect(wxEVT_MOVE, wxMoveEventHandler(Dialog::OnMove));
}


Dialog::~Dialog()
{
    TheForm->SetAdditionForm(nullptr);
}


void Dialog::OnMove(wxMoveEvent &)
{
    TheCanvas->Redraw();
}
