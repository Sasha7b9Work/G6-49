#include "defines.h"
#include "Log.h"
#include "Interface.h"
#include "Generator/Generator.h"
#include "Generator/Multiplexor.h"
#include "Generator/FPGA.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "FreqMeter/FreqMeter.h"
#include "Utils/Debug.h"
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
        SPI_BAUDRATEPRESCALER_32,      // Init.BaudRatePrescaler
        SPI_FIRSTBIT_MSB,               // Init.FirstBit
        SPI_TIMODE_DISABLED,            // Init.TIMode
        SPI_CRCCALCULATION_DISABLED,    // Init.CRCCalculation
        7                               // InitCRCPolynomial
    },
    0, 0, 0, 0, 0, 0, 
    0, 
    0, 
    0, 0, HAL_UNLOCKED, HAL_SPI_STATE_RESET, 0
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static uint8 buffer[LENGTH_SPI_BUFFER];         ///< Буфер для принимаемых команд
static uint8 trans[LENGTH_SPI_BUFFER];     
uint  Interface::freqForSend = MAX_UINT;
uint  Interface::timeLastReceive = 0;

static const struct FuncInterface
{
    typedef void(*pFuncInterfaceVV)();
    pFuncInterfaceVV func;
    FuncInterface(pFuncInterfaceVV f) : func(f) {};
}
commands[CommandPanel::Number] =
{
    Interface::CommandEmpty,
    Interface::CommandEnable,                   /// ENABLE_CHANNEL
    Interface::CommandFormWave,                 /// SET_FORM_WAVE
    Interface::CommandParameter,                /// SET_FREQUENCY
    Interface::CommandParameter,                /// SET_AMPLITUDE
    Interface::CommandParameter,                /// SET_OFFSET
    Interface::CommandParameter,                /// SET_DURATION
    Interface::CommandParameter,                /// SET_DUTYRATIO
    Interface::CommandParameter,                /// SET_PHASE
    Interface::CommandReset,                    /// RUN_RESET
    Interface::CommandModeDebug,                /// MODE_DEBUG
    Interface::CommandParameter,                /// SET_DELAY
    Interface::CommandWriteRegister,            /// WRITE_REGISTER
    Interface::CommandReadData,                 /// READ_DATA
    Interface::CommandEmpty,                    /// SET_DEPTH_MODULATION
    Interface::CommandEmpty,                    /// SET_POLARITY
    Interface::CommandEmpty,                    /// SET_DURATION_RISE
    Interface::CommandEmpty,                    /// SET_DURATION_FALL
    Interface::CommandEmpty,                    /// SET_DURATION_STADY
    Interface::CommandEmpty,                    /// SET_DUTY_FACTOR
    Interface::CommandSetWave,                  /// CREATE_HAND_WAVE
    Interface::CommandSetWave,                  /// SET_HAND_WAVE
    Interface::CommandSetRampForSine,           /// SetRampForSine
    Interface::CommandSetDurationForRampSine,   /// SetRampSineDuration
    Interface::CommandSetAmplitudeRampForSine   /// SetRampSineAmplitude
};

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

    HAL_NVIC_SetPriority(SPI1_IRQn, 1, 0);

    HAL_SPI_Init(&hSPI1);

    HAL_NVIC_EnableIRQ(SPI1_IRQn);

    HAL_SPI_TransmitReceive_IT(&hSPI1, trans, buffer, LENGTH_SPI_BUFFER);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
SPI_HandleTypeDef *Interface::HandleSPI()
{
    return &hSPI1;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::Update()
{ 
    /*
    if(TIME_MS - timeLastReceive < 1000)
    {
        HAL_SPI_DeInit(&hSPI1);
        HAL_SPI_Init(&hSPI1);
        HAL_NVIC_EnableIRQ(SPI1_IRQn);
        HAL_SPI_TransmitReceive_IT(&hSPI1, trs, buffer, LENGTH_SPI_BUFFER);
        CPU::SetReady();
        timeLastReceive = TIME_MS;
    }
    */
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::CommandEnable()
{
    Chan ch = (Chan::E)buffer[1];
    bool enable = buffer[2] == 1;
      
    Generator::EnableChannel(ch, enable);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::CommandReadData()
{
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::CommandSetRampForSine()
{
    Chan ch = (Chan::E)buffer[1];
    bool enabled = GetFloat(&buffer[2]) != 0;
    //AD9952::Ramp::SetEnabled(ch, enabled);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::CommandSetDurationForRampSine()
{
    Chan ch = (Chan::E)buffer[1];
    float duration = (float)((int)(GetFloat(&buffer[2]) + 0.5f));
    //AD9952::Ramp::SetDuration(ch, duration);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::CommandSetAmplitudeRampForSine()
{
    Chan ch = (Chan::E)buffer[1];
    float amplitude = (float)((int)(GetFloat(&buffer[2]) + 0.5f));
    //AD9952::Ramp::SetAmplitude(ch, amplitude);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::CommandFormWave()
{
    Chan ch = (Chan::E)buffer[1];
    Form form = (Form::E)buffer[2];
    Generator::SetFormWave(ch, form);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::CommandWriteRegister()
{
    Register reg = (Register::E)buffer[1];

    BitSet64 set;
    for (int i = 0; i < 8; i++)
    {
        set.byte[i] = buffer[i + 2];
    }

    uint64 value = set.dword;

    switch (reg.value)
    {
    case Register::Multiplexor1:
    case Register::Multiplexor2:
        Multiplexor::WriteRegister(reg, (uint)value);
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
        FPGA::WriteRegister(FPGA::RG::_0_Control, value);
        break;

    case Register::FPGA_RG1_Freq:
        FPGA::WriteRegister(FPGA::RG::_1_Freq, value);
        break;

    case Register::FPGA_RG2_Amplitude:
        FPGA::WriteRegister(FPGA::RG::_2_Amplitude, value);
        break;

    case Register::FPGA_RG3_RectA:
        FPGA::WriteRegister(FPGA::RG::_3_RectA, value);
        break;

    case Register::FPGA_RG4_RectB:
        FPGA::WriteRegister(FPGA::RG::_4_RectB, value);
        break;

    case Register::FPGA_RG5_PeriodImpulseA:
        FPGA::WriteRegister(FPGA::RG::_5_PeriodImpulseA, value);
        break;

    case Register::FPGA_RG6_DurationImpulseA:
        FPGA::WriteRegister(FPGA::RG::_6_DurationImpulseA, value);
        break;

    case Register::FPGA_RG7_PeriodImpulseB:
        FPGA::WriteRegister(FPGA::RG::_7_PeriodImpulseB, value);
        break;

    case Register::FPGA_RG8_DurationImpulseB:
        FPGA::WriteRegister(FPGA::RG::_8_DurationImpulseB, value);
        break;

    case Register::FPGA_RG9_FreqMeter:
        FPGA::WriteRegister(FPGA::RG::_9_FreqMeter, value);
        freqForSend = MAX_UINT;
        break;

    case Register::FPGA_RG10_Offset:
        FPGA::WriteRegister(FPGA::RG::_10_Offset, value);
        break;

    case Register::Multiplexor3:
        Multiplexor::WriteRegister(reg, (uint)value);
        break;

    case Register::FreqMeter_Resist:
        FreqMeter::SetResist((FreqResist::E)value);
        break;

    case Register::FreqMeter_Couple:
        FreqMeter::SetCouple((FreqCouple::E)value);
        break;

    case Register::FreqMeter_Filtr:
        FreqMeter::SetFiltr((FreqFiltr::E)value);
        break;

    case Register::Number:
    default:
        break;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
float Interface::GetFloat(uint8 buf[4])
{
    float result = 0.0f;
    memcpy(&result, buf, 4);
    return result;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::CommandParameter()
{
    Chan ch = (Chan::E)buffer[1];
    CommandPanel command = (CommandPanel::E)buffer[0];
    Generator::SetParameter(ch, command, GetFloat(&buffer[2]));
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::CommandCreateWave()
{
    Chan ch = (Chan::E)buffer[1];
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
void Interface::ReceiveCallback()
{
    if (buffer[0] < CommandPanel::Number)
    {
        commands[buffer[0]].func();
        if(freqForSend != MAX_UINT)
        {
            trans[0] = CommandGenerator::COM_FREQ_MEASURE;
            INIT_BIT_SET_32(bs, freqForSend);
            for(int i = 0; i < 4; i++)
            {
                trans[i + 1] = bs.byte[i];
            }
        }
        else
        {
            trans[0] = 0;
        }
    }
    timeLastReceive = TIME_MS;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::CommandSetWave()
{
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *)
{
    CPU::SetBusy();
    Interface::ReceiveCallback();
    memset(buffer, 0, LENGTH_SPI_BUFFER);
    HAL_SPI_TransmitReceive_IT(&hSPI1, trans, buffer, LENGTH_SPI_BUFFER);
    CPU::SetReady();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *)
{
    HAL_SPI_Init(&hSPI1);
    HAL_NVIC_EnableIRQ(SPI1_IRQn);
    HAL_SPI_TransmitReceive_IT(&hSPI1, trans, buffer, LENGTH_SPI_BUFFER);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------

