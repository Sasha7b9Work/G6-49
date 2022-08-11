#include "Keyboard/Keyboard.h"


void Frame::OnKeyDown(wxCommandEvent &event)
{
    Keyboard::AppendEvent((Key::E)event.GetId(), Action::Down);
    event.Skip();
}


void Frame::OnKeyUp(wxCommandEvent &event) //-V524
{
    Keyboard::AppendEvent((Key::E)event.GetId(), Action::Up);
    event.Skip();
}
