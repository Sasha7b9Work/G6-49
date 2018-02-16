#include "Menu.h"
#include "MenuItemsLogic.h"
#include "Display/InputWindowStruct.h"
#include "Generator/Generator.h"
#include "Keyboard/Keyboard.h"
#include "Pages/PageSignals.h"
#include "Pages/PageSignals2.h"
#include "Pages/PageService.h"
#include "Pages/PageDebug.h"
#include "Utils/Math.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Menu gMenu;

const Page *menu[NUM_PAGES] =
{
    &pSignals,
    &pSignals2,
    &pService,
    &pDebug
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Menu::Init(void)
{
    PanelSignals_Init();
    OnPress_Form();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::Update(void)
{
    while (!Keyboard_BufferIsEmpty())
    {
        StructControl control = Keyboard_GetNextControl();
        if (ADDITION_PAGE == &pInput)
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
    Control key = control.control;
    TypePress pressed = control.typePress;

    if (key == REG_A_LEFT)
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
            Item_Rotate(OPENED_ITEM, REG_A_LEFT);
        }
    }
    else if (key == REG_B_RIGHT)
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
            Item_Rotate(OPENED_ITEM, REG_B_RIGHT);
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
        else if (key >= B_F1 & key <= B_F4)
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
            Generator_EnableChannel(A, CHANNEL_ENABLED_A);
        }
        else if (key == B_ON2)
        {
            CHANNEL_ENABLED_B = !CHANNEL_ENABLED_B;
            Generator_EnableChannel(B, CHANNEL_ENABLED_B);
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Menu::RegIsControlPages(void)
{
    return OPENED_ITEM_IS_NONE && ADDITION_PAGE_IS_NONE;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
char *Menu::NameCurrentParameter(void)
{
    return ChoiceWaveParameter_CurrentName(&cpParameters);
}
