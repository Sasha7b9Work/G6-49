#pragma once
#include "Display/InputWindow.h"
#include "Settings/Settings.h"
#include "Menu/MenuItems.h"
#include "AddPageInput.h"
#include "PageSignals.h"
#include "Generator/Signals.h"
#include "Generator/Generator.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pSignals;
Page *PageSignals::pointer = (Page *)&pSignals;
extern ChoiceParameterBase cParameters;
/// Номер текущей формы сигнал
static int numForm = 0;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnPress_Channel(bool)
{
    cParameters.form = FORM_CURRENT;
    numForm = FORM_CURRENT->value;
}

DEF_CHOICE_2( cChannel,                                                                                           //--- НАСТРОЙКИ СИГНАЛОВ - КАНАЛ ---
    "КАНАЛ", "CHANNEL",
    "Выбор канала для настройки.",
    "Selecting a channel to set up.",
    "A", "A", "Управление параметрами сигнала на выходе A.",
              "Controlling the signal parameters at output A.",
    "B", "B", "Управление параметрами сигнала на выходе B.",
              "Controlling the signal parameters at output B.",
    FLAG_1, BIT_CHANNEL, pSignals, FuncActive, OnPress_Channel, FuncDraw
)


//----------------------------------------------------------------------------------------------------------------------------------------------------
void OnPress_Form(bool)
{
    WAVE_CURRENT.SetNextForm();

    numForm = WAVE_CURRENT.GetCurrentForm()->value;

    cParameters.form = FORM_CURRENT;

    Generator::TuneChannel(CURRENT_CHANNEL);
}

DEF_CHOICE_6( cForm,                                                                                              //--- НАСТРОЙКИ СИГНАЛОВ - ФОРМА ---
    "ФОРМА", "FORM",
    "Выбор формы сигнала.",
    "Select waveform.",
    FORM_RU(Form::Sine),            FORM_EN(Form::Sine),          "Синус",               "Sinus",
    FORM_RU(Form::RampPlus),        FORM_EN(Form::RampPlus),      "Нарастающая пила",    "Growing saw",
    FORM_RU(Form::RampMinus),       FORM_EN(Form::RampMinus),     "Убывающая пила",      "Wrecking saw",
    FORM_RU(Form::Meander),         FORM_EN(Form::Meander),       "Меандр",              "Meander",
    FORM_RU(Form::Impulse),         FORM_EN(Form::Impulse),       "Треугольник",         "Triangle",
    FORM_RU(Form::PacketImpuls),    FORM_EN(Form::PacketImpuls),  "Пакеты",              "Packets",
    numForm, pSignals, FuncActive, OnPress_Form, FuncDraw
)


//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_SetParameter()
{
    if(PARAM_CURRENT->IsInputValue())
    {
        InputWindow::Init();

        ADDITION_PAGE = (Page *)&pInput;
    }
    else if(PARAM_CURRENT->IsExitParameter())
    {
        FORM_CURRENT->CloseOpenedParameter();
    }
    else
    {
        FORM_CURRENT->OpenCurrentParameter();
    }
}

DEF_BUTTON( bSetParameter,                                                                    //--- НАСТРОЙКИ СИГНАЛОВ - ВВЕСТИ ЗНАЧЕНИЕ ПАРАМЕТРА ---
    "Изменить", "Change",
    "Открывает окно ввода параметра.",
    "Opens the parameter input window.",
    pSignals, FuncActive, OnPress_SetParameter, FuncDraw
)

DEF_CHOICE_PARAMETER(cParameters,                                                                              //--- НАСТРОЙКИ СИГНАЛОВ - ПАРАМЕТР ---
    "ПАРАМЕТР", "PARAMETER",
    "Выбор параметра для настройки.",
    "Choosing a setting for customization.",
    pSignals, FuncActive, OnPress_SetParameter, WAVE(Chan::A).GetForm(0)
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(cTypeTune,                                                                                      //--- НАСТРОЙКИ СИГНАЛОВ 2 - ЗАСЫЛКА ---
    "ЗАСЫЛКА", "SENDING",
    "Определяет периодичность засылки настроек сигналов в устройство управления.",
    "Specifies the frequency of sending signal settings to the control device.",
    "ОДНОКРАТНО", "SINGLE", "Засылка настроек сигнала происходит однократно, после нажатия кнопки подтверждения.",
    "The signal settings are sent once, after you press the confirm button.",
    "НЕПРЕРЫВНО", "CONTINUOUSLY", "Засылка настроек сигнала происходит синхронно с изменением информации на экране.",
    "Sending of signal settings occurs synchronously with changing information on the screen.",
    FLAG_1, BIT_TUNE_FULL, pSignals, FuncActive, FuncChangedChoice, FuncDraw
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
static bool OnKey(Control control)
{
    if(control.Is(Control::Esc))
    {
        return FORM_CURRENT->CloseOpenedParameter();
    }

    return false;
}

DEF_PAGE_5( pSignals,
    "НАСТРОЙКИ СИГНАЛОВ", "SIGNAL SETTINGS",
    "", "",
    cChannel,       ///< НАСТРОЙКИ СИГНАЛОВ - КАНАЛ
    cForm,          ///< НАСТРОЙКИ СИГНАЛОВ - ФОРМА
    cParameters,    ///< НАСТРОЙКИ СИГНАЛОВ - ПАРАМЕТР
    bSetParameter,  ///< НАСТРОЙКИ СИГНАЛОВ - ВВЕСТИ ЗНАЧЕНИЕ ПАРАМЕТРА
    cTypeTune,      ///< НАСТРОЙКИ СИГНАЛОВ - ЗАСЫЛКА
    Page::Settings, 0, FuncActive, FuncPress, OnKey
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
void PageSignals::Init()
{
    OnPress_Channel(true);
}
