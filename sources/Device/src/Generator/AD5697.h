// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Common.h"
#include "common/Command.h"
#include "Hardware/CPU.h"
#include "Hardware/HAL/HAL_PIO.h"


struct AD5697
{
    static void Init();

    static void SetOffset(const Chan &);

    static void SetFreqLevel(double level);

    static void SetFreqHysteresys(double hyst);

    // Включает/отключает режим калибровки
    static void EnabledCalibrateMode(bool enable) { calibrateMode = enable; };

private:
    static void Reset(const Chan &);

    // Записать три байта данных по данному адресу
    static void WriteParameter(uint8 address, uint8 data[3], HPort::E port, uint16 pin);

    static StructPIN PinLDAC(const Chan &);

    // Рассчитать код смещения для смещения offset
    static double CalculateCodeOffset(const Chan &);

    static bool calibrateMode;
};
