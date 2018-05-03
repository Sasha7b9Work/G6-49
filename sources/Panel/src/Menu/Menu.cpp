#include "Menu.h"
#include "Menu/MenuItems.h"
#include "Display/InputWindow.h"
#include "Display/InputWindowStruct.h"
#include "Generator/Generator.h"
#include "Hardware/CPU.h"
#include "Utils/Math.h"
#include "Menu/Pages/PageSettings.h"
#include "Menu/Pages/PageSettings2.h"
#include "Menu/Pages/PageService.h"
#include "Menu/Pages/PageDebug.h"
#include "Settings/Settings.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Control *Menu::itemUnderKey = 0;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const Page *menu[NUM_PAGES] =
{
    PageSignals::pointer,
    PageSignals2::pointer,
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
void Menu::ProcessControl(StructControl control)
{
    PanelControl key = control.control;
    TypePress pressed = control.typePress;

    if (key == REG_LEFT)
    {
        if (RegIsControlPages())
        {
            if (CURRENT_PAGE > 0)
            {
                --CURRENT_PAGE;
            }
        }
        else
        {
            Item_Rotate(OPENED_ITEM, REG_LEFT);
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
        else
        {
            Item_Rotate(OPENED_ITEM, REG_RIGHT);
        }
    }

    if (pressed == TypePress_Release || pressed == TypePress_LongPress)
    {
        if (OPENED_ITEM && pressed == TypePress_LongPress)
        {
            if (key == ButtonForItem(OPENED_ITEM))
            {
                OPENED_ITEM = 0;
            }
        }
        else if (key >= B_F1 && key <= B_F4)
        {
            void *address = ItemFromPage(key - B_F1);
            if (ItemIsChoice(address))
            {
                Choice_Press((Choice *)address, pressed);
            }
            else if (ItemIsButton(address))
            {
                Button_Press((Button *)address);
            }
            else if (ItemIsSButton(address))
            {
                SButton_Press((SButton *)address);
            }
            else if (ItemIsChoiceParameter(address))
            {
                ChoiceWaveParameter_Press((ChoiceParameter *)address, pressed);
            }
        }
        else if(key == B_ON1)
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
    return OPENED_ITEM_IS_NONE && ADDITION_PAGE_IS_NONE;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
const char *Menu::NameCurrentParameter(void)
{
    return ChoiceWaveParameter_CurrentName(PageSignals::cpParameters);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
Control *Menu::OpenedItem()
{
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
Control *Menu::CurrentItem()
{
    return 0;
}
