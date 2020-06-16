#include "Menu/Menu.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Pages.h"


DEF_PAGE_4(mainPageMenu,
    "лемч", "MENU",
    "", "",
    PageSignals::self,
    PageFrequencyCounter::self,
    PageService::self,
    &Item::emptyLight,
    Page::Main, 0, Item::FuncActive, Page::FuncEnter, FuncOnKey, Page::FuncDraw
)

Page *PageMain::self = const_cast<Page *>(reinterpret_cast<const Page *>(&mainPageMenu));
