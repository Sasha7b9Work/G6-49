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
#include "Menu/Pages/AddPageInput.h"
#include "Settings/Settings.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Item *Menu::itemUnderKey = 0;
Item *Menu::openedItem_ = 0;
Page *Menu::oldPage = 0;

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
        if (CURRENT_PAGE == AddPageInput::pointer)
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
    if (key.action.Is(Control::Action::Up) || key.action.Is(Control::Action::Long))
    {
        if(key.Is(Control::On1))
        {
            SWITCH_CHANNEL_A;
            Generator::EnableChannel(Chan::A, CHANNEL_ENABLED(Chan::A));
            return;
        }
        else if (key.Is(Control::On2))
        {
            SWITCH_CHANNEL_B;
            Generator::EnableChannel(Chan::B, CHANNEL_ENABLED(Chan::B));
            return;
        }
    }

    if (Hint::ProcessControl(key))
    {
    }
    else if (CURRENT_PAGE->ProcessingControl(key))
    {
    }
    else if (CURRENT_PAGE->Press(key))
    {
    }
    else if(GetOpenedItem())
    {
        GetOpenedItem()->Press(key);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Menu::RegIsControlSubPages()
{
    return GetOpenedItem() == 0 && oldPage == 0 && CURRENT_PAGE && CURRENT_PAGE->NumSubPages() != 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
Item *Menu::GetOpenedItem()
{
    return openedItem_;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::SetOpenedItem(Item *item)
{
    openedItem_ = item;
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
