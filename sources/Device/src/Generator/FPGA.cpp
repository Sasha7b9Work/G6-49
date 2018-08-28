#include "FPGA.h"
#include "defines.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include <string.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define ADDR_BANK           ((uint8*)NOR_MEMORY_ADRESS1)

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
    GPIO_InitTypeDef isGPIO = 
    {
        0,
        GPIO_MODE_AF_PP,
        GPIO_NOPULL,
        GPIO_SPEED_FREQ_VERY_HIGH,
        GPIO_AF12_FSMC
    };

    //              D2           D3          NOE          NWE          NE1             D0           D1
    isGPIO.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_7 | GPIO_PIN_14 | GPIO_PIN_15;
    HAL_GPIO_Init(GPIOD, &isGPIO);

    //               D4          D5            D6           D7
    isGPIO.Pin = GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10;
    HAL_GPIO_Init(GPIOE, &isGPIO);

    //               A0           A1          A2           A3           A4           A5            A6            A7            A8           A9
    isGPIO.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
    HAL_GPIO_Init(GPIOF, &isGPIO);

    //              A10
    isGPIO.Pin = GPIO_PIN_0 | GPIO_PIN_1;
    HAL_GPIO_Init(GPIOG, &isGPIO);

    NOR_HandleTypeDef hNOR =
    {
        FSMC_NORSRAM_DEVICE,
        FSMC_NORSRAM_EXTENDED_DEVICE,
        {
            FSMC_NORSRAM_BANK1,             // NSBank
            FSMC_DATA_ADDRESS_MUX_DISABLE,  // DataAddressMux
            FSMC_MEMORY_TYPE_NOR,           // MemoryTpe
            FSMC_NORSRAM_MEM_BUS_WIDTH_8,   // MemoryDataWidth
            FSMC_BURST_ACCESS_MODE_DISABLE, // BurstAccessMode
            FSMC_WAIT_SIGNAL_POLARITY_LOW,  // WaitSignalPolarity
            FSMC_WRAP_MODE_DISABLE,         // WrapMode
            FSMC_WAIT_TIMING_BEFORE_WS,     // WaitSignalActive
            FSMC_WRITE_OPERATION_ENABLE,    // WriteOperation
            FSMC_WAIT_SIGNAL_DISABLE,       // WaitSignal
            FSMC_EXTENDED_MODE_DISABLE,     // ExtendedMode
            FSMC_ASYNCHRONOUS_WAIT_DISABLE, // AsynchronousWait
            FSMC_WRITE_BURST_DISABLE,       // WriteBurst
            0,                              // ContinuousClock
            0,                              // WriteFifo
            FSMC_PAGE_SIZE_NONE             // PageSize
        },
        HAL_UNLOCKED, HAL_NOR_STATE_RESET
    };

    FSMC_NORSRAM_TimingTypeDef timing =
    {
/* AddressSetupTime      */ 15,     
/* AddressHoldTime       */ 15,     
/* DataSetupTime         */ 255,    
/* BusTurnAroundDuration */ 15,     
/* CLKDivision           */ 16,     
/* DataLatency           */ 17,     
/* AccessMode            */ FSMC_ACCESS_MODE_A
    };

    if (HAL_NOR_Init(&hNOR, &timing, NULL) != HAL_OK)
    {
        ERROR_HANDLER();
    }

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
    WriteRegister(RG::_0_Control, 1);
    uint8 *address = ADDR_BANK;
    
    uint16 *data = dataA;
    for(int i = 0; i < FPGA_NUM_POINTS; i++)
    {
        *address++ = (uint8)*data;
        data++;
    }
    data = dataA;
    for(int i = 0; i < FPGA_NUM_POINTS; i++)
    {
        *address++ = (uint8)((*data) >> 8);
        data++;
    }
    data = dataB;
    for(int i = 0; i < FPGA_NUM_POINTS; i++)
    {
        *address++ = (uint8)*data;
        data++;
    }
    data = dataB;
    for(int i = 0; i < FPGA_NUM_POINTS; i++)
    {
        *address++ = (uint8)((*data) >> 8);
        data++;
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
