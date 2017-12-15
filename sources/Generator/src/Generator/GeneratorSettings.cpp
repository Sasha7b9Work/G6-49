#include "GeneratroSettings.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SettingsGenerator setDDS =
{
    {
        {
            MSB_2_LSB,      // TransmissionSequence
            false,          // bidirectionalMode
            TypeWave_Sin,   // typeWave
            1e3f,           // frequency
            16383.0f,       // amplitude
            2048.0f         // offset
        },
        {
            MSB_2_LSB,      // TransmissionSequence
            false,          // bidirectionalMode
            TypeWave_Sin,   // typeWave
            1e3f,           // frequency
            16383.0f,       // amplitude
            2048.0f         // offset
        }
    }
};
