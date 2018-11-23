#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "structs.h"
#include "Command.h"
#include "Transceiver.h"
#include "Handlers.h"
#include "FDrive/FDriveDevice.h"
#include "FreqMeter/FreqMeter.h"
#include "Generator/GeneratorDevice.h"
#include "InterfaceDevice.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "Hardware/Modules/SPI.h"
#include "Settings/CalibrationSettings.h"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Handlers::Processing(Message *msg)
{
    static const struct FuncInterface
    {
        typedef void(*pFuncInterfaceVpM)(Message *);             // Параметром передаём указатель на принятые данные
        pFuncInterfaceVpM func;
        FuncInterface(pFuncInterfaceVpM f) : func(f) {};
    }
    commands[Command::Number] =
    {
        /* RequestData             */ Handlers::SendData,
        /* EnableChannel           */ Handlers::EnableChannel,
        /* SetFormWave             */ Handlers::SetFormWave,
        /* SetFrequency            */ Handlers::SetFrequency,
        /* SetAmplitude            */ Handlers::SetAmplitude,
        /* SetOffset               */ Handlers::SetOffset,
        /* SetDuration             */ Handlers::SetDuration,
        /* SetDutyRatio            */ Handlers::SetDutyRatio,
        /* SetPhase                */ Handlers::SetPhase,
        /* RunReset                */ Handlers::RunReset,
        /* ModeDebug               */ Handlers::ModeDebug,
        /* SetDelay                */ Handlers::SetDelay,
        /* WriteRegister           */ Handlers::WriteRegister,
        /* SetDurationRise         */ Handlers::E,
        /* SetDurationFall         */ Handlers::E,
        /* SetDurationStady        */ Handlers::E,
        /* SetDutyFactor           */ Handlers::E,
        /* SetManipulation         */ Handlers::SetManipulation,
        /* SetManipulationDuration */ Handlers::SetManipulationDuration,
        /* SetManipulationPeriod   */ Handlers::SetManipulationPeriod,
        /* SetPacketPeriod         */ Handlers::SetPacketPeriod,
        /* SetPacketNumber         */ Handlers::SetPacketNumber,
        /* SetStartMode            */ Handlers::SetStartMode,
        /* SetPeriod               */ Handlers::SetPeriod,
        /* SetPolarity             */ Handlers::SetPolarity,
        /* SetManipulationMode     */ Handlers::SetManipulationMode,
        /* LoadFromDDS             */ Handlers::LoadFormDDS,
        /* FreqMeasure             */ Handlers::E,
        /* Log                     */ Handlers::E,
        /* FDrive_NumDirsAndFiles  */ FDrive::Handler::Processing,
        /* FDrive_Mount            */ FDrive::Handler::Processing,
        /* FDrive_RequestDir       */ FDrive::Handler::Processing,
        /* FDrive_RequestFile      */ FDrive::Handler::Processing,
        /* Test                    */ Handlers::Test,
        /* SetKoeffCalibration     */ Handlers::SetKoeffCalibration,
        /* GetKoeffCalibration     */ Handlers::GetKoeffCalibration
    };

    uint com = msg->TakeByte();

    commands[com].func(msg);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Handlers::SendData(Message *)
{
    CPU::SetBusy();

    if (Interface::messages.Size() != 0)
    {
        Timer::PauseOnTime(2);

        CPU::SetReady();

        Transceiver::Transmit(Interface::messages.Front());

        CPU::SetBusy();

        Interface::messages.Pop();
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
void Handlers::EnableChannel(Message *message)
{
    Chan ch = (Chan::E)message->TakeByte();

    bool enable = (message->TakeByte() == 1);

    Generator::EnableChannel(ch, enable);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Handlers::SetFormWave(Message *msg)
{
    Chan ch = (Chan::E)msg->TakeByte();

    Form form = (Form::E)msg->TakeByte();

    Generator::SetFormWave(ch, form);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void SetGeneratorParameter(Message *msg, void(*func)(Chan, float))
{
    Chan ch = (Chan::E)msg->TakeByte();
    float value = msg->TakeFloat();
    func(ch, value);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Handlers::SetFrequency(Message *msg)
{
    SetGeneratorParameter(msg, Generator::SetFrequency);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Handlers::SetPeriod(Message *msg)
{
    SetGeneratorParameter(msg, Generator::SetPeriod);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Handlers::SetAmplitude(Message *msg)
{
    SetGeneratorParameter(msg, Generator::SetAmplitude);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Handlers::SetOffset(Message *msg)
{
    SetGeneratorParameter(msg, Generator::SetOffset);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Handlers::SetDuration(Message *msg)
{
    SetGeneratorParameter(msg, Generator::SetDuration);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Handlers::SetDutyRatio(Message *msg)
{
    SetGeneratorParameter(msg, Generator::SetDutyRatio);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Handlers::SetPhase(Message *msg)
{
    SetGeneratorParameter(msg, Generator::SetPhase);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Handlers::SetDelay(Message *msg)
{
    SetGeneratorParameter(msg, Generator::SetDelay);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Handlers::SetManipulationDuration(Message *msg)
{
    SetGeneratorParameter(msg, Generator::SetManipulationDuration);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Handlers::SetManipulationPeriod(Message *msg)
{
    SetGeneratorParameter(msg, Generator::SetManipulationPeriod);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Handlers::SetPacketPeriod(Message *msg)
{
    SetGeneratorParameter(msg, Generator::SetPacketPeriod);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Handlers::SetPacketNumber(Message *msg)
{
    SetGeneratorParameter(msg, Generator::SetPacketNumber);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Handlers::SetManipulation(Message *msg)
{
    Chan ch = (Chan::E)msg->TakeByte();

    AD9952::Manipulation::SetEnabled(ch, msg->TakeByte() != 0);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Handlers::SetStartMode(Message *msg)
{
    Chan ch = (Chan::E)msg->TakeByte();

    StartMode mode = (StartMode)msg->TakeByte();

    FPGA::SetStartMode(ch, mode);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Handlers::SetPolarity(Message *message)
{
    Chan ch = (Chan::E)message->TakeByte();

    FPGA::SetPolarity(ch, message->TakeByte());
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Handlers::SetManipulationMode(Message *msg)
{
    Chan ch = (Chan::E)msg->TakeByte();

    AD9952::Manipulation::SetType(ch, msg->TakeByte());
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Handlers::LoadFormDDS(Message *message)
{
    Chan ch = (Chan::E)message->TakeByte();

    std::memcpy(FPGA::DataDDS(ch), message->Data(2), FPGA_NUM_POINTS * 2);

    Generator::SetFormWave(ch, Form::DDS);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Handlers::SetKoeffCalibration(Message *msg)
{
    Chan ch = (Chan::E)msg->TakeByte();

    static const struct StructCal
    {
        int16 *pointer;
        StructCal(int16 *p) : pointer(p) {};
    }
    values[KoeffCal::Number] =
    {
        &CAL_AD9952_OFFSET_NEG(Chan::A),
        &CAL_AD9952_OFFSET_ZERO(Chan::A),
        &CAL_AD9952_OFFSET_POS(Chan::A),
        &CAL_AD9952_AMPLITUDE(Chan::A),
        &CAL_DDS_MAX(Chan::A),
        &CAL_DDS_MIN(Chan::A),
        &CAL_DDS_OFFSET(Chan::A),
        &CAL_FREQ_LEVEL_TRIG
    };

    int16 *koeff = values[msg->TakeByte()].pointer;

    koeff[ch] = (int16)msg->TakeHalfWord();

    setCal.Save();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Handlers::GetKoeffCalibration(Message *)
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Handlers::WriteRegister(Message *msg)
{
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
        Interface::freqForSend = MAX_UINT;
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
void Handlers::RunReset(Message *)
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

//---------------------------------------------------------------------------------------------------------------------
void Handlers::Test(Message *)
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Handlers::ModeDebug(Message *)
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Handlers::E(Message *)
{

}
