#pragma once
#include "CommonTypes.h"
#include "Wave.h"


struct Calibrator
{
    static void SetK(uint8 channel, uint8 signal, uint8 range, uint8 param, int16 k);

    static float GetAmplitudeK(Chan::E ch, Form::E form, float amplitude);

private:
    static uint8 CalculateRange(float amplitude);
};
