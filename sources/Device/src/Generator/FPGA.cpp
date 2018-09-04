#include "FPGA.h"
#include "defines.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "Utils/Math.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define MAX_VALUE ((1 << 14) - 1)
#define MIN_VALUE (0)

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FPGA::ModeWorkFPGA FPGA::modeWork = ModeNone;
uint8              FPGA::data[Chan::Number][FPGA_NUM_POINTS * 2];
float              FPGA::amplitude[Chan::Number] = {10.0f, 10.0f};
float              FPGA::offset[Chan::Number] = {5.0f, 5.0f};


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
void FPGA::SetWaveForm(Chan ch, Wave::Form form)
{
    struct StructFunc
    {
        typedef void(*pFuncFpgaVU8)(Chan);
        pFuncFpgaVU8 func;
        StructFunc(pFuncFpgaVU8 f) : func(f) {};
    };
    
    static const StructFunc func[Wave::Form::Number] =
    {
        EmptyFunc,
        CreateRampPlus,
        CreateRampMinus,
        CreateMeander,
    };
    
    func[form].func(ch);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::CreateMeander(Chan)
{
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SetFrequency(Chan ch, float frequency)
{
    WriteControlRegister();
    
    if (modeWork == ModeDDS)
    {
        uint64 N = (uint64)(frequency * 11e3f);
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
void FPGA::WriteControlRegister()
{
    if (modeWork == ModeDDS)
    {
        WriteRegister(RG::_0_Control, BINARY_U8(01111001));
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
void FPGA::EmptyFunc(Chan)
{
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::CreateRampPlus(Chan ch)
{
    modeWork = ModeDDS;

    float step = 2.0f / FPGA_NUM_POINTS;

    float *d = (float *)malloc(FPGA_NUM_POINTS * 4);

    for (int i = 0; i < FPGA_NUM_POINTS; i++)
    {
        d[i] = -1.0f + step * i;
    }

    TransformDataToCode(d, data[ch]);

    free(d);

    SendData();

    WriteControlRegister();

    WriteRegister(RG::_1_Freq, (uint64)(200e3 * 11e3));
    WriteRegister(RG::_2_Amplitude, 0xfffff);
    WriteRegister(RG::_10_Offset, 0);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::CreateRampMinus(Chan ch)
{
    modeWork = ModeDDS;

    float step = 2.0f / FPGA_NUM_POINTS;

    float *d = (float *)malloc(FPGA_NUM_POINTS * 4);

    for (int i = 0; i < FPGA_NUM_POINTS; i++)
    {
        d[i] = 1.0f - step * i;
    }

    TransformDataToCode(d, data[ch]);

    free(d);

    SendData();

    WriteControlRegister();

    WriteRegister(RG::_1_Freq, (uint64)(200e3 * 11e3));
    WriteRegister(RG::_2_Amplitude, 0xfffff);
    WriteRegister(RG::_10_Offset, 0);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SendData()
{
    WriteRegister(RG::_0_Control, 0);
    
    uint8 *pointer = &data[0][0];

    for(int i = 0; i < FPGA_NUM_POINTS * 4; i++)
    {
        WriteByte(*pointer++);
        CPU::WritePin(GeneratorWritePin::FPGA_WR_DATA, true);
        for(int x = 0; x < 10; x++) { }
        CPU::WritePin(GeneratorWritePin::FPGA_WR_DATA, false);
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
        20, // RG2_Amplitude,
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

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::TransformDataToCode(float d[FPGA_NUM_POINTS], uint8 code[FPGA_NUM_POINTS * 2])
{
    int max = 0x1fff;

    for(int i = 0; i < FPGA_NUM_POINTS; i++)
    {
        uint16 c = (uint16)(fabs(d[i]) * max);

        if(Sign(d[i]) == -1)
        {
            SetBit(c, 13);
        }

        code[i] = (uint8)c;
        code[i + FPGA_NUM_POINTS] = (uint8)(c >> 8);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
uint FPGA::OffsetToCode(float off)
{
    uint max = 0x1fff;

    off = (off - 5.0f) / 5.0f;

    int code = ~((int)(off * max)) + 1;

    return (uint)(code & 0x3fff);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SetAmplitude(Chan ch, float ampl)
{
    amplitude[ch] = ampl;

    uint nA = (uint)(amplitude[Chan::A] * 1023 / 10);
    uint nB = (uint)(amplitude[Chan::B] * 1023 / 10);

    WriteRegister(RG::_2_Amplitude, nA + (nB << 10));
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SetOffset(Chan ch, float off)
{
    offset[ch] = off;

    uint nA = OffsetToCode(offset[Chan::A]);
    uint nB = OffsetToCode(offset[Chan::B]);

    WriteRegister(RG::_10_Offset, nA + (nB << 14));
}
