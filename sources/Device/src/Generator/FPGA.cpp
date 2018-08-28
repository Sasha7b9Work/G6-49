#include "FPGA.h"
#include "defines.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include <string.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define MAX_VALUE ((1 << 14) - 1)
#define MIN_VALUE (0)

static float dur[2] = {0.0f, 0.0f};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FPGA::ModeWorkFPGA FPGA::modeWork = ModeNone;
uint16             FPGA::dataA[FPGA_NUM_POINTS];
uint16             FPGA::dataB[FPGA_NUM_POINTS];


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FPGA::Init()
{
    // Настраиваем выходы для записи в регистры ПЛИС

    CPU::WritePin(GeneratorWritePin::FPGA_WR_RG, false);
    CPU::WritePin(GeneratorWritePin::FPGA_CLK_RG, false);
    CPU::WritePin(GeneratorWritePin::FPGA_DT_RG, false);
    CPU::WritePin(GeneratorWritePin::FPGA_A0_RG, false);
    CPU::WritePin(GeneratorWritePin::FPGA_A1_RG, false);
    CPU::WritePin(GeneratorWritePin::FPGA_A2_RG, false);
    CPU::WritePin(GeneratorWritePin::FPGA_A3_RG, false);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SetWaveForm(Wave::Form form)
{
    typedef void(*pFuncFpgaVV)();
    
    static const pFuncFpgaVV func[Wave::Form::Number] =
    {
        CreateSine,
        //CreateCosine,
        //CreateMeander,
        CreateRampPlus,
        CreateRampMinus,
        //CreateTriangle,
        //CreateTrapeze,
        //CreateImpulse,
        //CreateExponentePlus,
        //CreateExponenteMinus,
        //CreateNoise,
        //CreateFree
    };
    
    func[form]();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SetFrequency(Chan ch, float frequency)
{
    WriteControlRegister();
    
    if (modeWork == ModeDDS)
    {
        uint N = (uint)(frequency * (1 << 30) / 1e8f + 0.5f);
        WriteRegister(RG::_1_Freq, N);
    }
    else if(modeWork == ModeImpulse || modeWork == ModeImpulse2)
    {
        if (ch == Chan::B && ModeImpulse2)
        {
            modeWork = ModeImpulse;
            WriteControlRegister();
        }
        uint N = (uint)(1e8f / frequency + 0.5f);
        WriteRegister((uint8)(ch == Chan::A ? RG::_5_PeriodImpulseA : RG::_7_PeriodImpulseB), N);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SetDuration(Chan ch, float duration)
{
    dur[ch] = duration;

    uint N = (uint)(duration / 1e-8f + 0.5f);

    WriteRegister(RegisterForDuration(ch), N);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SetDelay(float delay)
{
    modeWork = ModeImpulse2;

    WriteControlRegister();

    SetDuration(Chan::B, dur[1]);

    uint N = (uint)(delay / 1e-8f + 0.5f);
    
    WriteRegister(RG::_6_DurationImpulseA, N);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::WriteControlRegister()
{
    if (modeWork == ModeDDS)
    {
        WriteRegister(RG::_0_Control, 0);
    }
    else if (modeWork == ModeImpulse)
    {
        WriteRegister(RG::_0_Control, 2);
    }
    else if (modeWork == ModeImpulse2)
    {
        WriteRegister(RG::_0_Control, 4);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::CreateSine()
{
    memset(dataA, 0, FPGA_NUM_POINTS * 2);
    memset(dataB, 0, FPGA_NUM_POINTS * 2);
    SendData();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::CreateImpulse()
{
    modeWork = ModeImpulse;

    WriteControlRegister();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::CreateRampPlus()
{
    modeWork = ModeDDS;

    float step = (float)(MAX_VALUE - MIN_VALUE) / FPGA_NUM_POINTS;

    for (int i = 0; i < FPGA_NUM_POINTS; i++)
    {
        dataA[i] = (uint16)(step * i);
        dataB[i] = (uint16)(MAX_VALUE - dataA[i]);
    }

    SendData();

    WriteControlRegister();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::CreateRampMinus()
{
    modeWork = ModeDDS;

    float step = (float)(MAX_VALUE - MIN_VALUE) / FPGA_NUM_POINTS;

    for (int i = 0; i < FPGA_NUM_POINTS; i++)
    {
        dataA[i] = (uint16)(step * i);
        dataB[i] = (uint16)(MAX_VALUE - dataA[i]);
    }

    SendData();

    WriteControlRegister();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::CreateFree()
{

}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::CreateNoise()
{

}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::CreateCosine()
{

}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::CreateMeander()
{

}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::CreateTrapeze()
{

}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::CreateTriangle()
{
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::CreateExponentePlus()
{

}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::CreateExponenteMinus()
{
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SendData()
{
    WriteRegister(RG::_0_Control, 0);
    
    for(int i = 0; i < FPGA_NUM_POINTS; i++)
    {
        WriteByte((uint8)dataA[i]);
        CPU::WritePin(GeneratorWritePin::FPGA_WR_RG, true);
        Timer::PauseOnTicks(10);
        CPU::WritePin(GeneratorWritePin::FPGA_WR_RG, false);
    }

    WriteRegister(RG::_0_Control, 1);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::WriteByte(uint8 byte)
{
    static const GeneratorWritePin pins[8] = 
    {
        GeneratorWritePin::D0,
        GeneratorWritePin::D1,
        GeneratorWritePin::D2,
        GeneratorWritePin::D3,
        GeneratorWritePin::D4,
        GeneratorWritePin::D5,
        GeneratorWritePin::D6,
        GeneratorWritePin::D7
    };

    for(int i = 0; i < 8; i++)
    {
        CPU::WritePin(pins[i], GetBit(byte, i) == 1);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::WriteRegister(uint8 reg, uint64 value)
{
    int numBits[Register::Number] =
    {
        8,  // RG0_Control,
        40, // RG1_Freq,
        20, // RG2_Mul,
        28, // RG3_RectA,
        28, // RG4_RectB,
        32, // RG5_PeriodImpulseA,
        32, // RG6_DurationImpulseA,
        32, // RG7_PeriodImpulseB,
        32, // RG8_DurationImpulseB,
        13, // RG9_FreqMeter
        28  // RG10_Offset
    };

    WriteAddress(reg);

    for (int bit = numBits[reg] - 1; bit >= 0; bit--)
    {
        CPU::WritePin(GeneratorWritePin::FPGA_DT_RG, GetBit(value, bit) == 1);  // Устанавливаем или сбрасываем соответствующий бит
        CPU::WritePin(GeneratorWritePin::FPGA_CLK_RG, true);                    // И записываем его в ПЛИС
        CPU::WritePin(GeneratorWritePin::FPGA_CLK_RG, false);
    }

    CPU::WritePin(GeneratorWritePin::FPGA_WR_RG, true);                         // Теперь переписываем данные из сдвиговоого регистра в FPGA
    CPU::WritePin(GeneratorWritePin::FPGA_WR_RG, false);
    Timer::PauseOnTime(10);                              // Ждём 10 миллисекунд, пока данные перепишутся в FPGA
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::WriteAddress(uint8 reg)
{
    static const GeneratorWritePin pins[] = 
    {
        GeneratorWritePin::FPGA_A0_RG, 
        GeneratorWritePin::FPGA_A1_RG, 
        GeneratorWritePin::FPGA_A2_RG, 
        GeneratorWritePin::FPGA_A3_RG
    };

    for (int i = 0; i < 4; i++)
    {
        CPU::WritePin(pins[i], GetBit(reg, i) == 1);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
uint8 FPGA::RegisterForDuration(Chan ch)
{
    return ch == Chan::A ? (uint8)RG::_6_DurationImpulseA : (uint8)RG::_8_DurationImpulseB;
}
