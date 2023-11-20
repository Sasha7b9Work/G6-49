// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Messages.h"
#include "FreqMeter_d.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
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

    SetCouple(FreqCouple::DC);
}


void DFreqMeter::SetResist(FreqResist resist)
{
    HAL_PIO::Write(WR_FREQ_METER_RESIST, resist.Is(FreqResist::_50Ohm));
}


void DFreqMeter::SetCouple(FreqCouple couple)
{
    HAL_PIO::Write(WR_FREQ_METER_COUPLE, couple.Is(FreqCouple::DC));
}


void DFreqMeter::SetFiltr(FreqFiltr filtr)
{
    HAL_PIO::Write(WR_FREQ_METER_FILTR, filtr.Is(FreqFiltr::Enable));
}


void DFreqMeter::Update()
{
    if(HAL_PIO::Read(RD_FREQ_METER_DRY))
    {
        uint frequency = 0;
        for(int i = 30; i >= 0; i--)
        {
            HAL_PIO::Set(WR_FREQ_METER_CLK);
            volatile int j = 0;
            while(j < 25)
            {
                j++;
            }
            HAL_PIO::Reset(WR_FREQ_METER_CLK);
            HAL_TIM::Delay(1);
            if (HAL_PIO::Read(RD_FREQ_METER_DATA))
            {
                frequency += (1 << i);
            }
        }

        Message::FreqMeter::Measure(frequency).Transmit();
    }
}

