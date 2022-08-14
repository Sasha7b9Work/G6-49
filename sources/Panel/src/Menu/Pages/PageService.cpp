// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "Display/Colors.h"
#include "Display/Display.h"
#include "Display/Painter.h"
#include "Display/Symbols.h"
#include "Display/Text.h"
#include "Generator/Generator_p.h"
#include "Hardware/Beeper.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Pages.h"
#include "Settings/Settings.h"
#include "Settings/CalibrationSettings.h"
#include "Utils/StringUtils.h"
#include "Utils/NumberBuffer.h"
#include "Utils/Math.h"


extern const PageBase pUSB;


extern const PageBase pService;
Page *PageService::self = (Page *)&pService;


void PageService::OnPress_Reset()
{
    HAL_TIM::Delay(100);
    setCal.Load();
    set.LoadDefault();
    set.LoadToDevice();
    Menu::Init();
}

//DEF_BUTTON( bReset,                                                                                                                                                 //--- ������� - ����� ---
//    "�����", "RESET",
//    "����� �������� �� �������� �� ���������", "Reset to default",
//    pService, Item::EFuncActive, PageService::OnPress_Reset, FuncDraw
//)


DEF_CHOICE_2(cLanguage,
    "Language", "����",
    "����� �����", "Language choice",
    "Russian",    "Russian",    "", "",
    "A���������", "����������", "", "",
    set.lang, pService, Item::EFuncActive, FuncChangedChoice, Item::EFuncDraw
)


void PageService::OnChange_Volume(bool active)
{
    if (active)
    {
        Beeper::Init();
        setCal.Save();
    }
}


DEF_CHOICE_4(cVolume,
    "���������", "Sound",
    "��������� ������ ���������", "Setting the volume level",
    "����",  "Off",    "", "",
    "���",   "Min",    "", "",
    "C����", "Middle", "", "",
    "����",  "Max",    "", "",
    setCal.soundVolume, pService, Item::EFuncActive, PageService::OnChange_Volume, Item::EFuncDraw
)


extern const PageBase pDebug;


DEF_PAGE_4_VAR(pService,                                                                                                                                            //--- ������ --- //-V641
    "������", "SERVICE",    //-V641
    "��������� �������", "Service functions",
    &cLanguage,
    &cVolume,
    &Item::emptyLight,
    &Item::emptyLight,
    //PageDebug::self,
    Page::Service, (PageBase *)PageMain::self, Item::EFuncActive, Page::EFuncEnter, Item::EFuncOnKey, Page::EFuncAfterDraw
)


void PageDebug::Enable()
{
    pService.items[3] = PageDebug::self;
}
