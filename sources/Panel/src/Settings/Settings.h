#pragma once
#include "common/CommonTypes.h"
#include "Menu/MenuItems.h"
#include "FreqMeter/FreqMeter_p.h"
#include "Generator/Generator_p.h"
#include "Settings/SettingsTypes.h"


#define LANGUAGE                    (set.lang)
#define LANG_IS_RU                  (LANGUAGE == 0)
#define COLOR(x)                    (set.disp_Colors[x])
#define CURRENT_PAGE                (set.menu_currentPage)
#define MENU_POS_ACT_ITEM(x)        (set.menu_posActItem[x])
#define MENU_CURRENT_SUBPAGE(x)     (set.menu_currentSubPage[x])
#define CURRENT_CHANNEL             (set.current)
#define ENABLED_CH(ch)              (set.enabled[ch])


struct SettingsFreqMeter
{
    PFreqMeter::BillingTime billingTime;   // Время счёта
    PFreqMeter::AvePeriod   avePeriod;     // Число усредняемых периодов в режиме измерения периода
    FreqTimeStamps          timeStamps;    // Метки времени
    FreqMeasure::E          measure;       // Режим измерения частотомера
    int16                   level;         // Уровень синхронизации
    int16                   hysteresis;    // Величина гистерезиса
    FreqInterval::E         interval;
    FreqResist::E           resist;
    FreqCouple::E           couple;
    FreqFiltr::E            filtr;
    FreqTest::E             test;
};


struct SettingsDebug
{
    bool   showConsole;
    bool   modeEnabled;
    bool   showStatistics;
    bool   showSends;
};

struct SettingsRegisters
{
    uint8  PE15;
    uint8  PB10;
    uint8  PF0;
    uint8  PF5;
    uint8  PC13;
    uint8  PC14;
};


struct Settings // -V690
{
    uint16    size;                             // Размер структуры Settings
    col_val   disp_Colors[32];                  // Цвета
    Page*     menu_currentPage;                 // Адрес открытой страницы меню. 0, если открыта главная страница
    int8      menu_posActItem[Page::Count];     // Позиция активного пункта меню для каждой страницы
    int8      menu_currentSubPage[Page::Count]; // Номер текущей подстраницы для каждой страницы
    bool      enabled[2];
    Chan::E   current;
    uint8     lang;
    bool      bigSymbols;                       // Если 1, то символы выводятся чуть увеличенными

    SettingsFreqMeter freq;
    SettingsRegisters reg;
    SettingsDebug     dbg;

    void SaveToMemory();
    void LoadFromMemory();
    void LoadDefault();
    void LoadToDevice();

    Settings& operator=(const Settings &rhs);

private:
    // Сюда можно сохранить текущие настройки функцией Store
    static Settings stored;
};


extern Settings set;
