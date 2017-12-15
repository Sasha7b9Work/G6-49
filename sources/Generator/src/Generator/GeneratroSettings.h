#pragma once
#include "defines.h"
#include "GeneratorSettingsTypes.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct
{
    TransmissionSequence transmissionSequence;      ///< D8 Направление передачи бит
    bool        bidirectionalMode;                  ///< D9 Если true, то работает в двунаправленном режиме
    TypeWave    typeWave;                           ///< D12 Выбор синуса или косинуса

    float       frequency;                          ///< Частота
    float       amplitude;
    float       offset;
} SettingsAD9952;


typedef struct
{
    SettingsAD9952 ad9952[2];
} SettingsGenerator;

extern SettingsGenerator setDDS;
