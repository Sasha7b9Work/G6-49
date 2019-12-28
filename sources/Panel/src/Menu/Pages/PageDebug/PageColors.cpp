#include "defines.h"
#include "Menu/Pages/PageDebug/PageDebug.h"
#include "Settings/Settings.h"


static int16 redChanA = static_cast<int16>(R_FROM_COLOR(COLOR(Color::CHAN_A.value)));
static int16 greenChanA = static_cast<int16>(G_FROM_COLOR(COLOR(Color::CHAN_A.value)));
static int16 blueChanA = static_cast<int16>(B_FROM_COLOR(COLOR(Color::CHAN_A.value)));

static int16 redChanB = static_cast<int16>(R_FROM_COLOR(COLOR(Color::CHAN_B.value)));
static int16 greenChanB = static_cast<int16>(G_FROM_COLOR(COLOR(Color::CHAN_B.value)));
static int16 blueChanB = static_cast<int16>(B_FROM_COLOR(COLOR(Color::CHAN_B.value)));

static int16 redMenu = static_cast<int16>(R_FROM_COLOR(COLOR(Color::MENU_ITEM.value)));
static int16 greenMenu = static_cast<int16>(G_FROM_COLOR(COLOR(Color::MENU_ITEM.value)));
static int16 blueMenu = static_cast<int16>(B_FROM_COLOR(COLOR(Color::MENU_ITEM.value)));


static void OnChanged()
{

}


DEF_GOVERNOR( gRedChanA,
    "Красный",
    "",
    redChanA, 0, 255, *PageDebug::Colors::ChanA::self, Item::FuncActive, OnChanged, EmptyFuncVV, EmptyFuncVB);


DEF_GOVERNOR(gGreenChanA,
    "Зелёный",
    "",
    greenChanA, 0, 255, *PageDebug::Colors::ChanA::self, Item::FuncActive, OnChanged, EmptyFuncVV, EmptyFuncVB);

DEF_GOVERNOR(gBlueChanA,
    "Синий",
    "",
    blueChanA, 0, 255, *PageDebug::Colors::ChanA::self, Item::FuncActive, OnChanged, EmptyFuncVV, EmptyFuncVB);


DEF_PAGE_3( pChanA,
    "Канал 1",
    "",
    &gRedChanA,
    &gGreenChanA,
    &gBlueChanA,
    Page::Color_ChanA, PageDebug::Colors::self, Item::FuncActive, FuncPress, FuncOnKey, Page::FuncDraw
)



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_GOVERNOR(gRedChanB,
    "Красный",
    "",
    redChanB, 0, 255, *PageDebug::Colors::ChanB::self, Item::FuncActive, OnChanged, EmptyFuncVV, EmptyFuncVB);


DEF_GOVERNOR(gGreenChanB,
    "Зелёный",
    "",
    greenChanB, 0, 255, *PageDebug::Colors::ChanB::self, Item::FuncActive, OnChanged, EmptyFuncVV, EmptyFuncVB);

DEF_GOVERNOR(gBlueChanB,
    "Синий",
    "",
    blueChanB, 0, 255, *PageDebug::Colors::ChanB::self, Item::FuncActive, OnChanged, EmptyFuncVV, EmptyFuncVB);


DEF_PAGE_3(pChanB,
    "Канал 1",
    "",
    &gRedChanB,
    &gGreenChanB,
    &gBlueChanB,
    Page::Color_ChanA, PageDebug::Colors::self, Item::FuncActive, FuncPress, FuncOnKey, Page::FuncDraw
)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_GOVERNOR(gRedMenu,
    "Красный",
    "",
    redMenu, 0, 255, *PageDebug::Colors::Menu::self, Item::FuncActive, OnChanged, EmptyFuncVV, EmptyFuncVB);


DEF_GOVERNOR(gGreenMenu,
    "Зелёный",
    "",
    greenMenu, 0, 255, *PageDebug::Colors::Menu::self, Item::FuncActive, OnChanged, EmptyFuncVV, EmptyFuncVB);

DEF_GOVERNOR(gBlueMenu,
    "Синий",
    "",
    blueMenu, 0, 255, *PageDebug::Colors::Menu::self, Item::FuncActive, OnChanged, EmptyFuncVV, EmptyFuncVB);


DEF_PAGE_3(pMenu,
    "Канал 1",
    "",
    &gRedMenu,
    &gGreenMenu,
    &gBlueMenu,
    Page::Color_ChanA, PageDebug::Colors::self, Item::FuncActive, FuncPress, FuncOnKey, Page::FuncDraw
)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_3(pColors,
    "Цвета",
    "",
    PageDebug::Colors::ChanA::self,
    PageDebug::Colors::ChanB::self,
    PageDebug::Colors::Menu::self,
    Page::Colors, PageDebug::self, Item::FuncActive, FuncPress, FuncOnKey, Page::FuncDraw
)


Page *PageDebug::Colors::self = reinterpret_cast<Page *>(const_cast<PageBase *>(&pColors));
Page *PageDebug::Colors::ChanA::self = reinterpret_cast<Page *>(const_cast<PageBase *>(&pChanA));
Page *PageDebug::Colors::ChanB::self = reinterpret_cast<Page *>(const_cast<PageBase *>(&pChanB));
Page *PageDebug::Colors::Menu::self = reinterpret_cast<Page *>(const_cast<PageBase *>(&pMenu));
