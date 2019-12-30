#pragma once
#include "defines.h"
#include "Display/InputWindow.h"
#include "Settings/Settings.h"


/// Текущий сигнал
#define WAVE(ch)                        (waves[ch])
#define CURRENT_WAVE                    (WAVE(CURRENT_CHANNEL))

#define FORM(ch)                        (WAVE(ch).GetCurrentForm())
#define CURRENT_FORM                    (FORM(CURRENT_CHANNEL))

#define PARAM(form)                     ((form)->CurrentParameter())
#define CURRENT_PARAM                   (PARAM(CURRENT_FORM))
#define PARAM_CURRENT_IS_MANIPULATION   (((ParameterValue *)CurrentParameter())->Is(ParameterValue::Manipulation))

extern Wave waves[Chan::Count];
