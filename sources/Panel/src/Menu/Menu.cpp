#include "Menu.h"
#include "Menu/Hint.h"
#include "Menu/MenuItems.h"
#include "Display/InputWindow.h"
#include "Generator/Generator.h"
#include "Hardware/CPU.h"
#include "Utils/Math.h"
#include "Menu/Pages/PageSettings.h"
#include "Menu/Pages/PageSettings2.h"
#include "Menu/Pages/PageFrequencyCounter.h"
#include "Menu/Pages/PageFrequencyCounter2.h"
#include "Menu/Pages/PageFrequencyCounter3.h"
#include "Menu/Pages/PageService.h"
#include "Menu/Pages/PageDebug.h"
#include "Menu/Pages/PageUSB.h"
#include "Menu/Pages/PageRegisters.h"
#include "Settings/Settings.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Item *Menu::itemUnderKey = 0;
Item *Menu::openedItem = 0;
//Control Menu::panelControlHint = B_None;

const Page *pages[NUM_PAGES] =
{
    PageSignals::pointer,
    PageSignals2::pointer,
    PageRegisters::pointer,
    PageFrequencyCounter::pointer,
    PageFrequencyCounter3::pointer,
    PageFrequencyCounter2::pointer,
    PageUSB::pointer,
    PageService::pointer,
    PageDebug::pointer
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Menu::Update()
{
    while (!CPU::Keyboard::BufferIsEmpty())
    {
        Control control = CPU::Keyboard::GetNextControl();
        if (ADDITION_PAGE_IS_INPUT)
        {
            InputWindow::ProcessContorl(control);
        }
        else
        {
            ProcessControl(control);
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::ProcessControl(Control key)
{
    if(Hint::ProcessControl(key))
    {
    }
    else if(Menu::CurrentPage()->ProcessingControl(key))
    {
    }
    else if (openedItem && (key.Is(Control::REG_LEFT) || key.Is(Control::REG_RIGHT) || key.Is(Control::REG_BTN) || key.Is(Control::B_ESC) || 
                            key.Is(Control::B_LEFT) || key.Is(Control::B_RIGHT)))
    {
        openedItem = openedItem->Press(key);
    }
    else if (key >= Control::B_F1 && key <= Control::B_F5)
    {
        openedItem = CurrentPage()->GetItem(key - Control::B_F1)->Press(key);
    }
    else if (key.Is(Control::REG_LEFT))
    {
        if (RegIsControlPages())
        {
            if (CURRENT_PAGE > 0)
            {
                --CURRENT_PAGE;
            }
        }
    }
    else if (key.Is(Control::REG_RIGHT))
    {
        if (RegIsControlPages())
        {
            if (CURRENT_PAGE < NUM_PAGES - 1)
            {
                ++CURRENT_PAGE;
            }
        }
    }
    else if (key.action.Is(Control::Action::Up) || key.action.Is(Control::Action::Long))
    {
        if(key.Is(Control::B_ON1))
        {
            SWITCH_CHANNEL_A;
            Generator::EnableChannel(Chan::A, CHANNEL_ENABLED(Chan::A));
        }
        else if (key.Is(Control::B_ON2))
        {
            SWITCH_CHANNEL_B;
            Generator::EnableChannel(Chan::B, CHANNEL_ENABLED(Chan::B));
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Menu::RegIsControlPages()
{
    return openedItem == 0 && ADDITION_PAGE_IS_NONE;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
const char *Menu::NameCurrentParameter()
{
    return PageSignals::cpParameters->NameCurrentSubItem();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
Item *Menu::OpenedItem()
{
    return openedItem;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
Item *Menu::CurrentItem()
{
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
Page *Menu::CurrentPage()
{
    volatile int8 num = CURRENT_PAGE;

    if (num == 1)
    {
        num = num;
    }

    Page *page = (Page *)pages[num];

    return (ADDITION_PAGE_IS_NONE ? page : ADDITION_PAGE);
}
