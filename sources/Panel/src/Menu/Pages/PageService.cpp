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
Page *PageService::self = reinterpret_cast<Page *>(const_cast<PageBase *>(&pService));


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
//    pService, Item::FuncActive, PageService::OnPress_Reset, FuncDraw
//)


DEF_CHOICE_2(cLanguage,
    "Language", "����",
    "����� �����", "Language choice",
    "Russian",    "Russian",    "", "",
    "A���������", "����������", "", "",
    set.lang, pService, Item::FuncActive, FuncChangedChoice, FuncDraw
)


void PageService::OnChange_Volume(bool)
{
    Beeper::Init();
    setCal.Save();
}


DEF_CHOICE_4(cVolume,
    "���������", "Sound",
    "��������� ������ ���������", "Setting the volume level",
    "����",  "Off",    "", "",
    "���",   "Min",    "", "",
    "C����", "Middle", "", "",
    "����",  "Max",    "", "",
    setCal.soundVolume, pService, Item::FuncActive, PageService::OnChange_Volume, FuncDraw
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
    Page::Service, reinterpret_cast<PageBase *>(PageMain::self), Item::FuncActive, Page::FuncEnter, FuncOnKey, Page::FuncDraw
)


void PageDebug::Enable()
{
    pService.items[3] = PageDebug::self;
}
