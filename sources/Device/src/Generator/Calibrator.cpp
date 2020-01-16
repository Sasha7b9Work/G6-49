#include "defines.h"
#include "Generator/Calibrator.h"
#include "Settings/CalibrationSettings.h"


void Calibrator::SetK(uint8 channel, uint8 signal, uint8 range, uint8 param, int16 k)
{
    *setCal.GetK(channel, signal, range, param) = k;
}
