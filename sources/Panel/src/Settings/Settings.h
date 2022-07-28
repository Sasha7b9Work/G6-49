// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/CommonTypes.h"
#include "Menu/MenuItems.h"
#include "FreqMeter/FreqMeter_p.h"
#include "Generator/Generator_p.h"
#include "Settings/SettingsTypes.h"


#define LANGUAGE                    (set.lang)
#define LANG_IS_RU                  (LANGUAGE == 0)
#define COLOR(x)                    (set.disp_Colors[x])
#define CURRENT_PAGE                (set._menu_currentPage)
#define MENU_POS_ACT_ITEM(x)        (set.menu_posActItem[x])
#define MENU_CURRENT_SUBPAGE(x)     (set.menu_currentSubPage[x])
#define CURRENT_CHANNEL             (set.cur_chan)


struct SettingsFreqMeter
{
    PFreqMeter::BillingTime billingTime;   // ����� �����
    PFreqMeter::AvePeriod   avePeriod;     // ����� ����������� �������� � ������ ��������� �������
    FreqTimeStamps          timeStamps;    // ����� �������
    FreqMeasure::E          measure;       // ����� ��������� �����������
    int16                   level;         // ������� �������������
    int16                   hysteresis;    // �������� �����������
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
    uint16    size;                             // ������ ��������� Settings
    col_val   disp_Colors[32];                  // �����
    Page*     _menu_currentPage;                // ����� �������� �������� ����. 0, ���� ������� ������� ��������
    int8      menu_posActItem[Page::Count];     // ������� ��������� ������ ���� ��� ������ ��������
    int8      menu_currentSubPage[Page::Count]; // ����� ������� ����������� ��� ������ ��������
    bool      enabled[2];
    Chan      cur_chan;
    uint8     lang;
    bool      bigSymbols;                       // ���� 1, �� ������� ��������� ���� ������������

    SettingsFreqMeter freq;
    SettingsRegisters reg;
    SettingsDebug     dbg;

    void LoadDefault();
    void LoadToDevice();

    Settings& operator=(const Settings &rhs);

private:
    // ���� ����� ��������� ������� ��������� �������� Store
    static Settings stored;
};


extern Settings set;
