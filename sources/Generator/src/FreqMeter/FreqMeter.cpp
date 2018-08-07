#include "defines.h"
#include "FreqMeter.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "Interface/Interface.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FreqMeter::Init()
{
    /*
        PE14 - сопротивление 1 МОм/50 Ом
        PE15 - AC/DC
        PB10 - ФНЧ вкл/откл

        Инициализируются в CPU::InitPins()
    */
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FreqMeter::SetResist(FreqResist resist)
{
    CPU::WritePin(FREQ_METER_RESIST, resist == FreqResist_50Ohm);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FreqMeter::SetCouple(FreqCouple couple)
{
    CPU::WritePin(FREQ_METER_COUPLE, couple == FreqCouple_DC);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FreqMeter::SetFiltr(FreqFiltr filtr)
{
    CPU::WritePin(FREQ_METER_FILTR, filtr == FreqFiltr_Disable);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FreqMeter::Update()
{
    if(CPU::ReadPin(FREQ_METER_DRY))
    {
        uint data = 0;
        for(int i = 31; i >= 0; i--)
        {
            if(CPU::ReadPin(FREQ_METER_DATA))
            {
                data += (1 << i);
            }
            CPU::WritePin(FREQ_METER_CLK, true);
            volatile int j = 0;
            while(j < 25)
            {
                j++;
            }
            CPU::WritePin(FREQ_METER_CLK, false);
            Timer::PauseOnTime(1);
        }

        Interface::SendFrequency(data);
    }
}

