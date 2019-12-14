#include "Hardware/Keyboard.h"

static KeyEvent commands[10];
static int pointer = 0;


static void AddKeyboardEvent(KeyEvent &event)
{
    commands[pointer++] = event;
}



void Keyboard::Init()
{

}


void Keyboard::Draw()
{

}


bool Keyboard::BufferIsEmpty()
{
    return pointer == 0;
}


KeyEvent Keyboard::GetNextControl()
{
    KeyEvent retValue;

    if(BufferIsEmpty())
    {
        retValue.value = KeyEvent::None;
    }
    else
    {
        retValue = commands[0];
        for(int i = 1; i < pointer; i++)
        {
            commands[i - 1] = commands[i];
        }
        --pointer;
    }

    return retValue;
}


void Frame::OnKeyDown(wxCommandEvent &event)
{
    KeyEvent e(static_cast<KeyEvent::E>(event.GetId()), KeyEvent::Action::Down);
    AddKeyboardEvent(e);
    event.Skip();
}


void Frame::OnKeyUp(wxCommandEvent &event) //-V524
{
    KeyEvent e(static_cast<KeyEvent::E>(event.GetId()), KeyEvent::Action::Up);
    AddKeyboardEvent(e);
    event.Skip();
}
