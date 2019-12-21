#include "defines.h"
#include "log.h"
#include "Message.h"
#include "FreqMeter_d.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL_PIO.h"
#include "Utils/Debug.h"


void DFreqMeter::Init()
{
    /*
        PE14 - сопротивление 1 МОм/50 Ом
        PE15 - AC/DC
        PB10 - ФНЧ вкл/откл

        Инициализируются в CPU::InitPins()
    */

}


void DFreqMeter::SetResist(FreqResist resist)
{
    CPU::WritePin(GeneratorWritePin::FREQ_METER_RESIST, resist.Is(FreqResist::_50Ohm));
}


void DFreqMeter::SetCouple(FreqCouple couple)
{
    CPU::WritePin(GeneratorWritePin::FREQ_METER_COUPLE, couple.Is(FreqCouple::DC));
}


void DFreqMeter::SetFiltr(FreqFiltr filtr)
{
    CPU::WritePin(GeneratorWritePin::FREQ_METER_FILTR, filtr.Is(FreqFiltr::Enable));
}


void DFreqMeter::Update()
{
    if(HAL_PIO::Read(RD_FREQ_METER_DRY))
    {
        uint frequency = 0;
        for(int i = 30; i >= 0; i--)
        {
            CPU::WritePin(GeneratorWritePin::FREQ_METER_CLK, true);
            volatile int j = 0;
            while(j < 25)
            {
                j++;
            }
            CPU::WritePin(GeneratorWritePin::FREQ_METER_CLK, false);
            Timer::PauseOnTime(1);
            if (HAL_PIO::Read(RD_FREQ_METER_DATA))
            {
                frequency += (1 << i);
            }
        }

        Message::FreqMeter::Measure(frequency).Transmit();
    }
}

