#pragma once
#include "defines.h"
#include "Display/InputWindow.h"
#include "Settings/Settings.h"


/// Текущий сигнал
#define WAVE(ch)            (wave[ch])
#define WAVE_CURRENT        (WAVE(CURRENT_CHANNEL))

#define FORM(ch)            (WAVE(ch).GetCurrentForm())
#define FORM_CURRENT        (FORM(CURRENT_CHANNEL))

#define PARAM(form)         ((form)->CurrentParameter())
#define PARAM_CURRENT       (PARAM(FORM_CURRENT))


extern Wave wave[Chan::Number];
