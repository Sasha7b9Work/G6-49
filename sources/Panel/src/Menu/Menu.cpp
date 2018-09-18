#include "Menu.h"
#include "Menu/Hint.h"
#include "Menu/MenuItems.h"
#include "Display/InputWindow.h"
#include "Generator/Generator.h"
#include "Hardware/CPU.h"
#include "Utils/Math.h"
#include "Menu/Pages/PageSignals.h"
#include "Menu/Pages/PageFrequencyCounter.h"
#include "Menu/Pages/PageService.h"
#include "Menu/Pages/PageDebug.h"
#include "Settings/Settings.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Item *Menu::itemUnderKey = 0;
Item *Menu::openedItem = 0;
//Control Menu::panelControlHint = None;

const Page *pages[NUM_PAGES] =
{
    PageSignals::pointer,
    PageFrequencyCounter::pointer,
    PageService::pointer
};


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
    else if (openedItem && (key.Is(Control::Reg::Left) || key.Is(Control::Reg::Right) || key.Is(Control::Reg::Button) || key.Is(Control::Esc) || 
                            key.Is(Control::Left) || key.Is(Control::Right)))
    {
        openedItem = openedItem->Press(key);
    }
    else if (key >= Control::F1 && key <= Control::F5)
    {
        openedItem = CurrentPage()->GetItem(key - Control::F1)->Press(key);
    }
    else if (key.Is(Control::Reg::Left))
    {
        if (RegIsControlPages())
        {
            if (CURRENT_PAGE > 0)
            {
                --CURRENT_PAGE;
            }
        }
    }
    else if (key.Is(Control::Reg::Right))
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
        if(key.Is(Control::On1))
        {
            SWITCH_CHANNEL_A;
            Generator::EnableChannel(Chan::A, CHANNEL_ENABLED(Chan::A));
        }
        else if (key.Is(Control::On2))
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
