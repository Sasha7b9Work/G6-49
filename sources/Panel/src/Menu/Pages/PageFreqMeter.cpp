// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "Menu/Menu.h"
#include "Menu/Pages/Pages.h"
#include "Settings/Settings.h"
#include "Display/Symbols.h"


extern const PageBase pFrequencyCounter;
Page *PageFrequencyCounter::self = (Page *)&pFrequencyCounter;


// ��������� ��� �������� � ������������ � ������� ���������
static void Tune_Page();


void PageFrequencyCounter::OnPress_Measure(bool active)
{
    if (active)
    {
        Tune_Page();
        PageFrequencyCounter::WriteRegisterRG9();
    }
}

DEF_CHOICE_3( cMeasure,                                                                                                                                      //--- ���������� - ��������� ---
    "���������", "Measurement",
    "��������� ������ ������", "Setting operation mode",
    "���������", "Off",       "��������� ���������", "Measurements off",
    "�������",   "Frequency", "��������� �������",   "Frequency measurement",
    "������",    "Period",    "��������� �������",   "Period measurement",
    set.freq.measure, pFrequencyCounter, Item::EFuncActive, PageFrequencyCounter::OnPress_Measure, Item::EFuncDraw
)


static void OnPress_Interval(bool active)
{
    if (active)
    {
        PageFrequencyCounter::WriteRegisterRG9();
    }
}


DEF_CHOICE_2( cInterval,                                                                                                                              //--- ���������� - �������� ������� ---
    "�������� �������", "Launch interval",
    "����� ��������� ������� ��������� �������", "Frequency start interval selection",
    "1 c",  "1 s",  "������ �������� ��������� ��������� ������������ � ���������� 1 �������",  "The process of measuring the frequency meter is started at intervals of 1 second",
    "10 �", "10 s", "������ �������� ��������� ��������� ������������ � ���������� 10 �������", "The measurement process starts with a 10 second interval",
    set.freq.interval, pFrequencyCounter, Item::EFuncActive, OnPress_Interval, Item::EFuncDraw
)

volatile const ChoiceBase *pcInterval = &cInterval;


void PageFrequencyCounter::OnPress_BillingTime(bool active)
{
    if (active)
    {
        PageFrequencyCounter::WriteRegisterRG9();
    }
}


DEF_CHOICE_5( cBillingTime,                                                                                                                                //--- ���������� - ����� ����� ---
    "�� �����", "Time count",
    "����� ������� ���������� ���������", "Timing measurement",
    "1 ��",     "1 ms",     "������������ ��������� 1 ������������",    "1 millisecond measurement duration",
    "10 ��",    "10 ms",    "������������ ��������� 10 �����������",    "Measurement duration 10 milliseconds",
    "100 ��",   "100 ms",   "������������ ��������� 100 �����������",   "Measurement duration 100 milliseconds",
    "1000 ��",  "1000 ms",  "������������ ��������� 1000 �����������",  "Measurement duration 1000 milliseconds",
    "10000 ��", "10000 ms", "������������ ��������� 10000 �����������", "Measurement duration 10000 milliseconds",
    set.freq.billingTime, pFrequencyCounter, Item::EFuncActive, PageFrequencyCounter::OnPress_BillingTime, Item::EFuncDraw
)


DEF_CHOICE_2(cResist,                                                                                                                              //--- ���������� - ������������� ����� ---
    "R ��", "R in",
    "���������� �������������� ����� �����������", "Frequency counter input resistance control",
    "1 ���", "1 MOhm", "������������� ����� 1 ���", "Input impedance 1 MOhm",
    "50 ��", "50 Ohm", "������������� ����� 50 ��", "50 ohm input impedance",
    set.freq.resist, pFrequencyCounter, Item::EFuncActive, PageFrequencyCounter::OnPress_Resist, Item::EFuncDraw
)

void PageFrequencyCounter::OnPress_Resist(bool active)
{
    if (active)
    {
        PGenerator::LoadRegister(Register::FreqMeter_Resist, (uint)set.freq.resist);
    }
}


DEF_CHOICE_2(cCouple,                                                                                                                                             //--- ���������� - ���� ---
    "����", "Couple",
    "����������/��������� ���������� ������������", "Skips / Disables DC",
    COUPLE_AC, COUPLE_AC, "���������� ������������ ��������� �� ���� �����������",    "The constant component is fed to the input of the frequency counter",
    COUPLE_DC, COUPLE_DC,    "���������� ������������ �� ��������� �� ���� �����������", "The constant component does not go to the input of the frequency meter",
    set.freq.couple, pFrequencyCounter, Item::EFuncActive, PageFrequencyCounter::OnPress_Couple, Item::EFuncDraw
)

void PageFrequencyCounter::OnPress_Couple(bool active)
{
    if (active)
    {
        PGenerator::LoadRegister(Register::FreqMeter_Couple, (uint)set.freq.couple);
    }
}


DEF_CHOICE_2(cFiltr,                                                                                                                                               //--- ���������� - ��� ---
    "���", "LPF",
    "��������/��������� ������ ������ ������ �� ����� �����������", "Enables / disables the low-pass filter at the input of the frequency meter",
    DISABLED_RU, DISABLED_EN, "��� �� ����� ����������� ��������",  "LPF at the input of the frequency meter is disabled",
    ENABLED_RU,  ENABLED_EN,  "��� �� ����� ����������� ���������", "Low-pass filter at the input of the frequency meter",
    set.freq.filtr, pFrequencyCounter, Item::EFuncActive, PageFrequencyCounter::OnPress_Filtr, Item::EFuncDraw
)

void PageFrequencyCounter::OnPress_Filtr(bool active)
{
    if (active)
    {
        PGenerator::LoadRegister(Register::FreqMeter_Filtr, (uint)set.freq.filtr);
    }
}

DEF_CHOICE_5(cAvePeriod,                                                                                                                                //--- ���������� - ����� �������� ---
    "N ��������", "N eriods",
    "����� ����� ����������� �������� � ������ ��������� �������", "Choosing the number of averaged periods in period measurement mode",
    "1",     "1",     "��������� ����������� �� ������ �������",          "Measurements on one period",
    "10",    "10",    "��������� ����������� �� ������ ��������",         "Measure over ten periods",
    "100",   "100",   "��������� ����������� �� ��� ��������",            "Measurements over a hundred periods",
    "1000",  "1000",  "��������� ����������� �� ������ ��������",         "Measure over a thousand periods",
    "10000", "10000", "��������� ����������� �� ������ ������� ��������", "Measure over ten thousand periods",
    set.freq.avePeriod, pFrequencyCounter, Item::EFuncActive, PageFrequencyCounter::OnPress_AvePeriod, Item::EFuncDraw
)

void PageFrequencyCounter::OnPress_AvePeriod(bool active)
{
    if (active)
    {
        PageFrequencyCounter::WriteRegisterRG9();
    }
}


DEF_CHOICE_5(cTimeStamps,                                                                                                                                //--- ���������� - ����� ������� ---
    "����� ��", "Time stamps",
    "", "",
    "10-3 �", "10-3 s",  "", "",
    "10-4 �", "10-4 s",  "", "",
    "10-5 �", "10-5 s",  "", "",
    "10-6 �", "10-6 s",  "", "",
    "10-7 �", "10-7 s",  "", "",
    set.freq.timeStamps, pFrequencyCounter, Item::EFuncActive, PageFrequencyCounter::OnPress_TimeStamps, Item::EFuncDraw
)

void PageFrequencyCounter::OnPress_TimeStamps(bool active)
{
    if (active)
    {
        PageFrequencyCounter::WriteRegisterRG9();
    }
}


DEF_CHOICE_2(cTest,                                                                                                                                               //--- ���������� - ���� ---
    "����", "Test",
    "���������/���������� ��������� ������", "Enable / disable test mode",
    DISABLED_RU, DISABLED_EN, "", "",
    ENABLED_RU,  ENABLED_EN,  "", "",
    set.freq.test, pFrequencyCounter, Item::EFuncActive, PageFrequencyCounter::OnPress_Test, Item::EFuncDraw
)

void PageFrequencyCounter::OnPress_Test(bool active)
{
    if (active)
    {
        PageFrequencyCounter::WriteRegisterRG9();
    }
}



void PageFrequencyCounter::OnChange_FreqLevel()
{
    PFreqMeter::LoadLevel();
}

DEF_GOVERNOR( gLevel,                                                                                                                                          //--- ���������� - ������� ---
    "�������", "Level",
    "���������� ������ �������������", "Sync level adjustment",
    set.freq.level, -100, 100, pFrequencyCounter, Item::EFuncActive, PageFrequencyCounter::OnChange_FreqLevel
)



static void OnChange_Hysteresis()
{
    PFreqMeter::LoadHysteresis();
}

DEF_GOVERNOR( gHysteresis,                                                                                                                                  //--- ���������� - ���������� ---
    "����������", "Hysteresis",
    "����� ���������� ��� ���������� ������� ����� �� �������� ���������", "Sets hysteresis to reduce the effect of interference on measurement accuracy",
    set.freq.hysteresis, 0, 100, pFrequencyCounter, Item::EFuncActive, OnChange_Hysteresis
)

volatile const GovernorBase *pgHysteresis = &gHysteresis;


DEF_PAGE_8( pFrequencyCounter,                                                                                                                                           //--- ���������� --- //-V641 //-V1027
    "����������", "FREQMETER", //-V641
    "���������� ��������� �����������", "Freqmeter control",
    &cMeasure,       // ���������� - ���������
    &gLevel,         // ���������� - �������
    &Item::emptyLight,
    &Item::emptyLight,
    &cResist,        // ���������� - �������������
    &cCouple,        // ���������� - ����
    &cFiltr,         // ���������� - ���
    &cTest,          // ���������� - ����
    Page::FrequencyCounter, PageMain::self, Item::EFuncActive, Page::EFuncEnter, Item::EFuncOnKey, Page::EFuncAfterDraw
)


static void Tune_Page()
{
    PageBase *page = (PageBase *)&pFrequencyCounter;

    if (set.freq.measure == FreqMeasure::Freq)
    {
        page->items[2] = (Item *)&cBillingTime;
        page->items[3] = &Item::emptyLight;
    }
    else if (set.freq.measure == FreqMeasure::Period)
    {
        page->items[2] = (Item *)&cTimeStamps;
        page->items[3] = (Item *)&cAvePeriod;
    }
    else
    {
        page->items[2] = &Item::emptyLight;
        page->items[3] = &Item::emptyLight; //-V656
    } //-V656
}

#ifdef WIN32
#pragma warning(push)
#pragma warning(disable:4310)       // cast truncates constant value
#endif


void PageFrequencyCounter::WriteRegisterRG9()
{
    uint data = 0;

     //--- ����� ������ ------------------
    if(set.freq.measure == FreqMeasure::Period)
    {
        data |= 1;
    }

    //--- ����� ����������� �������� ----

    static const uint maskAvePeriod[5] =
    {
        BINARY_U8(00000000),    // -V2501   // 1
        BINARY_U8(00000010),    // -V2501   // 10
        BINARY_U8(00000110),    // -V2501   // 100
        BINARY_U8(00001010),    // -V2501   // 1000
        BINARY_U8(00001110)     // -V2501   // 10000
    };
    data |= maskAvePeriod[set.freq.avePeriod];

    //---------- ����� ��������� ----------------

    static const uint maskInterval[2] =
    {
        BINARY_U8(00000000),    // -V2501
        BINARY_U8(00010000)     // -V2501
    };
    data |= maskInterval[set.freq.interval];

    //--------- ����� ����� ---------------------

    static const uint maskTime[5] =
    {
        BINARY_U8(00000000),    // -V2501               // 1 ��
        BINARY_U8(00100000),    // -V2501               // 10 ��
        BINARY_U8(01000000),    // -V2501   // -V536    // 100 ��
        BINARY_U8(01100000),    // -V2501               // 1 �
        BINARY_U8(10000000)     // -V2501               // 10 �
    };
    data |= maskTime[set.freq.billingTime];

    //--------- ����� ������� -------------------

    static const uint maskTimeStamp[5] =
    {
        BINARY_U8(00001010),    // -V2501    // 1 ���
        BINARY_U8(00000110),    // -V2501    // 10 ���
        BINARY_U8(00000010),    // -V2501    // 100 ���
        BINARY_U8(00000001),    // -V2501    // 1 ���
        BINARY_U8(00000000)     // -V2501    // 10 ���
    };

    data |= (maskTimeStamp[set.freq.timeStamps] << 8);

    if(set.freq.test == FreqTest::On)
    {
        _SET_BIT(data, 12);
    }

    PGenerator::LoadRegister(Register::FPGA_RG9_FreqMeter, data);
    PFreqMeter::SetInactive();
}

#ifdef WIN32
#pragma warning(pop)
#endif
