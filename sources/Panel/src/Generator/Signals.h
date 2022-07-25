// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Settings/Settings.h"


// ������� ������
#define WAVE(ch)                        (waves[ch])
#define CURRENT_WAVE                    (WAVE(CURRENT_CHANNEL))

#define FORM(ch)                        (WAVE(ch).GetCurrentForm())
#define CURRENT_FORM                    (FORM(CURRENT_CHANNEL))

#define PARAM(form)                     ((form)->CurrentParameter())
#define CURRENT_PARAM                   (PARAM(CURRENT_FORM))

extern Wave waves[Chan::Count];
