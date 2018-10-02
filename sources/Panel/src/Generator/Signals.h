#pragma once
#include "defines.h"
#include "Display/InputWindow.h"
#include "Settings/Settings.h"


/// Текущий сигнал
#define WAVE(ch)                        (wave[ch])
#define WAVE_CURRENT                    (WAVE(CURRENT_CHANNEL))

#define FORM(ch)                        (WAVE(ch).GetCurrentForm())
#define FORM_CURRENT                    (FORM(CURRENT_CHANNEL))

#define PARAM(form)                     ((form)->CurrentParameter())
#define PARAM_CURRENT                   (PARAM(FORM_CURRENT))
#define PARAM_VALUE(param)              ((ParameterValue *)param)
#define PARAM_CURRENT_IS_MANIPULATION   (((ParameterValue *)CurrentParameter())->Is(ParameterValue::Manipulation))
#define PARAM_CURRENT_IS_EXIT           (((ParameterValue *)PARAM_CURRENT)->IsExitParameter())
#define PARAM_CURRENT_IS_COMPLEX        (((ParameterValue *)PARAM_CURRENT)->IsComplexParameter())
#define PARAM_CURRENT_PARENT            (((ParameterValue *)PARAM_CURRENT)->GetParent())



extern Wave wave[Chan::Number];
