// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "Menu/Menu.h"
#include "Menu/Hint.h"
#include "Menu/MenuItems.h"
#include "FDrive/FDrive_p.h"
#include "Generator/Signals.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Keyboard/Keyboard.h"
#include "Menu/Pages/Pages.h"
#include "Settings/Settings.h"


Item *Menu::pressedItem = nullptr;
static Page *oldPage = nullptr;
Item *Menu::openedItem = nullptr;
Item *Menu::currentItem = nullptr;

extern const PageBase pSignals;




void Menu::Init()
{
    PageSignals::Init();
    PageDebug::Colors::Init();
}


void Menu::Update()
{
    static uint time_press = 0;  // Время наступления последнего события. Если равно нулю, то настройки уже сохранены и сохранять их не требуется

    while (!Keyboard::BufferIsEmpty())
    {
        Control control = Keyboard::GetNextControl();

        if (PageDebug::SaveScreenToFlashIsEnabled() && control.key == Key::RegButton)
        {
            if (control.IsDown())
            {
                FDrive::SaveScreenToFlash();
            }

            continue;
        }

        if(OpenDebugPage(control))                              // Открываем страницу отладки, если распознали соотвествующую клавиатурную последовательность
        {
            continue;
        }

        ProcessKey(control);

        time_press = _TIME_MS;
    }

    const uint TIME_WAIT = 5000;

    if((time_press != 0) && ((_TIME_MS  - time_press) > TIME_WAIT))     // Сохраняем настройки, если прошло более TIME_WAIT мс
    {
        //Settings::Save();
        time_press = 0;
    }
}


void Menu::ProcessKey(const Control control) //-V801
{
                                                    // Закрываем составной параметр, если таковой открыт
    if (control.key == Key::Esc && control.IsRelease() && !PageTuneParameter::IsOpened() && CURRENT_FORM->CloseCompositeParameter())
    {
    }
    else if(ProcessOutputs(control))                // Обработка включения/отключения каналов
    {
    }
    else if (GetOpenedItem())                       // Если раскрыт какой-либо элемент меню - передаём управление уему
    {
        GetOpenedItem()->Press(control);
    }
    else if(Menu::GetCurrentItem())                 // Если какой-либо элемент меню является активным - передаём управление ему
    {
        Menu::GetCurrentItem()->Press(control);
    }
    else if (CURRENT_PAGE->Press(control))          // Иначе передаём управление текущей странице
    {
    }
}


bool Menu::ProcessOutputs(const Control control) //-V801
{
    if (control.IsRelease())
    {
        if (control.Is(Key::On1))
        {
            ProcessOutput(ChA);

            return true;
        }
        else if (control.Is(Key::On2))
        {
            ProcessOutput(ChB);

            return true;
        }
    }

    return false;
}


void Menu::ProcessOutput(const Chan &ch)
{
    if (WAVE(ch).StartModeIsSingle())
    {
        if (!ch.Enabled())
        {
            PGenerator::EnableChannel(ch, true);
        }
        else
        {
            PGenerator::SingleStart();
        }
    }
    else
    {
        PGenerator::EnableChannel(ch, !ch.Enabled());
    }
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


bool Menu::OpenDebugPage(const Control control) //-V801
{
    if(CURRENT_PAGE != PageService::self)
    {
        return false;
    }

    static const Control controls[] =
    {
        { Key::_1, Action::Down },
        { Key::_1, Action::Up },
        { Key::_2, Action::Down },
        { Key::_2, Action::Up },
        { Key::_3, Action::Down },
        { Key::_3, Action::Up },
        { Key::_4, Action::Down },
        { Key::_4, Action::Up },
        { Key::_8, Action::Down },
        { Key::_8, Action::Up },
        { Key::_5, Action::Down },
        { Key::_5, Action::Up },
        { Key::Count}
    };

    if(Keyboard::Decoder::Decode(controls, control))
    {
        PageDebug::Enable();
        return true;
    }

    return false;
}
