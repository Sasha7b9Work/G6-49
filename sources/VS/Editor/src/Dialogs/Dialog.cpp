#include "defines.h"
#include "Canvas.h"
#include "Dialog.h"
#include "Form.h"


Dialog::Dialog(const wxString &title, const wxSize &size) : wxDialog(nullptr, wxID_ANY, title)
{
    Connect(wxEVT_MOVE, wxMoveEventHandler(Dialog::OnMove));
	SetClientSize(size);
}


Dialog::~Dialog()
{
    TheForm->SetAdditionForm(nullptr);
}


void Dialog::OnMove(wxMoveEvent &)
{
    TheCanvas->Redraw();
}
