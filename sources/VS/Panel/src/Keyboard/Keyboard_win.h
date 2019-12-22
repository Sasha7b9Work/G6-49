#include "Keyboard/Keyboard.h"


void Frame::OnKeyDown(wxCommandEvent &event)
{
    Keyboard::AppendEvent(static_cast<Key::E>(event.GetId()), Key::Action::Down);
    event.Skip();
}


void Frame::OnKeyUp(wxCommandEvent &event) //-V524
{
    Keyboard::AppendEvent(static_cast<Key::E>(event.GetId()), Key::Action::Up);
    event.Skip();
}
