#pragma once
#include "Display/InputWindow.h"
#include "Settings/Settings.h"
#include "Menu/MenuItems.h"
#include "AddPageInput.h"
#include "PageSettings.h"
#include "Generator/Signals.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pSignals;
Page *PageSignals::pointer = (Page *)&pSignals;
extern ChoiceParameterBase parameters;
ChoiceParameter *PageSignals::cpParameters = (ChoiceParameter *)&parameters;
/// Номер текущей формы сигнал
static int numForm = 0;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnPress_Channel(bool)
{
    parameters.form = FORM_CURRENT;
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

    parameters.form = FORM_CURRENT;

    Settings::TuneGenerator(CURRENT_CHANNEL);
}

DEF_CHOICE_5( cForm,                                                                                              //--- НАСТРОЙКИ СИГНАЛОВ - ФОРМА ---
    "ФОРМА", "FORM",
    "Выбор формы сигнала.",
    "Select waveform.",
    FORM_RU(Form::Sine),          FORM_EN(Form::Sine),          "Синус.",               "Sinus.",
    FORM_RU(Form::RampPlus),      FORM_EN(Form::RampPlus),      "Нарастающая пила.",    "Growing saw.",
    FORM_RU(Form::RampMinus),     FORM_EN(Form::RampMinus),     "Убывающая пила.",      "Wrecking saw.",
    FORM_RU(Form::Meander),       FORM_EN(Form::Meander),       "Меандр.",              "Meander.",
    FORM_RU(Form::Impulse),       FORM_EN(Form::Impulse),       "Треугольник.",         "Triangle.",
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
    else
    {
        FORM_CURRENT->OpenCurrentParameter();
    }
}

DEF_BUTTON( bSetParameter,                                                                    //--- НАСТРОЙКИ СИГНАЛОВ - ВВЕСТИ ЗНАЧЕНИЕ ПАРАМЕТРА ---
    "ВВЕСТИ ЗНАЧЕНИЕ ПАРАМЕТРА", "ENTER VALUE PARAMETER",
    "Открывает окно ввода параметра.",
    "Opens the parameter input window.",
    pSignals, FuncActive, OnPress_SetParameter, FuncDraw
)

DEF_CHOICE_PARAMETER(parameters,                                                                               //--- НАСТРОЙКИ СИГНАЛОВ - ПАРАМЕТР ---
    "ПАРАМЕТР", "PARAMETER",
    "Выбор параметра для настройки.",
    "Choosing a setting for customization.",
    pSignals, FuncActive, OnPress_SetParameter, WAVE(Chan::A).GetForm(0)
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_PAGE_4
(
    pSignals,
    "НАСТРОЙКИ СИГНАЛОВ", "SIGNAL SETTINGS",
    "", "",
    cChannel,       ///< НАСТРОЙКИ СИГНАЛОВ - КАНАЛ
    cForm,          ///< НАСТРОЙКИ СИГНАЛОВ - ФОРМА
    parameters,     ///< НАСТРОЙКИ СИГНАЛОВ - ПАРАМЕТР
    bSetParameter,  ///< НАСТРОЙКИ СИГНАЛОВ - ВВЕСТИ ЗНАЧЕНИЕ ПАРАМЕТРА
    Page_Settings, 0, FuncActive, FuncPress, FuncOnKey
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
void PageSignals::Init()
{
    OnPress_Channel(true);
}

