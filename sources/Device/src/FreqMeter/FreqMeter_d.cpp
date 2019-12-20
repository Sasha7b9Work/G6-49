#include "defines.h"
#include "log.h"
#include "Message.h"
#include "FreqMeter_d.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "Utils/Debug.h"


void FreqMeter::Init()
{
    /*
        PE14 - сопротивление 1 МОм/50 Ом
        PE15 - AC/DC
        PB10 - ФНЧ вкл/откл

        Инициализируются в CPU::InitPins()
    */

}


void FreqMeter::SetResist(FreqResist resist)
{
    CPU::WritePin(GeneratorWritePin::FREQ_METER_RESIST, resist.Is(FreqResist::_50Ohm));
}


void FreqMeter::SetCouple(FreqCouple couple)
{
    CPU::WritePin(GeneratorWritePin::FREQ_METER_COUPLE, couple.Is(FreqCouple::DC));
}


void FreqMeter::SetFiltr(FreqFiltr filtr)
{
    CPU::WritePin(GeneratorWritePin::FREQ_METER_FILTR, filtr.Is(FreqFiltr::Enable));
}


void FreqMeter::Update()
{
    if(CPU::ReadPin(GeneratorReadPin::FREQ_METER_DRY))
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
            if (CPU::ReadPin(GeneratorReadPin::FREQ_METER_DATA))
            {
                frequency += (1 << i);
            }
        }

        Message::FreqMeter::Measure(frequency).Transmit();
    }
}

