#pragma warning(push)
#pragma warning(disable:5039)
#include "stdafx.h"
#include "defines.h"
#pragma warning(pop)


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void init()
{
    CPU::Init();

    Timer::Init();

    Display::Init();

    Timer::PauseOnTime(1000);

    setCal.Load();

    set.Load();

    Menu::Init();
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void update()
{
    CPU::Update();

    Menu::Update();

    Display::Update();

    Interface::Update();
}

