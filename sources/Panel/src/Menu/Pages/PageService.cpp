#include "Display/Colors.h"
#include "Display/Display.h"
#include "Display/Painter.h"
#include "Display/Text.h"
#include "Generator/Generator_p.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Settings/Settings.h"
#include "Settings/CalibrationSettings.h"
#include "PageService.h"
#include "Menu/Menu.h"
#include "Utils/StringUtils.h"
#include "Utils/NumberBuffer.h"
#include "Utils/Math.h"
#include "Display/Symbols.h"


extern const PageBase pUSB;


extern const PageBase pService;
Page *PageService::self = reinterpret_cast<Page *>(const_cast<PageBase *>(&pService));


static void OnPress_Reset()
{
    PGenerator::Reset();
    HAL_TIM::Delay(100);
    setCal.Load();
    set.Load(true);
}

DEF_BUTTON( bReset,                                                                                                                                                 //--- ОТЛАДКА - СБРОС ---
    "СБРОС",
    "Сброс настроек на значения по умолчанию",
    pService, Item::FuncActive, OnPress_Reset, FuncDraw
)


extern const PageBase pDebug;


DEF_PAGE_2( pService,                                                                                                                                                        //--- СЕРВИС --- //-V641
    "СЕРВИС",    //-V641
    "Сервисные функции",
    &bReset,
    &pDebug,
    Page::Service, Menu::mainPage, Item::FuncActive, Page::FuncEnter, FuncOnKey, Page::FuncDraw
)
