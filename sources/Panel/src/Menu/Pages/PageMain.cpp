// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "Menu/Pages/Pages.h"


DEF_PAGE_3(pageMain,
           "лемч", "MENU",
           "", "",
           PageSignals::self,
           PageFrequencyCounter::self,
           PageService::self,
           Page::Main, nullptr, Item::FuncActive, Page::FuncEnter, FuncOnKey, FuncBeforeDraw
)

Page *PageMain::self = reinterpret_cast<Page *>(const_cast<PageBase *>(&pageMain));
