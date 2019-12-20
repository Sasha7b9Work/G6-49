#include "GeneratroSettings.h"


SettingsGenerator setDDS =
{
    {
        {
            TransmissionSequence::MSB_2_LSB,    // TransmissionSequence
            false,                              // bidirectionalMode
            TypeWave::Sin,                      // typeWave
            1e3F,                               // frequency
            16383.0F,                           // amplitude
            2048.0F                             // offset
        },
        {
            TransmissionSequence::MSB_2_LSB,    // TransmissionSequence
            false,                              // bidirectionalMode
            TypeWave::Sin,                      // typeWave
            1e3F,                               // frequency
            16383.0F,                           // amplitude
            2048.0F                             // offset
        }
    }
};
