#include "Menu.h"
#include "Menu/Hint.h"
#include "Menu/MenuItems.h"
#include "Display/InputWindow.h"
#include "Generator/Generator_p.h"
#include "Generator/Signals.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "Keyboard/Keyboard.h"
#include "Utils/Math.h"
#include "Menu/Pages/Include/PageSignals.h"
#include "Menu/Pages/PageFrequencyCounter.h"
#include "Menu/Pages/PageService.h"
#include "Menu/Pages/PageDebug.h"
#include "Menu/Pages/Addition/PageInput.h"
#include "Settings/Settings.h"
#include "Utils/Debug.h"


Item *Menu::pressedItem = nullptr;
static Page *oldPage = nullptr;
Item *Menu::openedItem = nullptr;
Item *Menu::currentItem = nullptr;

extern const PageBase pSignals;

DEF_PAGE_3( mainPageMenu,
    "МЕНЮ",
    "",
    *PageSignals::pointer,
    *PageFrequencyCounter::pointer,
    *PageService::pointer,
    Page::Main, 0, Item::FuncActive, FuncPress, FuncOnKey
)

Page *Menu::mainPage = const_cast<Page *>(reinterpret_cast<const Page *>(&mainPageMenu));


void Menu::Init()
{
    PageSignals::Init();
}


void Menu::Update()
{
    static uint timePress = 0;  // Время наступления последнего события. Если равно нулю, то настройки уже сохранены и сохранять их не требуется

    while (!Keyboard::BufferIsEmpty())
    {
        Key control = Keyboard::GetNextControl();

        ProcessContorl(control);

        timePress = TIME_MS;
    }

    const uint TIME_WAIT = 5000;

    if(timePress && (TIME_MS  - timePress) > TIME_WAIT)      // Сохраняем настройки, если прошло более TIME_WAIT мс
    {
        //Settings::Save();
        timePress = 0;
    }
}


void Menu::ProcessContorl(Key &control)
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
    else
    {
        // здесь ничего
    }
}


bool Menu::ProcessOutputs(Key &control)
{
    if (control.action.IsRelease())
    {
        if (control.Is(Key::On1))
        {
            if (!WAVE(Chan::A).StartModeIsSingle())
            {
                SWITCH_CHANNEL_A;
            }
            PGenerator::EnableChannel(Chan::A, CHANNEL_ENABLED(Chan::A));
            return true;
        }
        else if (control.Is(Key::On2))
        {
            if (!WAVE(Chan::B).StartModeIsSingle())
            {
                SWITCH_CHANNEL_B;
            }
            PGenerator::EnableChannel(Chan::B, CHANNEL_ENABLED(Chan::B));
            return true;
        }
        else
        {
            // здесь ничего
        }
    }

    return false;
}


bool Menu::RegIsControlSubPages()
{
    return (GetOpenedItem() == nullptr) && (oldPage == nullptr) && (CURRENT_PAGE) && (CURRENT_PAGE->NumSubPages() != 0);
}


int Menu::GetPosition(const Page *page)
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


void Menu::SetAdditionPage(Page *page)
{
    oldPage = CURRENT_PAGE;
    CURRENT_PAGE = page;
}


void Menu::ResetAdditionPage()
{
    CURRENT_PAGE = oldPage;
    oldPage = nullptr;
    InputWindow::DeInit();
}


Item* Menu::GetOpenedItem()
{
    return openedItem;
}


void Menu::SetOpenedItem(Item *item)
{
    openedItem = item;
}


void Menu::SetCurrentItem(Item *item)
{
    currentItem = item;
}


void Menu::ResetCurrentItem()
{
    currentItem = nullptr;
}


Item* Menu::GetCurrentItem()
{
    return currentItem;
}


void Menu::ResetOpenedItem()
{
    openedItem = nullptr;
}
