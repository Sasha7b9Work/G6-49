// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Messages.h"
#include "Menu/Pages/Pages.h"
#include "Settings/Settings.h"


static void OnPress_PE15(bool active)
{
    if (active)
    {
        Message::RegCPU(4, 15, set.reg.PE15).Transmit();
    }
}

DEF_CHOICE_2( cPE15,
    "PE15", "PE15",
    "", "",
    "0", "0", "", "",
    "1", "1", "", "",
    set.reg.PE15, (*PageDebug::SubRange::self), Item::EFuncActive, OnPress_PE15, Item::EFuncDraw
)


static void OnPress_PB10(bool active)
{
    if (active)
    {
        Message::RegCPU(1, 10, set.reg.PB10).Transmit();
    }
}

DEF_CHOICE_2( cPB10,
   "PB10", "PB10",
    "", "",
    "0", "0", "", "",
    "1", "1", "", "",
    set.reg.PB10, (*PageDebug::SubRange::self), Item::EFuncActive, OnPress_PB10, Item::EFuncDraw
)


static void OnPress_PF0(bool active)
{
    if (active)
    {
        Message::RegCPU(5, 0, set.reg.PF0).Transmit();
    }
}

DEF_CHOICE_2( cPF0,
    "PF0", "PF0",
    "", "",
    "0", "0", "", "",
    "1", "1", "", "",
    set.reg.PF0, (*PageDebug::SubRange::self), Item::EFuncActive, OnPress_PF0, Item::EFuncDraw
)


static void OnPress_PF5(bool active)
{
    if (active)
    {
        Message::RegCPU(5, 5, set.reg.PF5).Transmit();
    }
}

DEF_CHOICE_2( cPF5,
    "PF5", "PF5",
    "", "",
    "0", "0", "", "",
    "1", "1", "", "",
    set.reg.PF5, (*PageDebug::SubRange::self), Item::EFuncActive, OnPress_PF5, Item::EFuncDraw
)


static void OnPress_PC13(bool active)
{
    if (active)
    {
        Message::RegCPU(2, 15, set.reg.PC13).Transmit();
    }
}

DEF_CHOICE_2( cPC13,
    "PC13", "PC13",
    "", "",
    "0", "0", "", "",
    "1", "1", "", "",
    set.reg.PC13, (*PageDebug::SubRange::self), Item::EFuncActive, OnPress_PC13, Item::EFuncDraw
)


static void OnPress_PC14(bool active)
{
    if (active)
    {
        Message::RegCPU(2, 14, set.reg.PC14).Transmit();
    }
}

DEF_CHOICE_2(cPC14,
    "PC14", "PC14",
    "", "",
    "0", "0", "", "",
    "1", "1", "", "",
    set.reg.PC14, (*PageDebug::SubRange::self), Item::EFuncActive, OnPress_PC14, Item::EFuncDraw
)


DEF_PAGE_6(pSubRanges, //-V641
    "Диапазоны", "Ranges",
    "", "",
    &cPE15,
    &cPB10,
    &cPF0,
    &cPF5,
    &cPC13,
    &cPC14,
    Page::SubRanges, PageDebug::self, Item::EFuncActive, Page::FuncEnter, Item::EFuncOnKey, Page::EFuncAfterDraw
)

Page *PageDebug::SubRange::self = (Page *)&pSubRanges;
