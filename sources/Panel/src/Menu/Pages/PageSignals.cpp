#pragma once
#include "Display/InputWindow.h"
#include "Settings/Settings.h"
#include "Settings/SettingsSignals.h"
#include "Menu/MenuItems.h"
#include "AddPageInput.h"
#include "PageSignals.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern PageBase pSignals;
Page *PageSignals::pointer = (Page *)&pSignals;

static void OnPress_SetParameter();

static uint8 waveForm = 0;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnPress_Channel(bool)
{
    waveForm = (uint8)WAVE_FORM;
    InputWindowStruct::FillAllowParameters(CHANNEL, WAVE_FORM, &PageSignals::cpParameters.allowParameters);
    PageSignals::cpParameters.numParameter = (uint8 *)&(set.sig_parameter[WAVE_FORM]);
}

DEF_CHOICE_2( cChannel,                                                                                           //--- Õ¿—“–Œ… » —»√Õ¿ÀŒ¬ -  ¿Õ¿À ---
    " ¿Õ¿À", "CHANNEL",
    "¬˚·Ó Í‡Ì‡Î‡ ‰Îˇ Ì‡ÒÚÓÈÍË",
    "Selecting a channel to set up",
    "1", "1",
    "2", "2",
    CHANNEL, pSignals, FuncActive, OnPress_Channel, FuncDraw
)

// Õ¿—“–Œ… » —»√Õ¿ÀŒ¬ - ‘Œ–Ã¿ ------------------------------------------------------------------------------------------------------------------------
static const Choice cForm =
{
    Item_Choice, {}, OnPress_Form,
    {
        "‘Œ–Ã¿", "FORM"
    },
    {
        "—»Õ”—",    "SINE",
        "œ»À¿",     "SAW",
        "»Ãœ”À‹—",  "IMPULSE"
    },
    (uint8*)&waveForm
};

static void SetWaveForm(Channel ch, WaveForm form)
{
    set.sig_form[ch] = form;
    TuneGenerator(ch);
}

void PageSignals::OnPress_Form()
{
    WaveForm form = (WaveForm)waveForm;

    if (form == Form_Saw || form == Form_Impulse)
    {
        SetWaveForm(A, form);
        SetWaveForm(B, form);
    }

    WAVE_FORM = form;
    InputWindowStruct::FillAllowParameters(CHANNEL, WAVE_FORM, &cpParameters.allowParameters);
    cpParameters.numParameter = (uint8 *)&(set.sig_parameter[WAVE_FORM]);
    TuneGenerator(CHANNEL);
}

// Õ¿—“–Œ… » —»√Õ¿ÀŒ¬ - ¬¬≈—“» «Õ¿◊≈Õ»≈ œ¿–¿Ã≈“–¿ ----------------------------------------------------------------------------------------------------
static const Button bSetParameter =
{
    Item_Button, {}, OnPress_SetParameter,
    {
        "¬¬≈—“» «Õ¿◊≈Õ»≈ œ¿–¿Ã≈“–¿", "ENTER VALUE PARAMETER"
    }
};

static void OnPress_SetParameter(void)
{
    InputWindow::Init();

    ADDITION_PAGE = &pInput;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void PageSignals::Init()
{
    waveForm = (uint8)WAVE_FORM;
    InputWindowStruct::FillAllowParameters(CHANNEL, WAVE_FORM, &cpParameters.allowParameters);
    cpParameters.numParameter = (uint8 *)&(set.sig_parameter[(WaveForm)waveForm]);
}

DEF_CHOICE_PARAMETER(cpParameters,                                                                             //--- Õ¿—“–Œ… » —»√Õ¿ÀŒ¬ - œ¿–¿Ã≈“– ---
                     "œ¿–¿Ã≈“–", "PARAMETER",
                     "¬˚·Ó Ô‡‡ÏÂÚ‡ ‰Îˇ Ì‡ÒÚÓÈÍË",
                     "Choosing a setting for customization",
                     pSignals, FuncActive, OnPress_SetParameter, (set.sig_parameter[(WaveForm)waveForm]),
                     true, true, true, true, false, false, false, false
)

const PageBase pSignals =
{
    Item_Page, {}, 0,
    {
        "Õ¿—“–Œ… » —»√Õ¿ÀŒ¬", "SIGNAL SETTINGS"
    },
    {
        (void*)&cChannel,       ///< Õ¿—“–Œ… » —»√Õ¿ÀŒ¬ -  ¿Õ¿À
        (void*)&cForm,          ///< Õ¿—“–Œ… » —»√Õ¿ÀŒ¬ - ‘Œ–Ã¿
        (void*)&cpParameters,   ///< Õ¿—“–Œ… » —»√Õ¿ÀŒ¬ - œ¿–¿Ã≈“–
        (void*)&bSetParameter   ///< Õ¿—“–Œ… » —»√Õ¿ÀŒ¬ - ¬¬≈—“» «Õ¿◊≈Õ»≈ œ¿–¿Ã≈“–¿
    }
};
