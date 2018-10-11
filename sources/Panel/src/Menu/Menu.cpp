#include "Menu.h"
#include "Menu/Hint.h"
#include "Menu/MenuItems.h"
#include "Display/InputWindow.h"
#include "Generator/Generator.h"
#include "Generator/Signals.h"
#include "Hardware/CPU.h"
#include "Utils/Math.h"
#include "Menu/Pages/PageSignals.h"
#include "Menu/Pages/PageFrequencyCounter.h"
#include "Menu/Pages/PageService.h"
#include "Menu/Pages/PageDebug.h"
#include "Menu/Pages/AddPageInput.h"
#include "Settings/Settings.h"
#include "Utils/Debug.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Item *Menu::pressedItem = 0;
Item *Menu::openedItem = 0;
Page *Menu::oldPage = 0;
Item *Menu::currentItem = 0;

extern const PageBase pSignals;

DEF_PAGE_3( mainPageMenu,
    "лемч", "MENU",
    "", "",
    *PageSignals::pointer,
    *PageFrequencyCounter::pointer,
    *PageService::pointer,
    Page::Main, 0, FuncActive, FuncPress, FuncOnKey
)

Page *Menu::mainPage = (Page *)&mainPageMenu;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Menu::Init()
{
    PageSignals::Init();
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::Update()
{
    while (!CPU::Keyboard::BufferIsEmpty())
    {
        Control control = CPU::Keyboard::GetNextControl();

        ProcessContorl(control);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::ProcessContorl(Control control)
{
    if(ProcessOutputs(control))
    {
    }
    else if (GetOpenedItem())
    {
        GetOpenedItem()->Press(control);
    }
    else if (Hint::ProcessControl(control))
    {
    }
    else if(Menu::GetCurrentItem())
    {
        Menu::GetCurrentItem()->Press(control);
    }
    else if (CURRENT_PAGE->Press(control))
    {
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Menu::ProcessOutputs(Control control)
{
    if (control.action.IsRelease())
    {
        if (control.Is(Control::On1))
        {
            if (!WAVE(Chan::A).StartModeIsSingle())
            {
                SWITCH_CHANNEL_A;
            }
            Generator::EnableChannel(Chan::A, CHANNEL_ENABLED(Chan::A));
            return true;
        }
        else if (control.Is(Control::On2))
        {
            if (!WAVE(Chan::B).StartModeIsSingle())
            {
                SWITCH_CHANNEL_B;
            }
            Generator::EnableChannel(Chan::B, CHANNEL_ENABLED(Chan::B));
            return true;
        }
    }

    return false;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Menu::RegIsControlSubPages()
{
    return GetOpenedItem() == 0 && oldPage == 0 && CURRENT_PAGE && CURRENT_PAGE->NumSubPages() != 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Menu::GetPosition(Page *page)
{
    Page *keeper = page->Keeper();

    if(keeper == 0)
    {
        return 0;
    }

    for(int i = 0; i < keeper->NumItems(); i++)
    {
        if(page == keeper->items[i])
        {
            return i;
        }
    }

    return -1;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::SetAdditionPage(Page *page)
{
    oldPage = CURRENT_PAGE;
    CURRENT_PAGE = page;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::ResetAdditionPage()
{
    CURRENT_PAGE = oldPage;
    oldPage = 0;
}
