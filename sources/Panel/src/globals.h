#pragma once
#include "defines.h"
#include "Display/InputWindow.h"
#include "Settings/Settings.h"


#define INPUT_WINDOW_STRUCT(ch, form, param)    (sig_structParameter[ch][form][param])
#define AMPLITUDE(ch, form)                     (INPUT_WINDOW_STRUCT(ch, form, Amplitude))
#define AMPLITUDE_DIG(ch, form, digit)          (AMPLITUDE(ch, form).digits[digit])
#define AMPLITUDE_UNIT_STR(ch, form)            (AMPLITUDE(ch, form).strUnits[LANGUAGE])

#define FREQUENCY(ch, form)                     (INPUT_WINDOW_STRUCT(ch, form, Frequency))
#define FREQUENCY_DIG(ch, form, digit)          (FREQUENCY(ch, form).digits[digit])
#define FREQUENCY_UNIT_STR(ch, form)            (FREQUENCY(ch, form).strUnits[LANGUAGE])

#define PARAMETER(ch, form, param)              (INPUT_WINDOW_STRUCT(ch, form, param))
#define PARAMETER_DIG(ch, form, param, digit)   (PARAMETER(ch, *form, *param).buffer[digit])
#define PARAMETER_UNIT(ch, form, param)         (INPUT_WINDOW_STRUCT(ch, form, param).param)

extern InputWindow::Struct sig_structParameter[Chan::Number][Form::Number][Parameter::Number];




/// Текущий сигнал
#define WAVE(ch)            (wave[ch])
#define WAVE_CURRENT        (WAVE(CURRENT_CHANNEL))

#define FORM(ch)            (WAVE(ch).GetCurrentForm())
#define FORM_CURRENT        (FORM(CURRENT_CHANNEL))

#define PARAM(form)         ((form)->CurrentParameter())
#define PARAM_CURRENT       (PARAM(FORM_CURRENT))


extern Wave wave[Chan::Number];
