#pragma once
#include "defines.h"
#include "Display/InputWindow.h"


#define INPUT_WINDOW_STRUCT(ch, form, param)    (sig_structParameter[ch][form][param])
#define AMPLITUDE(ch, form)                     (INPUT_WINDOW_STRUCT(ch, form, Amplitude))
#define AMPLITUDE_DIG(ch, form, digit)          (AMPLITUDE(ch, form).digits[digit])
#define AMPLITUDE_UNIT_STR(ch, form)            (AMPLITUDE(ch, form).strUnits[LANGUAGE])

#define FREQUENCY(ch, form)                     (INPUT_WINDOW_STRUCT(ch, form, Frequency))
#define FREQUENCY_DIG(ch, form, digit)          (FREQUENCY(ch, form).digits[digit])
#define FREQUENCY_UNIT_STR(ch, form)            (FREQUENCY(ch, form).strUnits[LANGUAGE])

#define PARAMETER(ch, form, param)              (INPUT_WINDOW_STRUCT(ch, form, param))
#define PARAMETER_DIG(ch, form, param, digit)   (PARAMETER(ch, form, param).buffer[digit])
#define PARAMETER_ORDER(ch, form, param)        (INPUT_WINDOW_STRUCT(ch, form, param).order)
#define PARAMETER_UNIT(ch, form, param)         (INPUT_WINDOW_STRUCT(ch, form, param).param)

extern InputWindow::Struct sig_structParameter[Chan::Number][Wave::Form::Number][Wave::Form::Parameter::Number];
