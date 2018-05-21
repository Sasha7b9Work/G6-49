#pragma once
#include "Display/InputWindow.h"
#include "Settings/Settings.h"
#include "Settings/SettingsSignals.h"
#include "Menu/MenuItems.h"
#include "AddPageInput.h"
#include "PageSettings.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pSignals;
Page *PageSignals::pointer = (Page *)&pSignals;
extern ChoiceParameterBase parameters;
ChoiceParameter *PageSignals::cpParameters = (ChoiceParameter *)&parameters;

static uint8 waveForm = 0;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnPress_Channel(bool)
{
    waveForm = WAVE_FORM.ToValue();
    InputWindowStruct::FillAllowParameters(CURRENT_CHANNEL, WAVE_FORM, &parameters.allowParameters);
    parameters.numParameter = (uint8 *)&CURRENT_PARAMETER(WAVE_FORM);
}

DEF_CHOICE_2( cChannel,                                                                                           //--- Õ¿—“–Œ… » —»√Õ¿ÀŒ¬ -  ¿Õ¿À ---
    " ¿Õ¿À", "CHANNEL",
    "¬€¡Œ–  ¿Õ¿À¿ ƒÀﬂ Õ¿—“–Œ… »",
    "SELECTING A CHANNEL TO SET UP",
    "A", "A",
    "B", "B",
    CURRENT_CHANNEL, pSignals, FuncActive, OnPress_Channel, FuncDraw
)


//----------------------------------------------------------------------------------------------------------------------------------------------------
/*
static void SetWaveForm(Channel ch, WaveForm form)
{
    set.sig_form[ch] = form;
    TuneGenerator(ch);
}
*/

void PageSignals::OnPress_Form(bool)
{
    WaveForm form = WaveForm(waveForm);

    /*
    if (form.type == Form_Saw || form.type == Form_Impulse)
    {
        SetWaveForm(A, form);
        SetWaveForm(B, form);
    }
    */

    WAVE_FORM = form;
    InputWindowStruct::FillAllowParameters(CURRENT_CHANNEL, WAVE_FORM, &parameters.allowParameters);
    parameters.numParameter = (uint8 *)&CURRENT_PARAMETER(WAVE_FORM);
    TuneGenerator(CURRENT_CHANNEL);
}

DEF_CHOICE_7( cForm,                                                                                              //--- Õ¿—“–Œ… » —»√Õ¿ÀŒ¬ - ‘Œ–Ã¿ ---
    "‘Œ–Ã¿", "FORM",
    "¬€¡Œ– ‘Œ–Ã€ —»√Õ¿À¿",
    "SELECT WAVEFORM",
    "—»Õ”—",        "SINE",
    " Œ—»Õ”—",      "COSINE",
    "Ã≈¿Õƒ–",       "MEANDER",
    "œ»À¿+",        "SAW+",
    "œ»À¿-",        "SAW-",
    "»Ãœ”À‹—",      "IMPULSE",
    "œ–Œ»«¬ŒÀ‹Õ€…", "FREE",
    waveForm, pSignals, FuncActive, PageSignals::OnPress_Form, FuncDraw
)


//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_SetParameter()
{
    InputWindow::Init();

    ADDITION_PAGE = (Page *)&pInput;
}

DEF_BUTTON( bSetParameter,                                                                    //--- Õ¿—“–Œ… » —»√Õ¿ÀŒ¬ - ¬¬≈—“» «Õ¿◊≈Õ»≈ œ¿–¿Ã≈“–¿ ---
    "¬¬≈—“» «Õ¿◊≈Õ»≈ œ¿–¿Ã≈“–¿", "ENTER VALUE PARAMETER",
    "Œ“ –€¬¿≈“ Œ ÕŒ ¬¬Œƒ¿ œ¿–¿Ã≈“–¿",
    "OPENS THE PARAMETER INPUT WINDOW",
    pSignals, FuncActive, OnPress_SetParameter, FuncDraw
)

DEF_CHOICE_PARAMETER(parameters,                                                                               //--- Õ¿—“–Œ… » —»√Õ¿ÀŒ¬ - œ¿–¿Ã≈“– ---
    "œ¿–¿Ã≈“–", "PARAMETER",
    "¬€¡Œ– œ¿–¿Ã≈“–¿ ƒÀﬂ Õ¿—“–Œ… »",
    "CHOOSING A SETTING FOR CUSTOMIZATION",
    pSignals, FuncActive, OnPress_SetParameter, (CURRENT_PARAMETER(WaveForm(waveForm))),
    true, true, true, true, false, false, false, false
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
void PageSignals::Init()
{
    waveForm = WAVE_FORM.ToValue();
    InputWindowStruct::FillAllowParameters(CURRENT_CHANNEL, WAVE_FORM, &parameters.allowParameters);
    parameters.numParameter = (uint8 *)&CURRENT_PARAMETER(WaveForm(waveForm));
}

DEF_PAGE_4
(
    pSignals,
    "Õ¿—“–Œ… » —»√Õ¿ÀŒ¬", "SIGNAL SETTINGS",
    "", "",
    cChannel,       ///< Õ¿—“–Œ… » —»√Õ¿ÀŒ¬ -  ¿Õ¿À
    cForm,          ///< Õ¿—“–Œ… » —»√Õ¿ÀŒ¬ - ‘Œ–Ã¿
    parameters,     ///< Õ¿—“–Œ… » —»√Õ¿ÀŒ¬ - œ¿–¿Ã≈“–
    bSetParameter,  ///< Õ¿—“–Œ… » —»√Õ¿ÀŒ¬ - ¬¬≈—“» «Õ¿◊≈Õ»≈ œ¿–¿Ã≈“–¿
    Page_Settings, 0, FuncActive, FuncPress
)
