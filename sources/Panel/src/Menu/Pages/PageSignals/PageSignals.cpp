#include "stdafx.h"
#ifndef WIN32
#include "Display/InputWindow.h"
#include "Settings/Settings.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/AddPageInput.h"
#include "Menu/Pages/Include/PageSignals.h"
#include "Generator/Signals.h"
#include "Generator/GeneratorPanel.h"
#include "Menu/Menu.h"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pSignals;
Page *PageSignals::pointer = (Page *)&pSignals;
extern ChoiceParameterBase cParameters;
/// Номер текущей формы сигнал
static int numForm = 0;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_CHOICE_2( cChannel,                                                                                           //--- НАСТРОЙКИ СИГНАЛОВ - Канал ---
    "КАНАЛ", "CHANNEL",
    "Выбор канала для настройки.",
    "Selecting a channel to set up.",
    "A", "A", "Управление параметрами сигнала на выходе A.",
              "Controlling the signal parameters at output A.",
    "B", "B", "Управление параметрами сигнала на выходе B.",
              "Controlling the signal parameters at output B.",
    FLAG_1, BIT_CHANNEL, pSignals, FuncActive, PageSignals::OnPress_Channel, FuncDraw
)


//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_7( cFormA,                                                                                             //--- НАСТРОЙКИ СИГНАЛОВ - Форма ---
    "ФОРМА", "FORM",
    "Выбор формы сигнала.",
    "Select waveform.",
    FORM_RU(Form::Sine),         FORM_EN(Form::Sine),         "Синус",            "Sinus",
    FORM_RU(Form::RampPlus),     FORM_EN(Form::RampPlus),     "Нарастающая пила", "Growing saw",
    FORM_RU(Form::RampMinus),    FORM_EN(Form::RampMinus),    "Убывающая пила",   "Wrecking saw",
    FORM_RU(Form::Triangle),     FORM_RU(Form::Triangle),     "Треугольник",      "Triangle",
    FORM_RU(Form::Meander),      FORM_EN(Form::Meander),      "Меандр",           "Meander",
    FORM_RU(Form::Impulse),      FORM_EN(Form::Impulse),      "Треугольник",      "Triangle",
    FORM_RU(Form::PacketImpuls), FORM_EN(Form::PacketImpuls), "Пакеты",           "Packets",
    numForm, pSignals, FuncActive, PageSignals::OnPress_Form, FuncDraw
)

DEF_CHOICE_6( cFormB,                                                                                             //--- НАСТРОЙКИ СИГНАЛОВ - Форма ---
    "ФОРМА", "FORM",
    "Выбор формы сигнала.",
    "Select waveform.",
    FORM_RU(Form::Sine),      FORM_EN(Form::Sine),      "Синус",            "Sinus",
    FORM_RU(Form::RampPlus),  FORM_EN(Form::RampPlus),  "Нарастающая пила", "Growing saw",
    FORM_RU(Form::RampMinus), FORM_EN(Form::RampMinus), "Убывающая пила",   "Wrecking saw",
    FORM_RU(Form::Triangle),  FORM_RU(Form::Triangle),  "Треугольник",      "Triangle",
    FORM_RU(Form::Meander),   FORM_EN(Form::Meander),   "Меандр",           "Meander",
    FORM_RU(Form::Impulse),   FORM_EN(Form::Impulse),   "Треугольник",      "Triangle",
    numForm, pSignals, FuncActive, PageSignals::OnPress_Form, FuncDraw
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_ChnageParameter()
{
    FORM_CURRENT->ChangeParameter();
}

DEF_BUTTON( bChangeParameter,                                                                 //--- НАСТРОЙКИ СИГНАЛОВ - Ввести значение параметра ---
    "Изменить", "Change",
    "Открывает окно ввода параметра.",
    "Opens the parameter input window.",
    pSignals, FuncActive, OnPress_ChnageParameter, FuncDraw
)

DEF_CHOICE_PARAMETER(cParameters,                                                                              //--- НАСТРОЙКИ СИГНАЛОВ - Параметр ---
    "ПАРАМЕТР", "PARAMETER",
    "Выбор параметра для настройки.",
    "Choosing a setting for customization.",
    pSignals, FuncActive, OnPress_ChnageParameter, WAVE(Chan::A).GetForm(0)
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(cTypeTune,                                                                                         //--- НАСТРОЙКИ СИГНАЛОВ - Засылка ---
    "ЗАСЫЛКА", "SENDING",
    "Определяет периодичность засылки настроек сигналов в устройство управления.",
    "Specifies the frequency of sending signal settings to the control device.",
    "ОДНОКРАТНО", "SINGLE",         "Засылка настроек сигнала происходит однократно, после нажатия кнопки подтверждения.",
                                    "The signal settings are sent once, after you press the confirm button.",
    "НЕПРЕРЫВНО", "CONTINUOUSLY",   "Засылка настроек сигнала происходит синхронно с изменением информации на экране.",
                                    "Sending of signal settings occurs synchronously with changing information on the screen.",
    FLAG_1, BIT_TUNE_FULL, pSignals, FuncActive, FuncChangedChoice, FuncDraw
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
static bool OnKey(Control key)
{
    if(key.Is(Control::Esc) && key.action.IsRelease())
    {
        return FORM_CURRENT->CloseOpenedParameter();
    }

    return false;
}

DEF_PAGE_6( pSignals,
    "НАСТРОЙКИ СИГНАЛОВ", "SIGNAL SETTINGS",
    "", "",
    &cChannel,                          ///< НАСТРОЙКИ СИГНАЛОВ - Канал
    &cFormA,                            ///< НАСТРОЙКИ СИГНАЛОВ - Форма
    &cParameters,                       ///< НАСТРОЙКИ СИГНАЛОВ - Параметр
    &bChangeParameter,                  ///< НАСТРОЙКИ СИГНАЛОВ - Ввести значение параметра
    PageSignals::PageLoad::pointer,     ///< НАСТРОЙКИ СИГНАЛОВ - ЗАГРУЗКА
    &cTypeTune,                         ///< НАСТРОЙКИ СИГНАЛОВ - Засылки
    Page::Settings, Menu::mainPage, FuncActive, FuncPress, OnKey
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
void PageSignals::Init()
{
    OnPress_Channel(true);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void PageSignals::OnPress_Form(bool)
{
    ChoiceBase *choice = (ChoiceBase *)pSignals.items[1];

    WAVE_CURRENT.SetForm(choice->CurrentIndex());

    cParameters.form = FORM_CURRENT;

    Generator::TuneChannel(CURRENT_CHANNEL);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void PageSignals::OnPress_Channel(bool)
{
    cParameters.form = FORM_CURRENT;
    numForm = FORM_CURRENT->value;
    pSignals.items[1] = Chan(CURRENT_CHANNEL).IsA() ? (Item *)&cFormA : (Item *)&cFormB;
}
