#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "FreqMeter.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "Interface/InterfaceDevice.h"
#include "Utils/Debug.h"
#include "Settings/Settings.h"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FreqMeter::Init()
{
    /*
        PE14 - сопротивление 1 МОм/50 Ом
        PE15 - AC/DC
        PB10 - ФНЧ вкл/откл

        Инициализируются в CPU::InitPins()
    */

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FreqMeter::SetResist(FreqResist resist)
{
    CPU::WritePin(GeneratorWritePin::FREQ_METER_RESIST, resist.Is(FreqResist::_50Ohm));
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FreqMeter::SetCouple(FreqCouple couple)
{
    CPU::WritePin(GeneratorWritePin::FREQ_METER_COUPLE, couple.Is(FreqCouple::DC));
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FreqMeter::SetFiltr(FreqFiltr filtr)
{
    CPU::WritePin(GeneratorWritePin::FREQ_METER_FILTR, filtr.Is(FreqFiltr::Disable));
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FreqMeter::Update()
{
    if(CPU::ReadPin(GeneratorReadPin::FREQ_METER_DRY))
    {
        uint data = 0;
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
                data += (1 << i);
            }
        }

        Interface::SendFrequency(data);
    }
}

