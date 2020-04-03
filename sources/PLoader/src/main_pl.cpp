#include "defines.h"
#include "common/Messages_pl.h"
#include "Updater_pl.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "Interface/Interface_pl.h"
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
        DEBUG_POINT_0;

        Updater::Update();

        DEBUG_POINT_0;

        if(!Keyboard::BufferIsEmpty() && !alreadyRequest)
        {
            DEBUG_POINT_0;
            alreadyRequest = true;
            Message::RequestUpdate().Transmit();
            DEBUG_POINT_0;
        }
        
        DEBUG_POINT_0;
        PInterface::Update();
        DEBUG_POINT_0;
    }
}
