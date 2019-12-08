static KeyEvent commands[10];
static int pointer = 0;


static void AddKeyboardEvent(KeyEvent &event)
{
    commands[pointer++] = event;
}



void CPU::Keyboard::Init()
{

}


void CPU::Keyboard::Draw()
{

}


bool CPU::Keyboard::BufferIsEmpty()
{
    return pointer == 0;
}


KeyEvent CPU::Keyboard::GetNextControl()
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
}


void Frame::OnKeyUp(wxCommandEvent &event) //-V524
{
    KeyEvent e(static_cast<KeyEvent::E>(event.GetId()), KeyEvent::Action::Up);
    AddKeyboardEvent(e);
}
