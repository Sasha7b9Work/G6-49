#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Log.h"
#include "InterfaceDevice.h"
#include "FDrive/FDriveDevice.h"
#include "Utils/Console.h"
#include "Generator/GeneratorDevice.h"
#include "Generator/FPGA.h"
#include "Hardware/CPU/CPU.h"
#include "Hardware/Timer.h"
#include "FreqMeter/FreqMeter.h"
#include "Utils/Array.h"
#include "Utils/Debug.h"
#include "Utils/StringUtils.h"
#include "Command.h"
#include "structs.h"
#include <string.h>
#include <stdlib.h>
#endif


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint8 *Interface::recv = 0;                         
/// Ненулевое значение означает, что его следует передать в панель как измеренное частотомером значение
static uint freqForSend = MAX_UINT;
uint  Interface::timeLastReceive = 0;

static const struct FuncInterface
{
    typedef void(*pFuncInterfaceVV)();
    pFuncInterfaceVV func;
    FuncInterface(pFuncInterfaceVV f) : func(f) {};
}
commands[Command::Number] =
{
/* RequestData             */ Interface::Empty,
/* EnableChannel           */ Interface::EnableChannel,
/* SetFormWave             */ Interface::SetFormWave,
/* SetFrequency            */ Interface::ParameterValue,
/* SetAmplitude            */ Interface::ParameterValue,
/* SetOffset               */ Interface::ParameterValue,
/* SetDuration             */ Interface::ParameterValue,
/* SetDutyRatio            */ Interface::ParameterValue,
/* SetPhase                */ Interface::ParameterValue,
/* RunReset                */ Interface::RunReset,
/* ModeDebug               */ Interface::ModeDebug,
/* SetDelay                */ Interface::ParameterValue,
/* WriteRegister           */ Interface::WriteRegister,
/* SetDurationRise         */ Interface::Empty,
/* SetDurationFall         */ Interface::Empty,
/* SetDurationStady        */ Interface::Empty,
/* SetDutyFactor           */ Interface::Empty,
/* SetManipulation         */ Interface::SetManipulation,
/* SetManipulationDuration */ Interface::ParameterValue,
/* SetManipulationPeriod   */ Interface::ParameterValue,
/* SetPacketPeriod         */ Interface::ParameterValue,
/* SetPacketNumber         */ Interface::ParameterValue,
/* SetStartMode            */ Interface::SetStartMode,
/* SetPeriod               */ Interface::ParameterValue,
/* SetPolarity             */ Interface::SetPolarity,
/* SetManipulationMode     */ Interface::SetManipulationMode,
/* LoadFromDDS             */ Interface::LoadFormDDS,
/* FreqMeasure             */ Interface::Empty,
/* Log                     */ Interface::Empty,
/* FDrive_NumDirsAndFiles  */ Interface::Empty,
/* FDrive_Mount            */ Interface::Empty,
/* FDrive_RequestDir       */ Interface::Empty,
/* FDrive_RequestFile      */ Interface::Empty,
/* Test                    */ Interface::Test
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Послать имеющиеся данные в главный процессор
static void SendData();


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Interface::Init()
{
    free(recv);
    recv = (uint8 *)malloc(2);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::EnableChannel()
{
    Chan ch = (Chan::E)recv[1];
    bool enable = recv[2] == 1;
      
    Generator::EnableChannel(ch, enable);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::SetPolarity()
{
    Chan ch = (Chan::E)recv[1];
    FPGA::SetPolarity(ch, recv[2]);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::LoadFormDDS()
{
    Chan ch = (Chan::E)recv[1];

    uint8 temp[2];

    // Сюда примем количество байт
    CPU::SPI1_::Receive(temp, 2);

    CPU::SPI1_::Receive(FPGA::DataDDS(ch), FPGA_NUM_POINTS * 2);

    Generator::SetFormWave(ch, Form::DDS);
}

//---------------------------------------------------------------------------------------------------------------------
void Interface::Test()
{
    srand(TIME_MS);
    
    Array array(400);
    
    for(int i = 0; i < array.Size(); i++)
    {
        array.Data()[i] = (uint8)rand();
    }
    
    uint size = 1 + 4 + 4 + array.Size();
    
    // Передаем количество байт
    CPU::SPI1_::Transmit(&size, 2);
    
    // А теперь передаем сами байты
    
    uint8 buffer[9] = {Command::Test};
    
    BitSet32 bsSize(array.Size());
    bsSize.WriteToBuffer(buffer + 1);
    
    BitSet32 bsCRC(array.CRC32());
    bsCRC.WriteToBuffer(buffer + 5);
    
    CPU::SPI1_::Transmit(buffer, 9);
    
    CPU::SPI1_::Transmit(array.Data(), array.Size());
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::SetManipulation()
{
    Chan ch = (Chan::E)recv[1];
    AD9952::Manipulation::SetEnabled(ch, recv[2] != 0);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::SetManipulationMode()
{
    Chan ch = (Chan::E)recv[1];
    AD9952::Manipulation::SetType(ch, recv[2]);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::SetStartMode()
{
    Chan ch = (Chan::E)recv[1];
    StartMode mode = (StartMode)recv[2];
    FPGA::SetStartMode(ch, mode);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::SetFormWave()
{
    Chan ch = (Chan::E)recv[1];
    Form form = (Form::E)recv[2];
    Generator::SetFormWave(ch, form);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::WriteRegister()
{
    Register reg = (Register::E)recv[1];

    BitSet64 set;
    for (int i = 0; i < 8; i++)
    {
        set.byte[i] = recv[i + 2];
    }

    uint64 value = set.dword;

    switch (reg.value)
    {
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
    case Register::Multiplexor1:
    case Register::Multiplexor2:
    default:
        break;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::ParameterValue()
{
    Chan ch = (Chan::E)recv[1];
    Command command = (Command::E)recv[0];
    Generator::SetParameter(ch, command, Buffer2Float(recv + 2));
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::CreateWave()
{
    Chan ch = (Chan::E)recv[1];
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::RunReset()
{
#ifndef WIN32

#define MAIN_PROGRAM_START_ADDRESS  (uint)0x8000000
    typedef void(*pFunction)();
    __disable_irq();
    pFunction JumpToApplication = (pFunction)(*(__IO uint *)(MAIN_PROGRAM_START_ADDRESS + 4));
    __enable_irq();
    JumpToApplication();

#endif
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::ModeDebug()
{
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::Empty()
{
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::SendFrequency(uint value)
{
    freqForSend = value;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::Update()
{
    uint16 numBytes = 0;
    CPU::SPI1_::Receive(&numBytes, 2);                  // Узнаём количество байт, которые хочет передать панель

    if(numBytes)
    {
        ResizeRecieveBuffer(numBytes);                  // Устанавливаем требуемый размер буфера

        CPU::SPI1_::Receive(recv, numBytes);            // Принимаем данные

        if(recv[0] < Command::Number)
        {
            if(recv[0] == Command::RequestData)         // Если запрос на передачу данных
            {
                SendData();                             // То засыалем данные
            }
            else
            {
                commands[recv[0]].func();               // Иначе обрабатываем команду
            }
        }
        else
        {
//            Console::AddFormatString("Принята неправильная команда %d", recv[0]);
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void SendData()
{
    bool isSending = false;

    if(freqForSend != MAX_UINT)
    {
        /*
        uint8 buffer[5];
        buffer[0] = Command::FreqMeasure;

        BitSet32 bs(freqForSend);

        bs.WriteToBuffer(buffer + 1);

        Interface::Send(buffer, 5);

        freqForSend = MAX_UINT;

        isSending = true;
        */
    }

    if(Console::ExistString())
    {
        /*
        char buffer[LENGTH_SPI_BUFFER] = {Command::Log};
        Console::GetString(buffer + 1);
        Interface::Send(buffer, LENGTH_SPI_BUFFER);

        isSending = true;
        */
    }

    if(FDrive::NumBytesForSend())
    {
        /*
        Console::AddString(__FUNCTION__);
        Console::AddInt(__LINE__);
        uint numBytes = FDrive::NumBytesForSend();
        uint8 *buffer = (uint8 *)malloc(numBytes);
        Interface::Send(FDrive::GetDataForSend(buffer), numBytes);
        free(buffer);

        isSending = true;
        */
    }

    if(!isSending)
    {
        uint16 numBytes = 0;
        CPU::SPI1_::Transmit(&numBytes, 2);
    }
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::Send(void *buffer, uint size)
{
    CPU::SPI1_::Transmit(&size, 2);

    CPU::SPI1_::Transmit(buffer, size);
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::ResizeRecieveBuffer(uint16 size)
{
    free(recv);

    recv = (uint8 *)malloc(size);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
Interface::Data::Data(uint size, uint8 command) : data(0)
{
    Init(size);

    if (size)
    {
        data[0] = command;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
Interface::Data::~Data()
{
    Release();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Interface::Data::Init(uint _size)
{
    if (data)
    {
        delete data;
    }

    size = _size;

    if (size)
    {
        data = (uint8 *)malloc(size);

        return data != 0;
    }

    return false;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::Data::Release()
{
    if (data)
    {
        delete data;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Interface::Data::IsEmpty() const
{
    return data == 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
uint8 *Interface::Data::GetData()
{
    return data;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
uint Interface::Data::GetSize() const
{
    return size;
}
