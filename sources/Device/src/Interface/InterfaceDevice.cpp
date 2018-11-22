#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "log.h"
#include "Message.h"
#include "Transceiver.h"
#include "InterfaceDevice.h"
#include "FDrive/FDriveDevice.h"
#include "Generator/GeneratorDevice.h"
#include "Generator/FPGA.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "Hardware/Modules/SPI.h"
#include "FreqMeter/FreqMeter.h"
#include "Settings/CalibrationSettings.h"
#include "Utils/Array.h"
#include "Utils/Debug.h"
#include "Utils/Queue.h"
#include "Utils/StringUtils.h"
#include "Command.h"
#include "structs.h"
#include <string.h>
#include <cstdlib>
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Ненулевое значение означает, что его следует передать в панель как измеренное частотомером значение
volatile static uint freqForSend = MAX_UINT;
uint  Interface::timeLastReceive = 0;

static const struct FuncInterface
{
    typedef void(*pFuncInterfaceVpM)(Message *);             // Параметром передаём указатель на принятые данные
    pFuncInterfaceVpM func;
    FuncInterface(pFuncInterfaceVpM f) : func(f) {};
}
commands[Command::Number] =
{
/* RequestData             */ Interface::SendData,
/* EnableChannel           */ Interface::EnableChannel,
/* SetFormWave             */ Interface::SetFormWave,
/* SetFrequency            */ Interface::SetFrequency,
/* SetAmplitude            */ Interface::SetAmplitude,
/* SetOffset               */ Interface::SetOffset,
/* SetDuration             */ Interface::SetDuration,
/* SetDutyRatio            */ Interface::SetDutyRatio,
/* SetPhase                */ Interface::SetPhase,
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
/* Test                    */ Interface::Test,
/* SetKoeffCalibration     */ Interface::SetKoeffCalibration,
/* GetKoeffCalibration     */ Interface::GetKoeffCalibration
};


/// Очередь сообщений, ожидающих отправки
Queue messages;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Interface::Update()
{
    CPU::SetReady();

    uint size = 0;

    if (SPI1_::Receive(&size, 4, 10))                                                           // Узнаём размер принимаемого сообщения
    {
        Message first;              // Сюда принимаем первое сообщение
        Message second;             // Сюда принимаем второе сообщение

        uint timeout = size > 100U ? 200U : 10U;

        if (first.AllocateMemory(size))
        {
            if (SPI1_::Receive(first.Data(), first.Size(), timeout))                            // Принимаем данные
            {
                if (SPI1_::Transmit(&size, 4, timeout))                                         // Передаём его размер
                {
                    if (SPI1_::Transmit(first.Data(), first.Size(), timeout))                   // И данные
                    {
                        if (SPI1_::Receive(&size, 4, 10))
                        {
                            if (second.AllocateMemory(size))                                    // Второй раз сообщение будем принимать в этот буфер
                            {
                                if (SPI1_::Receive(second.Data(), second.Size(), timeout))      // Что и делаем
                                {
                                    size = second.Size();

                                    if (SPI1_::Transmit(&size, 4, timeout))
                                    {
                                        if (SPI1_::Transmit(second.Data(), second.Size(), timeout))
                                        {
                                            if (second.IsEquals(&first))                        // Проверяем, совпали ли оба принятых сообщения
                                            {
                                                commands[first.Data()[0]].func(&first);         // И, если совпали, передаём сообщение на выполение
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    CPU::SetBusy();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::SendData(Message *)
{
    CPU::SetBusy();

    if (messages.Size() != 0)
    {
        Timer::PauseOnTime(2);

        CPU::SetReady();

        Transceiver::Transmit(messages.Front());

        CPU::SetBusy();

        messages.Pop();
    }
    else
    {
        Message message(1, Command::RequestData);

        Timer::PauseOnTime(2);

        CPU::SetReady();

        Transceiver::Transmit(&message);

        CPU::SetBusy();
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::EnableChannel(Message *message)
{
    message->TakeByte();

    Chan ch = (Chan::E)message->TakeByte();

    bool enable = (message->TakeByte() == 1);
      
    Generator::EnableChannel(ch, enable);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::SetPolarity(Message *message)
{
    message->TakeByte();

    Chan ch = (Chan::E)message->TakeByte();

    FPGA::SetPolarity(ch, message->TakeByte());
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::LoadFormDDS(Message *message)
{
    message->TakeByte();

    Chan ch = (Chan::E)message->TakeByte();

    std::memcpy(FPGA::DataDDS(ch), message->Data() + 2, FPGA_NUM_POINTS * 2);

    Generator::SetFormWave(ch, Form::DDS);
}

//---------------------------------------------------------------------------------------------------------------------
void Interface::Test(Message *)
{
    std::srand(TIME_MS);
    
    Array array(400);
    
    for(uint i = 0; i < array.Size(); i++)
    {
        array.Data()[i] = (uint8)std::rand();
    }
    
    uint size = 1 + 4 + 4 + array.Size();
    
    // Передаем количество байт
    SPI1_::Transmit(&size, 2, 10);
    
    // А теперь передаем сами байты
    
    uint8 buffer[9] = {Command::Test};
    
    BitSet32 bsSize(array.Size());
    bsSize.WriteToBuffer(buffer + 1);
    
    BitSet32 bsCRC(array.CRC32());
    bsCRC.WriteToBuffer(buffer + 5);
    
    SPI1_::Transmit(buffer, 9, 10);
    
    SPI1_::Transmit(array.Data(), array.Size(), 100);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::SetKoeffCalibration(Message *msg)
{
    //uint8 *data = msg->Data();

    //char string[100];

    //BitSet16 bs(msg->Data() + 3);

    //sprintf(string, "кан %d, koэф %d = %d", data[1], data[2], (int16)bs.halfWord);

    //Console::AddString(string);

    msg->TakeByte();

    Chan ch = (Chan::E)msg->TakeByte();

    static int16 * const values[] =
    {
        &CAL_AD9952_OFFSET_NEG(Chan::A),
        &CAL_AD9952_OFFSET_ZERO(Chan::A),
        &CAL_AD9952_OFFSET_POS(Chan::A),
        &CAL_AD9952_AMPLITUDE(Chan::A),
        &CAL_DDS_MAX(Chan::A),
        &CAL_DDS_MIN(Chan::A)
    };

    int16 *koeff = values[msg->TakeByte()];

    koeff[ch] = (int16)msg->TakeHalfWord();

    setCal.Save();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::GetKoeffCalibration(Message *)
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::SetManipulation(Message *msg)
{
    msg->TakeByte();

    Chan ch = (Chan::E)msg->TakeByte();

    AD9952::Manipulation::SetEnabled(ch, msg->TakeByte() != 0);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::SetManipulationMode(Message *msg)
{
    msg->TakeByte();

    Chan ch = (Chan::E)msg->TakeByte();

    AD9952::Manipulation::SetType(ch, msg->TakeByte());
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::SetStartMode(Message *msg)
{
    msg->TakeByte();

    Chan ch = (Chan::E)msg->TakeByte();

    StartMode mode = (StartMode)msg->TakeByte();

    FPGA::SetStartMode(ch, mode);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::SetFormWave(Message *msg)
{
    msg->TakeByte();

    Chan ch = (Chan::E)msg->TakeByte();

    Form form = (Form::E)msg->TakeByte();

    Generator::SetFormWave(ch, form);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::WriteRegister(Message *msg)
{
    msg->TakeByte();

    Register reg = (Register::E)msg->TakeByte();

    uint64 value = msg->TakeDoubleWord();

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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::ParameterValue(Message *msg)
{
    Command command = (Command::E)msg->TakeByte();

    Chan ch = (Chan::E)msg->TakeByte();
    
    Generator::SetParameter(ch, command, msg->TakeFloat());
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void SetGeneratorParameter(Message *msg, void(*func)(Chan, float))
{
    msg->TakeByte();            // Удаляем команду - нам она не нужна, мы уже знаем функцию func
    Chan ch = (Chan::E)msg->TakeByte();
    float value = msg->TakeFloat();
    func(ch, value);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::SetFrequency(Message *msg)
{
    SetGeneratorParameter(msg, Generator::SetFrequency);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::SetAmplitude(Message *msg)
{
    SetGeneratorParameter(msg, Generator::SetAmplitude);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::SetOffset(Message *msg)
{
    SetGeneratorParameter(msg, Generator::SetOffset);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::SetDuration(Message *msg)
{
    SetGeneratorParameter(msg, Generator::SetDuration);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::SetDutyRatio(Message *msg)
{
    SetGeneratorParameter(msg, Generator::SetDutyRatio);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::SetPhase(Message *msg)
{
    SetGeneratorParameter(msg, Generator::SetPhase);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::RunReset(Message *)
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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::ModeDebug(Message *)
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::Empty(Message *)
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::Send(void *buffer, uint size)
{
    SPI1_::Transmit(&size, 2, 10);

    SPI1_::Transmit(buffer, size, 100);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Interface::Data::Data(uint size, uint8 command) : data(0), size(0)
{
    Init(size);

    if (size)
    {
        data[0] = command;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Interface::Data::~Data()
{
    Release();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Interface::Data::Init(uint _size)
{
    if (data)
    {
        delete data;
    }

    size = _size;

    if (size)
    {
        data = (uint8 *)std::malloc(size);

        return data != 0;
    }

    return false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::Data::Release()
{
    if (data)
    {
        delete data;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Interface::Data::IsEmpty() const
{
    return data == 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint8 *Interface::Data::GetData()
{
    return data;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint Interface::Data::GetSize() const
{
    return size;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::AddMessageInQueue(Message *message)
{
    messages.Push(message);
}
