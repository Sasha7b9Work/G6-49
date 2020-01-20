#pragma once
#include "Common.h"
#include "CommonTypes.h"


struct Calibrator
{
    static void SetK(uint8 channel, uint8 signal, uint8 range, uint8 param, int16 k);

    static float GetAmplitudeK(Chan::E ch, TypeForm::E form, float amplitude);

private:
    static uint8 CalculateRange(float amplitude);
};
