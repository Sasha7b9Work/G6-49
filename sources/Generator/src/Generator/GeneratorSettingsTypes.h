#pragma once
#include "CommonTypes.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Последовательность передачи данных - от старшего к младшему  или от младшего к старшему
struct TransmissionSequence
{
    enum
    {
        MSB_2_LSB,          ///< От старшего бита к младшему
        LSB_2_MSB           ///< От младшего бита к старшему
    };
    uint8 value;
    TransmissionSequence(uint8 v) : value(v)
    {
    };
};

struct TypeWave
{
    enum
    {
        Sin,
        Cos
    };
    uint8 value;
    TypeWave(uint8 v) : value(v)
    {
    };
};
