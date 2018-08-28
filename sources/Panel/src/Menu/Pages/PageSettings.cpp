#pragma once
#include "Display/InputWindow.h"
#include "Settings/Settings.h"
#include "Menu/MenuItems.h"
#include "AddPageInput.h"
#include "PageSettings.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pSignals;
Page *PageSignals::pointer = (Page *)&pSignals;
extern ChoiceParameterBase parameters;
ChoiceParameter *PageSignals::cpParameters = (ChoiceParameter *)&parameters;

static Wave::Form waveForm = Wave::Form::Sine;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnPress_Channel(bool)
{
    waveForm = WAVE_FORM_CURRENT;
    InputWindow::FillAllowParameters(CURRENT_CHANNEL, WAVE_FORM_CURRENT, &parameters.allowParameters);
    parameters.numParameter = (uint8 *)&CURRENT_PARAMETER(WAVE_FORM_CURRENT);
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
void PageSignals::OnPress_Form(bool)
{
    WAVE_FORM(CURRENT_CHANNEL) = waveForm;
    InputWindow::FillAllowParameters(CURRENT_CHANNEL, WAVE_FORM_CURRENT, &parameters.allowParameters);
    parameters.numParameter = (uint8 *)&CURRENT_PARAMETER(WAVE_FORM_CURRENT);
    Settings::TuneGenerator(CURRENT_CHANNEL);
}

DEF_CHOICE_3( cForm,                                                                                              //--- НАСТРОЙКИ СИГНАЛОВ - ФОРМА ---
    "ФОРМА", "FORM",
    "Выбор формы сигнала.",
    "Select waveform.",
    FORM_RU(Wave::Form::Sine),            FORM_EN(Wave::Form::Sine),        "Синус.",                       "Sinus.",
    FORM_RU(Wave::Form::RampPlus),        FORM_EN(Wave::Form::RampPlus),    "Нарастающая пила.",            "Growing saw.",
    FORM_RU(Wave::Form::RampMinus),       FORM_EN(Wave::Form::RampMinus),   "Убывающая пила.",              "Wrecking saw.",
//    FORM_RU(Wave::Form::Triangle),        FORM_EN(Wave::Form::Triangle),    "Треугольник.",                 "Triangle.",
//    FORM_RU(Wave::Form::Trapeze),         FORM_EN(Wave::Form::Trapeze),     "Трапеция.",                    "Trapeze.",
//    FORM_RU(Wave::Form::Impulse),         FORM_EN(Wave::Form::Impulse),     "Импульс.",                     "Impulse.",
//    FORM_RU(Wave::Form::ExpPlus),         FORM_EN(Wave::Form::ExpPlus),     "Возрастающая экспонента.",     "Growing exponent.",
//    FORM_RU(Wave::Form::ExpMinus),        FORM_EN(Wave::Form::ExpMinus),    "Убывающая экспонента.",        "Decreasing exponent.",
//    FORM_RU(Wave::Form::Noise),           FORM_EN(Wave::Form::Noise),       "Шум.",                         "Noise.",
//    FORM_RU(Wave::Form::Free),            FORM_EN(Wave::Form::Free),        "Произвольная форма сигнала.",  "Arbitrary waveform.",
//    FORM_RU(Wave::Form::Cosine),          FORM_EN(Wave::Form::Cosine),      "Косинус.",                     "Cosine.",
//    FORM_RU(Wave::Form::Meander),         FORM_EN(Wave::Form::Meander),     "Меандр.",                      "Meander.",
    waveForm, pSignals, FuncActive,     PageSignals::OnPress_Form, FuncDraw
)


//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_SetParameter()
{
    InputWindow::Init();

    ADDITION_PAGE = (Page *)&pInput;
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
    pSignals, FuncActive, OnPress_SetParameter, (CURRENT_PARAMETER(waveForm)),
    true, true, true, true, false, false, false, false
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
void PageSignals::Init()
{
    waveForm = WAVE_FORM_CURRENT;
    InputWindow::FillAllowParameters(CURRENT_CHANNEL, WAVE_FORM_CURRENT, &parameters.allowParameters);
    parameters.numParameter = (uint8 *)&CURRENT_PARAMETER(Wave::Form(waveForm));
}

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
