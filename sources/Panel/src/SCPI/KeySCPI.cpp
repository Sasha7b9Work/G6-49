// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Keyboard/Keyboard.h"
#include "SCPI/KeySCPI.h"


// :KEY:PRESS:
static pchar FuncKeyPress(pchar);
// :KEY:LONG:
static pchar FuncKeyLong(pchar);

static void HintKey(String *);


static pchar const keyNames[Key::Count + 1] =
{
    " NONE",       // None
    " F1",         // F1
    " F2",         // F2
    " F3",         // F3
    " F4",         // F4
    " 0",          // _0
    " 1",          // _1
    " 2",          // _2
    " 3",          // _3
    " 4",          // _4
    " 5",          // _5
    " 6",          // _6
    " 7",          // _7
    " 8",          // _8
    " 9",          // _9
    " DOT",        // Dot
    " MINUS",      // Minus
    " ESC",        // Esc
    " LEFT",       // Left
    " RIGHT",      // Right
    " CHANNELA",   // On1
    " CHANNELB",   // On2
    ""
};


const StructSCPI SCPI::key[] =
{
    SCPI_LEAF(":PRESS", FuncKeyPress, "Button press",            HintKey),
    SCPI_LEAF(":LONG",  FuncKeyLong,  "Press and hold a button", HintKey),
    SCPI_EMPTY()
};


static pchar FuncKeyPress(pchar buffer)
{
    for(int i = 0; i < Key::Count; i++)
    {
        pchar end = SCPI::BeginWith(buffer, keyNames[i]);
        if(end)
        {
            SCPI_PROLOG(end)

            Keyboard::AppendEvent((Key::E)i, Action::Down);
            Keyboard::AppendEvent((Key::E)i, Action::Up);

            SCPI_EPILOG(end)
        }
    }

    return nullptr;
}


static void HintKey(String *message)
{
    SCPI::ProcessHint(message, keyNames);
}


static pchar FuncKeyLong(pchar buffer)
{
    for(int i = 0; i < Key::Count; i++)
    {
        pchar end = SCPI::BeginWith(buffer, keyNames[i]);
        if(end)
        {
            SCPI_PROLOG(end)

            Keyboard::AppendEvent((Key::E)i, Action::Down);
            Keyboard::AppendEvent((Key::E)i, Action::Long);

            SCPI_EPILOG(end)
        }
    }

    return nullptr;
}


static pchar const regNames[Key::Count + 1] =
{
    " LEFT",   // ������� ������
    " RIGHT",  // ������� �����
    " PRESS",  // �������
    ""
};


pchar SCPI::FuncReg(pchar buffer)
{
    for (int i = 0; i < 3; i++)
    {
        pchar end = SCPI::BeginWith(buffer, regNames[i]);
        if (end)
        {
            SCPI_PROLOG(end)

            Keyboard::AppendEvent((Key::E)(i + Key::RotateLeft), Action::Down);
            Keyboard::AppendEvent((Key::E)(i + Key::RotateLeft), Action::Up);

            SCPI_EPILOG(end)
        }
    }

    return nullptr;
}


void SCPI::HintReg(String *)
{

}
