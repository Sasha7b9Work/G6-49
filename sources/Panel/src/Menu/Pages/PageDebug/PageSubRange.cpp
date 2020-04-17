#include "defines.h"
#include "common/Messages.h"
#include "Menu/Pages/PageDebug/PageDebug.h"
#include "Settings/Settings.h"


static void OnPress_PE15(bool)
{
    Message::RegCPU(4, 15, set.bitPE15).Transmit();
}

DEF_CHOICE_2( cPE15,
    "PE15", "PE15",
    "", "",
    "0", "0", "", "",
    "1", "1", "", "",
    set.bitPE15, (*PageDebug::SubRange::self), Item::FuncActive, OnPress_PE15, FuncDraw
)


static void OnPress_PB10(bool)
{
    Message::RegCPU(1, 10, set.bitPB10).Transmit();
}

DEF_CHOICE_2( cPB10,
   "PB10", "PB10",
    "", "",
    "0", "0", "", "",
    "1", "1", "", "",
    set.bitPB10, (*PageDebug::SubRange::self), Item::FuncActive, OnPress_PB10, FuncDraw
)


static void OnPress_PF0(bool)
{
    Message::RegCPU(5, 0, set.bitPF0).Transmit();
}

DEF_CHOICE_2( cPF0,
    "PF0", "PF0",
    "", "",
    "0", "0", "", "",
    "1", "1", "", "",
    set.bitPF0, (*PageDebug::SubRange::self), Item::FuncActive, OnPress_PF0, FuncDraw
)


static void OnPress_PF5(bool)
{
    Message::RegCPU(5, 5, set.bitPF5).Transmit();
}

DEF_CHOICE_2( cPF5,
    "PF5", "PF5",
    "", "",
    "0", "0", "", "",
    "1", "1", "", "",
    set.bitPF5, (*PageDebug::SubRange::self), Item::FuncActive, OnPress_PF5, FuncDraw
)


static void OnPress_PC13(bool)
{
    Message::RegCPU(2, 15, set.bitPC13).Transmit();
}

DEF_CHOICE_2( cPC13,
    "PC13", "PC13",
    "", "",
    "0", "0", "", "",
    "1", "1", "", "",
    set.bitPC13, (*PageDebug::SubRange::self), Item::FuncActive, OnPress_PC13, FuncDraw
)


static void OnPress_PC14(bool)
{
    Message::RegCPU(2, 14, set.bitPC14).Transmit();
}

DEF_CHOICE_2(cPC14,
    "PC14", "PC14",
    "", "",
    "0", "0", "", "",
    "1", "1", "", "",
    set.bitPC14, (*PageDebug::SubRange::self), Item::FuncActive, OnPress_PC14, FuncDraw
)


DEF_PAGE_6(pSubRanges,
    "Диапазоны", "Ranges",
    "", "",
    &cPE15,
    &cPB10,
    &cPF0,
    &cPF5,
    &cPC13,
    &cPC14,
    Page::SubRanges, PageDebug::self, Item::FuncActive, Page::FuncEnter, FuncOnKey, Page::FuncDraw
)

Page *PageDebug::SubRange::self = reinterpret_cast<Page *>(const_cast<PageBase *>(&pSubRanges));
