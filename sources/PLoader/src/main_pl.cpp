#include "defines.h"
#include "common/Interface_p.h"
#include "common/Messages_l.h"
#include "Updater_pl.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "Keyboard/Keyboard.h"
#include "Utils/Debug.h"


// True будет означать, что запрос на обновление уже послан
static bool alreadyRequest = false;


int main()
{
    CPU::Init();
    Timer::Init();
    HAL_LTDC::Init();
    Keyboard::Init();

    while(true)
    {
        Updater::Update();

        if(!Keyboard::BufferIsEmpty() && !alreadyRequest)
        {
            alreadyRequest = true;
            Message::RequestUpgrade().Transmit();
        }

        PInterface::Update();
    }
}
