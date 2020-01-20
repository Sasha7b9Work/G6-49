#pragma once
#include "common/CommonTypes.h"



/// Последовательность передачи данных - от старшего к младшему  или от младшего к старшему
struct TransmissionSequence
{
    enum E
    {
        MSB_2_LSB,          ///< От старшего бита к младшему
        LSB_2_MSB           ///< От младшего бита к старшему
    } value;
    TransmissionSequence(E v) : value(v) {};
};

struct TypeWave
{
    enum E
    {
        Sin,
        Cos
    } value;
    TypeWave(E v) : value(v) {};
};
