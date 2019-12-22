#include "defines.h"
#include "Keyboard/Keyboard.h"
#include "SCPI/KeySCPI.h"
#include "SCPI/SCPI.h"


// :KEY:PRESS:
static const char *FuncKeyPress(const char *);
// :KEY:LONG:
static const char *FuncKeyLong(const char *);

static void HintKey(String *);


static const char *const keyNames[KeyEvent::Count + 1] =
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
    " ON1",        // On1
    " ON2",        // On2
    " REG_LEF",    // RegLeft
    " REG_RIGHT",  // RegRight
    " REG_BUTTON", // RegButton
    ""
};


const StructSCPI SCPI::key[] =
{
    SCPI_LEAF(":PRESS", FuncKeyPress, "Button press",            HintKey),
    SCPI_LEAF(":LONG",  FuncKeyLong,  "Press and hold a button", HintKey),
    SCPI_EMPTY()
};


static const char *FuncKeyPress(const char *buffer)
{
    for(int i = 0; i < KeyEvent::Count; i++)
    {
        const char *end = SCPI::BeginWith(buffer, keyNames[i]);
        if(end)
        {
            SCPI_PROLOG(end)

            Keyboard::AppendEvent(static_cast<KeyEvent::E>(i), KeyEvent::Action::Down);
            Keyboard::AppendEvent(static_cast<KeyEvent::E>(i), KeyEvent::Action::Up);

            SCPI_EPILOG(end)
        }
    }

    return nullptr;
}


static void HintKey(String *message)
{
    SCPI::ProcessHint(message, keyNames);
}


static const char *FuncKeyLong(const char *buffer)
{
    for(int i = 0; i < KeyEvent::Count; i++)
    {
        const char *end = SCPI::BeginWith(buffer, keyNames[i]);
        if(end)
        {
            SCPI_PROLOG(end)

            Keyboard::AppendEvent(static_cast<KeyEvent::E>(i), KeyEvent::Action::Down);
            Keyboard::AppendEvent(static_cast<KeyEvent::E>(i), KeyEvent::Action::Long);

            SCPI_EPILOG(end)
        }
    }

    return nullptr;
}
