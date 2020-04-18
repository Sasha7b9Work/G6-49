#include "Menu/Menu.h"
#include "Menu/Hint.h"
#include "Menu/MenuItems.h"
#include "Generator/Signals.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Keyboard/Keyboard.h"
#include "Menu/Pages/PageSignals/PageSignals.h"
#include "Menu/Pages/PageFrequencyCounter.h"
#include "Menu/Pages/PageService.h"
#include "Menu/Pages/PageDebug/PageDebug.h"
#include "Settings/Settings.h"


Item *Menu::pressedItem = nullptr;
static Page *oldPage = nullptr;
Item *Menu::openedItem = nullptr;
Item *Menu::currentItem = nullptr;

extern const PageBase pSignals;

DEF_PAGE_4( mainPageMenu,
    "МЕНЮ", "MENU",
    "", "",
    PageSignals::self,
    PageFrequencyCounter::self,
    PageService::self,
    Item::EmptyLight(),
    Page::Main, 0, Item::FuncActive, Page::FuncEnter, FuncOnKey, Page::FuncDraw
)

Page *Menu::mainPage = const_cast<Page *>(reinterpret_cast<const Page *>(&mainPageMenu));


void Menu::Init()
{
    PageSignals::Init();
    PageDebug::Colors::Init();
}


void Menu::Update()
{
    static uint timePress = 0;  // Время наступления последнего события. Если равно нулю, то настройки уже сохранены и сохранять их не требуется

    while (!Keyboard::BufferIsEmpty())
    {
        Key key = Keyboard::GetNextControl();

        if(OpenDebugPage(key))                              // Открываем страницу отладки, если распознали соотвествующую клавиатурную последовательность
        {
            continue;
        }

        if(key.IsUp())
        {
            key = key;
        }

        ProcessKey(key);

        timePress = TIME_MS;
    }

    const uint TIME_WAIT = 5000;

    if(timePress && (TIME_MS  - timePress) > TIME_WAIT)     // Сохраняем настройки, если прошло более TIME_WAIT мс
    {
        //Settings::Save();
        timePress = 0;
    }
}


void Menu::ProcessKey(const Key &key)
{
    if(ProcessOutputs(key))                     // Обработка включения/отключения каналов
    {
    }
    else if (GetOpenedItem())                   // Если раскрыт какой-либо элемент меню - передаём управление уему
    {
        GetOpenedItem()->Press(key);
    }
    else if(Menu::GetCurrentItem())             // Если какой-либо элемент меню является активным - передаём управление ему
    {
        Menu::GetCurrentItem()->Press(key);
    }
    else if (CURRENT_PAGE->Press(key))          // Иначе передаём управление текущей странице
    {
    }
}


bool Menu::ProcessOutputs(const Key &key)
{
    if (key.IsRelease())
    {
        if (key.Is(Key::On1))
        {
            if (!WAVE(Chan::A).StartModeIsSingle())
            {
                set.enabled[Chan::A] = !set.enabled[Chan::A];
            }
            PGenerator::EnableChannel(Chan::A, set.enabled[Chan::A]);
            return true;
        }
        else if (key.Is(Key::On2))
        {
            if (!WAVE(Chan::B).StartModeIsSingle())
            {
                set.enabled[Chan::B] = !set.enabled[Chan::B];
            }
            PGenerator::EnableChannel(Chan::B, set.enabled[Chan::B]);
            return true;
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
    CURRENT_PAGE->funcEnter(true);
}


void Menu::ResetAdditionPage()
{
    CURRENT_PAGE->funcEnter(false);
    CURRENT_PAGE = oldPage;
    oldPage = nullptr;
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


bool Menu::OpenDebugPage(const Key &key)
{
    if(CURRENT_PAGE != PageService::self)
    {
        return false;
    }

    static const Key keys[] =
    {
        { Key::_1, Key::Down },
        { Key::_1, Key::Up },
        { Key::_2, Key::Down },
        { Key::_2, Key::Up },
        { Key::_3, Key::Down },
        { Key::_3, Key::Up },
        { Key::_4, Key::Down },
        { Key::_4, Key::Up },
        { Key::_8, Key::Down },
        { Key::_8, Key::Up },
        { Key::_5, Key::Down },
        { Key::_5, Key::Up },
        { Key::Count}
    };

    if(Keyboard::Decoder::Decode(keys, key))
    {
        PageDebug::Enable();
        return true;
    }

    return false;
}
