#include "defines.h"
#include "Log.h"
#include "Interface.h"
#include "Generator/Generator.h"
#include "Generator/Multiplexor.h"
#include "Generator/FPGA.h"
#include "Hardware/CPU.h"
#include "FreqMeter/FreqMeter.h"
#include "Command.h"
#include "structs.h"
#include <string.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static SPI_HandleTypeDef hSPI1 =                                   // Для связи с панелью
{
    SPI1,
    {
        SPI_MODE_SLAVE,                 // Init.Mode
        SPI_DIRECTION_2LINES,           // Init.Direction
        SPI_DATASIZE_8BIT,              // Init.DataSize
        SPI_POLARITY_HIGH,              // Init.CLKPolarity
        SPI_PHASE_2EDGE,                // Init.CLKPhase
        SPI_NSS_SOFT,                   // Init.NSS
        SPI_BAUDRATEPRESCALER_8,        // Init.BaudRatePrescaler
        SPI_FIRSTBIT_MSB,               // Init.FirstBit
        SPI_TIMODE_DISABLED,            // Init.TIMode
        SPI_CRCCALCULATION_DISABLED,    // Init.CRCCalculation
        7                               // InitCRCPolynomial
    },
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, HAL_UNLOCKED, HAL_SPI_STATE_RESET, 0
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static uint8 buffer[LENGTH_SPI_BUFFER];     ///< Буфер для принимаемых команд
uint Interface::freqForSend = MAX_UINT;

                                            //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Interface::Init()
{
    GPIO_InitTypeDef isGPIOA =
    {   //  SCK         MI           MO
        GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7,
        GPIO_MODE_AF_PP,
        GPIO_NOPULL,
        GPIO_SPEED_HIGH,
        GPIO_AF5_SPI1
    };
    HAL_GPIO_Init(GPIOA, &isGPIOA);

    HAL_SPI_Init(&hSPI1);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::ProcessingCommand()
{
    CPU::SetReady();

    uint8 trans[LENGTH_SPI_BUFFER] = {0};

    if (freqForSend != MAX_UINT)
    {
        trans[0] = CommandGenerator::COM_FREQ_MEASURE;
    }
   
    if (HAL_SPI_TransmitReceive(&hSPI1, trans, buffer, LENGTH_SPI_BUFFER, 10) == HAL_OK)
    {
        CPU::SetBusy();

        ProcessCommand();

        if (trans[0] != 0)
        {
            SendToInterface(trans);
        }
    }
    else
    {
        HAL_SPI_DeInit(&hSPI1);
        HAL_SPI_Init(&hSPI1);
    }

    CPU::SetBusy();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::SendToInterface(uint8 *trans)
{
    BitSet32 data;
    data.word = freqForSend;
    for (int i = 0; i < 4; i++)
    {
        trans[1 + i] = data.byte[i];
    }

    CPU::SetReady();

    uint8 recv[LENGTH_SPI_BUFFER];

    HAL_SPI_TransmitReceive(&hSPI1, trans, recv, LENGTH_SPI_BUFFER, 10);

    CPU::SetBusy();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::ProcessCommand()
{
    typedef void(*pFuncInterfaceVV)();

    static const pFuncInterfaceVV commands[CommandPanel::Number] =
    {
        CommandEmpty,
        CommandEnable,          /// ENABLE_CHANNEL
        CommandFormWave,        /// SET_FORM_WAVE
        CommandParameter,       /// SET_FREQUENCY
        CommandParameter,       /// SET_AMPLITUDE
        CommandParameter,       /// SET_OFFSET
        CommandParameter,       /// SET_DURATION
        CommandParameter,       /// SET_DUTYRATIO
        CommandParameter,       /// SET_PHASE
        CommandReset,           /// RUN_RESET
        CommandModeDebug,       /// MODE_DEBUG
        CommandParameter,       /// SET_DELAY
        CommandWriteRegister,   /// WRITE_REGISTER
        CommandReadData         /// READ_DATA
    };
  
    if (buffer[0] < CommandPanel::Number)
    {       
        pFuncInterfaceVV f = commands[buffer[0]];
        f();
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::CommandEnable()
{
    Chan ch = (Chan)buffer[1];
    bool enable = buffer[2] == 1;
    
    __IO uint8 b1 = buffer[1];
    __IO uint8 b2 = buffer[2];

    if((b1 != 0 && b1 != 1) || (b2 != 0 && b2 != 1))
    {
        b1 = b1;
    }
    
    Generator::EnableChannel(ch, enable);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::CommandReadData()
{
}

/*
void Interface::CommandWriteService()
{
    if(freqForSend != 0)
    {
        // Передаём число 5 - количество готовых к передаче байт (1 байт команды + 4 байта данных)
        BitSet32 data;
        data.word = 0;
        data.byte0 = 5;
        WriteToInterface((uint8 *)data.word, 4);

        data.word = freqForSend;
        uint8 trans[5] = {FREQ_MEASURE, data.byte0, data.byte1, data.byte2, data.byte3};
        WriteToInterface(trans, 5);

        freqForSend = 0;
    }
}
*/

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::CommandFormWave()
{
    Chan ch = (Chan)buffer[1];
    WaveForm form = (WaveForm)buffer[2];
    Generator::SetFormWave(ch, form);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::CommandWriteRegister()
{
    Register reg = (Register)buffer[1];

    BitSet32 set;
    for (int i = 0; i < 4; i++)
    {
        set.byte[i] = buffer[i + 2];
    }

    uint value = set.word;

    switch (reg.name)
    {
    case Register::Multiplexor1:
    case Register::Multiplexor2:
        Multiplexor::WriteRegister(reg, value);
        break;

    case Register::OffsetA:
        AD5697::SetOffset(Chan::A, (float)value);
        break;

    case Register::OffsetB:
        AD5697::SetOffset(Chan::B, (float)value);
        break;

    case Register::FreqMeterLevel:
        AD5697::SetFreqLevel((float)value);
        break;

    case Register::FreqMeterHYS:
        AD5697::SetFreqHysteresys((float)value);
        break;

    case Register::FPGA_RG0_Control:
        FPGA::WriteRegister(FPGA::RG0_Control, value);
        break;

    case Register::FPGA_RG1_Freq:
        FPGA::WriteRegister(FPGA::RG1_Freq, value);
        break;

    case Register::FPGA_RG2_Mul:
        FPGA::WriteRegister(FPGA::RG2_Mul, value);
        break;

    case Register::FPGA_RG3_RectA:
        FPGA::WriteRegister(FPGA::RG3_RectA, value);
        break;

    case Register::FPGA_RG4_RectB:
        FPGA::WriteRegister(FPGA::RG4_RectB, value);
        break;

    case Register::FPGA_RG5_PeriodImpulseA:
        FPGA::WriteRegister(FPGA::RG5_PeriodImpulseA, value);
        break;

    case Register::FPGA_RG6_DurationImpulseA:
        FPGA::WriteRegister(FPGA::RG6_DurationImpulseA, value);
        break;

    case Register::FPGA_RG7_PeriodImpulseB:
        FPGA::WriteRegister(FPGA::RG7_PeriodImpulseB, value);
        break;

    case Register::FPGA_RG8_DurationImpulseB:
        FPGA::WriteRegister(FPGA::RG8_DurationImpulseB, value);
        break;

    case Register::FPGA_RG9_FreqMeter:
        FPGA::WriteRegister(FPGA::RG9_FreqMeter, value);
        freqForSend = MAX_UINT;
        break;

    case Register::FPGA_RG10_Offset:
        FPGA::WriteRegister(FPGA::RG10_Offset, value);
        break;

    case Register::Multiplexor3:
        Multiplexor::WriteRegister(reg, value);
        break;

    case Register::FreqMeter_Resist:
        FreqMeter::SetResist((FreqResist)value);
        break;

    case Register::FreqMeter_Couple:
        FreqMeter::SetCouple((FreqCouple)value);
        break;

    case Register::FreqMeter_Filtr:
        FreqMeter::SetFiltr((FreqFiltr)value);
        break;

    case Register::Number:
    default:
        break;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::CommandParameter()
{
    Chan ch = (Chan)buffer[1];
    CommandPanel command = (CommandPanel)buffer[0];
    float value = 0.0f;
    memcpy(&value, &buffer[2], 4);
    Generator::SetParameter(ch, command, value);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::CommandReset()
{
#ifndef MSVC

#define MAIN_PROGRAM_START_ADDRESS  (uint)0x8000000
    typedef void(*pFunction)();
    __disable_irq();
    pFunction JumpToApplication = (pFunction)(*(__IO uint *)(MAIN_PROGRAM_START_ADDRESS + 4));
    __enable_irq();
    JumpToApplication();

#endif
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::CommandModeDebug()
{
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::CommandEmpty()
{
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::SendFrequency(uint value)
{
    freqForSend = value;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
/*
static void SlaveSynchro()
{
    uint8 txByte = SPI_SLAVE_SYNBYTE;
    uint8 rxByte = 0x00;

    do
    {
        HAL_SPI_TransmitReceive(&hSPI1, &txByte, &rxByte, 1, HAL_MAX_DELAY);
    } while(rxByte != SPI_MASTER_SYNBYTE);
}
*/


