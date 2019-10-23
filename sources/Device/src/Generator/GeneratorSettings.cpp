#include "stdafx.h"
#ifndef WIN32
#include "GeneratroSettings.h"
#endif



SettingsGenerator setDDS =
{
    {
        {
            TransmissionSequence::MSB_2_LSB,    // TransmissionSequence
            false,                              // bidirectionalMode
            TypeWave::Sin,                      // typeWave
            1e3f,                               // frequency
            16383.0f,                           // amplitude
            2048.0f                             // offset
        },
        {
            TransmissionSequence::MSB_2_LSB,    // TransmissionSequence
            false,                              // bidirectionalMode
            TypeWave::Sin,                      // typeWave
            1e3f,                               // frequency
            16383.0f,                           // amplitude
            2048.0f                             // offset
        }
    }
};
