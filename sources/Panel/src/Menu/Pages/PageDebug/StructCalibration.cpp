#include "defines.h"
#include "Menu/Pages/PageDebug/StructCalibration.h"
#include "Settings/Settings.h"



int16 *StructCalibration::GetK(int channel, int signal, int range, uint8 parameter)
{
    if(channel > 1)
    {
        channel = 1;
    }

    if(signal > 1)
    {
        signal = 1;
    }

    return &set.k[channel][signal][range][parameter];
}
