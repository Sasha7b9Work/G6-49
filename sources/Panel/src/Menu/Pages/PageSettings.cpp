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
    waveForm = (uint8)WAVE_FORM;
    InputWindowStruct::FillAllowParameters(CURRENT_CHANNEL, WAVE_FORM, &parameters.allowParameters);
    parameters.numParameter = (uint8 *)&CURRENT_PARAMETER(WAVE_FORM);
}

DEF_CHOICE_2( cChannel,                                                                                           //--- ÍÀÑÒĞÎÉÊÈ ÑÈÃÍÀËÎÂ - ÊÀÍÀË ---
    "ÊÀÍÀË", "CHANNEL",
    "Âûáîğ êàíàëà äëÿ íàñòğîéêè",
    "Selecting a channel to set up",
    "A", "A",
    "B", "B",
    CURRENT_CHANNEL, pSignals, FuncActive, OnPress_Channel, FuncDraw
)


//----------------------------------------------------------------------------------------------------------------------------------------------------
static void SetWaveForm(Channel ch, WaveForm form)
{
    set.sig_form[ch] = form;
    TuneGenerator(ch);
}

void PageSignals::OnPress_Form(bool)
{
    WaveForm form = (WaveForm)waveForm;

    if (form == Form_Saw || form == Form_Impulse)
    {
        SetWaveForm(A, form);
        SetWaveForm(B, form);
    }

    WAVE_FORM = form;
    InputWindowStruct::FillAllowParameters(CURRENT_CHANNEL, WAVE_FORM, &parameters.allowParameters);
    parameters.numParameter = (uint8 *)&CURRENT_PARAMETER(WAVE_FORM);
    TuneGenerator(CURRENT_CHANNEL);
}

DEF_CHOICE_3( cForm,                                                                                              //--- ÍÀÑÒĞÎÉÊÈ ÑÈÃÍÀËÎÂ - ÔÎĞÌÀ ---
    "ÔÎĞÌÀ", "FORM",
    "Âûáîğ ôîğìû ñèãíàëà",
    "Select waveform",
    "ÑÈÍÓÑ",   "SINE",
    "ÏÈËÀ",    "SAW",
    "ÈÌÏÓËÜÑ", "IMPULSE",
    waveForm, pSignals, FuncActive, PageSignals::OnPress_Form, FuncDraw
)


//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_SetParameter()
{
    InputWindow::Init();

    ADDITION_PAGE = (Page *)&pInput;
}

DEF_BUTTON( bSetParameter,                                                                    //--- ÍÀÑÒĞÎÉÊÈ ÑÈÃÍÀËÎÂ - ÂÂÅÑÒÈ ÇÍÀ×ÅÍÈÅ ÏÀĞÀÌÅÒĞÀ ---
    "ÂÂÅÑÒÈ ÇÍÀ×ÅÍÈÅ ÏÀĞÀÌÅÒĞÀ", "ENTER VALUE PARAMETER",
    "Îòêğûâàåò îêíî ââîäà ïàğàìåòğà",
    "Opens the parameter input window",
    pSignals, FuncActive, OnPress_SetParameter, FuncDraw
)

DEF_CHOICE_PARAMETER(parameters,                                                                               //--- ÍÀÑÒĞÎÉÊÈ ÑÈÃÍÀËÎÂ - ÏÀĞÀÌÅÒĞ ---
    "ÏÀĞÀÌÅÒĞ", "PARAMETER",
    "Âûáîğ ïàğàìåòğà äëÿ íàñòğîéêè",
    "Choosing a setting for customization",
    pSignals, FuncActive, OnPress_SetParameter, (CURRENT_PARAMETER((WaveForm)waveForm)),
    true, true, true, true, false, false, false, false
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
void PageSignals::Init()
{
    waveForm = (uint8)WAVE_FORM;
    InputWindowStruct::FillAllowParameters(CURRENT_CHANNEL, WAVE_FORM, &parameters.allowParameters);
    parameters.numParameter = (uint8 *)&CURRENT_PARAMETER((WaveForm)waveForm);
}

DEF_PAGE_4
(
    pSignals,
    "ÍÀÑÒĞÎÉÊÈ ÑÈÃÍÀËÎÂ", "SIGNAL SETTINGS",
    "", "",
    cChannel,       ///< ÍÀÑÒĞÎÉÊÈ ÑÈÃÍÀËÎÂ - ÊÀÍÀË
    cForm,          ///< ÍÀÑÒĞÎÉÊÈ ÑÈÃÍÀËÎÂ - ÔÎĞÌÀ
    parameters,     ///< ÍÀÑÒĞÎÉÊÈ ÑÈÃÍÀËÎÂ - ÏÀĞÀÌÅÒĞ
    bSetParameter,  ///< ÍÀÑÒĞÎÉÊÈ ÑÈÃÍÀËÎÂ - ÂÂÅÑÒÈ ÇÍÀ×ÅÍÈÅ ÏÀĞÀÌÅÒĞÀ
    Page_Settings, 0, FuncActive, FuncPress
)
