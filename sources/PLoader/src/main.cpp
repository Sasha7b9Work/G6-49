#include "defines.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "Keyboard/Keyboard.h"


// Эта функция запускает процедуру обновления
static void Update();


int main()
{
    CPU::Init();
    Timer::Init();
    Keyboard::Init();

    uint start = TIME_MS;

    while (TIME_MS - start < 1000)
    {
        if(!Keyboard::BufferIsEmpty())
        {
            Update();
            break;
        }
    }
}


static void Update()
{

}
