#pragma once


struct StructCalibration
{
    static int16 *GetK(int channel, int signal, int range, uint8 parameter);
};
