// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Painter.h"
#include "Menu/Pages/Pages.h"
#include "Settings/Settings.h"


static int16 redChanA = 0;
static int16 greenChanA = 0;
static int16 blueChanA = 0;

static int16 redChanB = 0;
static int16 greenChanB = 0;
static int16 blueChanB = 0;

static int16 redMenu = 0;
static int16 greenMenu = 0;
static int16 blueMenu = 0;


void PageDebug::Colors::Init()
{
    redChanA = (int16)R_FROM_COLOR(Color::CHAN_A.GetRGB());
    greenChanA = (int16)G_FROM_COLOR(Color::CHAN_A.GetRGB());
    blueChanA = (int16)B_FROM_COLOR(Color::CHAN_A.GetRGB());

    redChanB = (int16)R_FROM_COLOR(Color::CHAN_B.GetRGB());
    greenChanB = (int16)G_FROM_COLOR(Color::CHAN_B.GetRGB());
    blueChanB = (int16)B_FROM_COLOR(Color::CHAN_B.GetRGB());

    redMenu = (int16)R_FROM_COLOR(Color::MENU_ITEM.GetRGB());
    greenMenu = (int16)G_FROM_COLOR(Color::MENU_ITEM.GetRGB());
    blueMenu = (int16)B_FROM_COLOR(Color::MENU_ITEM.GetRGB());
}


static void OnChanged()
{
    COLOR(Color::CHAN_A.value) = MAKE_COLOR(redChanA, greenChanA, blueChanA);
    COLOR(Color::CHAN_B.value) = MAKE_COLOR(redChanB, greenChanB, blueChanB);
    COLOR(Color::MENU_ITEM.value) = MAKE_COLOR(redMenu, greenMenu, blueMenu);

    Painter::LoadPalette();
}


DEF_GOVERNOR( gRedChanA,
    "Красный", "Red",
    "", "",
    redChanA, 0, 255, *PageDebug::Colors::ChanA::self, Item::EFuncActive, OnChanged, EmptyFuncVV, EmptyFuncVB);


DEF_GOVERNOR(gGreenChanA,
    "Зелёный", "Green",
    "", "",
    greenChanA, 0, 255, *PageDebug::Colors::ChanA::self, Item::EFuncActive, OnChanged, EmptyFuncVV, EmptyFuncVB);

DEF_GOVERNOR(gBlueChanA,
    "Синий", "Blue",
    "", "",
    blueChanA, 0, 255, *PageDebug::Colors::ChanA::self, Item::EFuncActive, OnChanged, EmptyFuncVV, EmptyFuncVB);


DEF_PAGE_3( pChanA, //-V641
    "Канал 1", "Channel 1", //-V641
    "", "",
    &gRedChanA,
    &gGreenChanA,
    &gBlueChanA,
    Page::Color_ChanA, PageDebug::Colors::self, Item::EFuncActive, Page::EFuncEnter, Item::EFuncOnKey, Page::EFuncAfterDraw
)



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_GOVERNOR(gRedChanB,
    "Красный", "Red",
    "", "",
    redChanB, 0, 255, *PageDebug::Colors::ChanB::self, Item::EFuncActive, OnChanged, EmptyFuncVV, EmptyFuncVB);


DEF_GOVERNOR(gGreenChanB,
    "Зелёный", "Green",
    "", "",
    greenChanB, 0, 255, *PageDebug::Colors::ChanB::self, Item::EFuncActive, OnChanged, EmptyFuncVV, EmptyFuncVB);

DEF_GOVERNOR(gBlueChanB,
    "Синий", "Blue",
    "", "",
    blueChanB, 0, 255, *PageDebug::Colors::ChanB::self, Item::EFuncActive, OnChanged, EmptyFuncVV, EmptyFuncVB);


DEF_PAGE_3(pChanB, //-V641
    "Канал 2", "Channel 2", //-V641
    "", "",
    &gRedChanB,
    &gGreenChanB,
    &gBlueChanB,
    Page::Color_ChanA, PageDebug::Colors::self, Item::EFuncActive, Page::EFuncEnter, Item::EFuncOnKey, Page::EFuncAfterDraw
)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_GOVERNOR(gRedMenu,
    "Красный", "Red",
    "", "",
    redMenu, 0, 255, *PageDebug::Colors::Menu::self, Item::EFuncActive, OnChanged, EmptyFuncVV, EmptyFuncVB);


DEF_GOVERNOR(gGreenMenu,
    "Зелёный", "Green",
    "", "",
    greenMenu, 0, 255, *PageDebug::Colors::Menu::self, Item::EFuncActive, OnChanged, EmptyFuncVV, EmptyFuncVB);

DEF_GOVERNOR(gBlueMenu,
    "Синий", "Blue",
    "", "",
    blueMenu, 0, 255, *PageDebug::Colors::Menu::self, Item::EFuncActive, OnChanged, EmptyFuncVV, EmptyFuncVB);


DEF_PAGE_3(pMenu, //-V641
    "Меню", "Menu", //-V641
    "", "",
    &gRedMenu,
    &gGreenMenu,
    &gBlueMenu,
    Page::Color_ChanA, PageDebug::Colors::self, Item::EFuncActive, Page::EFuncEnter, Item::EFuncOnKey, Page::EFuncAfterDraw
)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_3(pColors,
    "Цвета", "Colors",
    "", "",
    PageDebug::Colors::ChanA::self,
    PageDebug::Colors::ChanB::self,
    PageDebug::Colors::Menu::self,
    Page::Colors, PageDebug::self, Item::EFuncActive, Page::EFuncEnter, Item::EFuncOnKey, Page::EFuncAfterDraw
)


Page *PageDebug::Colors::self = (Page *)&pColors;
Page *PageDebug::Colors::ChanA::self = (Page *)&pChanA;
Page *PageDebug::Colors::ChanB::self = (Page *)&pChanB;
Page *PageDebug::Colors::Menu::self = (Page *)&pMenu;
