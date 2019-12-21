#include "Keyboard/Keyboard.h"


void Frame::OnKeyDown(wxCommandEvent &event)
{
    Keyboard::AppendEvent(static_cast<KeyEvent::E>(event.GetId()), KeyEvent::Action::Down);
    event.Skip();
}


void Frame::OnKeyUp(wxCommandEvent &event) //-V524
{
    Keyboard::AppendEvent(static_cast<KeyEvent::E>(event.GetId()), KeyEvent::Action::Up);
    event.Skip();
}
