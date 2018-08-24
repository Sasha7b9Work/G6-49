#pragma once
#include "defines.h"
#include "GeneratorSettingsTypes.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct SettingsAD9952
{
    TransmissionSequence transmissionSequence;  ///< D8 Направление передачи бит
    bool        bidirectionalMode;              ///< D9 Если true, то работает в двунаправленном режиме
    TypeWave    typeWave;                       ///< D12 Выбор синуса или косинуса
    
    float       frequency;                      ///< Частота
    float       amplitude;
    float       offset;
};


struct SettingsGenerator
{
    SettingsAD9952 ad9952[2];
};

extern SettingsGenerator setDDS;
