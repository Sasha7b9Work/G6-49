#include "PageFrequencyCounter2.h"
#include "Settings/Settings.h"
#include "Generator/Generator.h"
#include "Settings/Settings.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pFrequencyCounter2;
Page *PageFrequencyCounter2::pointer = (Page *)&pFrequencyCounter2;

static void OnPress_Resist(bool);
static void OnPress_Couple(bool);
static void OnPress_Filtr(bool);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_CHOICE_2(cResist,
    "СОПРОТИВЛЕНИЕ ВХОДА", "INPUT RESISTANCE",
    "Управление сопротивлением входа частотомера",
    "Control of the resistance of the frequency meter input",
    "1 МОм", "1 MOhm",  "Сопротивление входа 1 МОм",
                        "Input Impedance 1 MOhm",
    "50 Ом", "50 Ohm",  "Сопротивление входа 50 Ом",
                        "Input Impedance 50 Ohm",
    FLAG_1, BIT_FREQ_RESIST, pFrequencyCounter2, FuncActive, OnPress_Resist, FuncDraw
)

static void OnPress_Resist(bool)
{
    Generator::LoadRegister(Register::FreqMeter_Resist, (uint)FREQ_RESIST);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(cCouple,
    "ВХОД", "COUPLE",
    "Пропускает/запрещает постоянную составляющую",
    "Skips / prohibits the DC component",
    "Перем", "AC",  "Постоянная составляющая поступает на вход частотомера",
                    "The DC component is fed to the frequency meter input",
    "Пост", "DC",   "Постоянная составляющая не поступает на вход частотомера",
                    "The DC component is not input to the frequency meter input",
    FLAG_1, BIT_FREQ_COUPLE, pFrequencyCounter2, FuncActive, OnPress_Couple, FuncDraw
)

static void OnPress_Couple(bool)
{
    Generator::LoadRegister(Register::FreqMeter_Couple, (uint)FREQ_COUPLE);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(cFiltr,
    "ФНЧ", "LPF",
    "Включает/отключает фильтр нижних частот на входе частотомера",
    "Enables / disables the low-pass filter at the input of the frequency meter",
    DISABLED_RU, DISABLED_EN,   "ФНЧ на входе частотомера отключен",
                                "LPF at the frequency meter input is disabled",
    ENABLED_RU, ENABLED_EN,     "ФНЧ на входе частотомера водключен",
                                "LPF at the input of the frequency meter is turned on",
    FLAG_1, BIT_FREQ_FILTR, pFrequencyCounter2, FuncActive, OnPress_Filtr, FuncDraw
)

static void OnPress_Filtr(bool)
{
    Generator::LoadRegister(Register::FreqMeter_Filtr, (uint)FREQ_FILTR);
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_PAGE_3(pFrequencyCounter2,
    "ЧАСТОТОМЕР 2", "FREQUENCY METER",
    "Управление функциями частотомера",
    "Control of frequency meter functions",
    cResist,
    cCouple,
    cFiltr,
    Page::Name::FrequencyCounter2, 0, FuncActive, FuncPress, FuncOnKey
)
