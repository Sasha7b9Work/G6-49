#include "defines.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "Interface/Interface_pl.h"
#include "Keyboard/Keyboard.h"


int main()
{
    CPU::Init();
    Timer::Init();
    Keyboard::Init();


    while(1)
    {
        PInterface::Update();
    }
}
