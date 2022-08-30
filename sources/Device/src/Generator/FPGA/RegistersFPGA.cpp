// 2022/08/30 12:22:42 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Generator/FPGA/RegistersFPGA.h"
#include "Hardware/HAL/HAL_PIO.h"
#include "Utils/Math.h"
#include "Hardware/HAL/HAL.h"


namespace FPGA
{
    // Установить на A0_RG...A3_RG адрес, соответсвующй регистру
    static void WriteAddress(Register::E reg);
}


void FPGA::Register::Write(const E reg, const uint64 _value)
{
    static const int numBits[Register::Count] =
    {
        16, // _0_Control,
        40, // _1_Freq,
        20, // _2_Amplitude,
        28, // _3_RectA,
        28, // _4_RectB,
        32, // _5_PeriodImpulseA,
        32, // _6_DurationImpulseA,
        32, // _7_PeriodImpulseB,
        32, // _8_DurationImpulseB,
        13, // _9_FreqMeter
        28, // _10_Offset
        2   // _11_Start
    };

    content[reg] = _value;

    WriteAddress(reg);

    for (int i = 0; i < 2; i++)
    {
        for (int bit = numBits[reg] - 1; bit >= 0; bit--)
        {
            HAL_PIO::Write(WR_FPGA_DT_RG, Bit::Get(_value, bit));    // Устанавливаем или сбрасываем соответствующий бит
            HAL_PIO::Set(WR_FPGA_CLK_RG);                           // И записываем его в ПЛИС
            HAL_PIO::Reset(WR_FPGA_CLK_RG);
        }
    }

    HAL_PIO::Set(WR_FPGA_WR_RG);                                    // Теперь переписываем данные из сдвиговоого регистра в FPGA
    HAL_PIO::Reset(WR_FPGA_WR_RG);
    HAL_TIM::Delay(10);                                         // Ждём 10 миллисекунд, пока данные перепишутся в FPGA
}


void FPGA::WriteAddress(Register::E reg)
{
    HAL_PIO::Write(WR_FPGA_A0_RG, Bit::Get(reg, 0));
    HAL_PIO::Write(WR_FPGA_A1_RG, Bit::Get(reg, 1));
    HAL_PIO::Write(WR_FPGA_A2_RG, Bit::Get(reg, 2));
    HAL_PIO::Write(WR_FPGA_A3_RG, Bit::Get(reg, 3));
}
