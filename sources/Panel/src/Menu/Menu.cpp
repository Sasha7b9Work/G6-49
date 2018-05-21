#include "Menu.h"
#include "globals.h"
#include "Menu/MenuItems.h"
#include "Display/InputWindow.h"
#include "Display/InputWindowStruct.h"
#include "Generator.h"
#include "Hardware/CPU.h"
#include "Utils/Math.h"
#include "Menu/Pages/PageSettings.h"
#include "Menu/Pages/PageSettings2.h"
#include "Menu/Pages/PageFrequencyCounter.h"
#include "Menu/Pages/PageService.h"
#include "Menu/Pages/PageDebug.h"
#include "Menu/Pages/PageUSB.h"
#include "Settings/Settings.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Control *Menu::itemUnderKey = 0;
Control *Menu::openedItem = 0;
Control *Menu::itemHint = 0;
PanelControl Menu::panelControlHint = B_None;

const Page *menu[NUM_PAGES] =
{
    PageSignals::pointer,
    PageSignals2::pointer,
    PageFrequencyCounter::pointer,
    PageUSB::pointer,
    PageService::pointer,
    PageDebug::pointer
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Menu::Init(void)
{
    PageSignals::Init();
    PageSignals::OnPress_Form();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
Control *Menu::ItemHint()
{
    return itemHint;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
PanelControl Menu::PanelControlHint()
{
    return panelControlHint;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::Update(void)
{
    while (!CPU::Keyboard::BufferIsEmpty())
    {
        StructControl control = CPU::Keyboard::GetNextControl();
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
void Menu::ProcessControl(StructControl strContr)
{
    PanelControl key = strContr.key;
    TypePress pressed = strContr.typePress;

    if(key == B_ESC && pressed == Long)
    {
        IN_MODE_SHOW_HINTS++;
        itemHint = 0;
        panelControlHint = B_None;
    }
    
    if(IN_MODE_SHOW_HINTS)
    {
        if(key >= B_F1 && key <= B_F5)
        {
            itemHint = CurrentPage()->Item(key - B_F1);
            panelControlHint = B_None;
        }
        else
        {
            panelControlHint = key;
            itemHint = 0;
        }
    }
    else if (openedItem && (key == REG_LEFT || key == REG_RIGHT || key == REG_BTN || key == B_ESC || key == B_LEFT || key == B_RIGHT))
    {
        openedItem = openedItem->Press(strContr);
    }
    else if (key >= B_F1 && key <= B_F5)
    {
        openedItem = CurrentPage()->Item(key - B_F1)->Press(strContr);
    }
    else if (key == REG_LEFT)
    {
        if (RegIsControlPages())
        {
            if (CURRENT_PAGE > 0)
            {
                --CURRENT_PAGE;
            }
        }
    }
    else if (key == REG_RIGHT)
    {
        if (RegIsControlPages())
        {
            if (CURRENT_PAGE < NUM_PAGES - 1)
            {
                ++CURRENT_PAGE;
            }
        }
    }
    else if (pressed == Up || pressed == Long)
    {
        if(key == B_ON1)
        {
            CHANNEL_ENABLED_A = !CHANNEL_ENABLED_A;
            Generator::EnableChannel(A, CHANNEL_ENABLED_A);
        }
        else if (key == B_ON2)
        {
            CHANNEL_ENABLED_B = !CHANNEL_ENABLED_B;
            Generator::EnableChannel(B, CHANNEL_ENABLED_B);
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Menu::RegIsControlPages(void)
{
    return openedItem == 0 && ADDITION_PAGE_IS_NONE;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
const char *Menu::NameCurrentParameter(void)
{
    return PageSignals::cpParameters->NameCurrentSubItem();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
Control *Menu::OpenedItem()
{
    return openedItem;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
Control *Menu::CurrentItem()
{
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
Page *Menu::CurrentPage(void)
{
    volatile int8 num = CURRENT_PAGE;

    if (num == 1)
    {
        num = num;
    }

    Page *page = (Page *)menu[num];

    return (ADDITION_PAGE_IS_NONE ? page : ADDITION_PAGE);
}
