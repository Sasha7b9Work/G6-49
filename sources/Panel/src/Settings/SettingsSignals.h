#pragma once
#include "Settings.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define AMPLITUDE(ch, form)             (set.sig_structParameter[ch][form][Amplitude])
#define AMPLITUDE_DIG(ch, form, digit)  (AMPLITUDE(ch, form).digits[digit])
#define AMPLITUDE_UNIT_STR(ch, form)    (AMPLITUDE(ch, form).strUnits[LANGUAGE])

#define FREQUENCY(ch, form)             (set.sig_structParameter[ch][form][Frequency])
#define FREQUENCY_DIG(ch, form, digit)  (FREQUENCY(ch, form).digits[digit])
#define FREQUENCY_UNIT_STR(ch, form)    (FREQUENCY(ch, form).strUnits[LANGUAGE])

#define PARAMETER(ch, form, param)              (set.sig_structParameter[ch][form][param])
#define PARAMETER_DIG(ch, form, param, digit)   (PARAMETER(ch, form, param).inputBuffer[digit])
#define PARAMETER_ORDER(ch, form, param)        (set.sig_structParameter[ch][form][param].order)
#define PARAMETER_UNIT(ch, form, param)         (set.sig_structParameter[ch][form][param].param)
