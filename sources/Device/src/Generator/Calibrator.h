#pragma once


struct Calibrator
{
    static void SetK(uint8 channel, uint8 signal, uint8 range, uint8 param, int16 k);
};
