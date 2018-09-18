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


DEF_PAGE_3( mainPageMenu,
    "лемч", "MENU",
    "", "",
    PageSignals::pointer,
    PageFrequencyCounter::pointer,
    PageService::pointer,
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
    else if(CURRENT_PAGE && CURRENT_PAGE->ProcessingControl(key))
    {
    }
    else if (openedItem && (key.Is(Control::Reg::Left) || key.Is(Control::Reg::Right) || key.Is(Control::Reg::Button) || key.Is(Control::Esc) || 
                            key.Is(Control::Left) || key.Is(Control::Right)))
    {
        openedItem = openedItem->Press(key);
    }
    else if (key >= Control::F1 && key <= Control::F5)
    {
        if(CURRENT_PAGE)
        {
            openedItem = CURRENT_PAGE->GetItem(key - Control::F1)->Press(key);
        }
    }
    else if (key.Is(Control::Reg::Left))
    {
        if (RegIsControlSubPages())
        {
            if (CURRENT_PAGE > 0)
            {
                --CURRENT_PAGE;
            }
        }
    }
    else if (key.Is(Control::Reg::Right))
    {
        if (RegIsControlSubPages())
        {
            //if (CURRENT_PAGE < NUM_PAGES - 1)
            //{
            //    ++CURRENT_PAGE;
            //}
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
bool Menu::RegIsControlSubPages()
{
    return openedItem == 0 && ADDITION_PAGE_IS_NONE && CURRENT_PAGE && CURRENT_PAGE->NumSubPages() != 0;
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
int Menu::GetPosition(Page *page)
{
    Page *keeper = (Page *)KEEPER(page);

    if(keeper == 0)
    {
        return 0;
    }

    for(int i = 0; i < keeper->NumItems(); i++)
    {
        if(page == keeper->GetItem(i))
        {
            return i;
        }
    }

    return -1;
}
