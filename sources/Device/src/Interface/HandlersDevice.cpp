#include "defines.h"
#include "log.h"
#include "structs.h"
#include "Command.h"
#include "Transceiver.h"
#include "HandlersDevice.h"
#include "FDrive/FDriveDevice.h"
#include "FreqMeter/FreqMeter.h"
#include "Generator/Generator_d.h"
#include "InterfaceDevice.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "Hardware/VCP.h"
#include "Hardware/Modules/SPI.h"
#include "Settings/CalibrationSettings.h"


namespace Handlers
{
    void E(SimpleMessage *);

    void SendData(SimpleMessage *);

    void EnableChannel(SimpleMessage *);

    void SetFormWave(SimpleMessage *);

    void SetFrequency(SimpleMessage *);

    void SetAmplitude(SimpleMessage *);

    void SetOffset(SimpleMessage *);

    void SetDuration(SimpleMessage *);

    void SetDutyRatio(SimpleMessage *);

    void SetPhase(SimpleMessage *);

    void RunReset(SimpleMessage *);

    void ModeDebug(SimpleMessage *);

    void SetDelay(SimpleMessage *);

    void WriteRegister(SimpleMessage *);

    void SetManipulation(SimpleMessage *);

    void SetManipulationDuration(SimpleMessage *);

    void SetManipulationPeriod(SimpleMessage *);

    void SetPacketPeriod(SimpleMessage *);

    void SetPacketNumber(SimpleMessage *);

    void SetStartMode(SimpleMessage *);

    void SetPeriod(SimpleMessage *);

    void SetPolarity(SimpleMessage *);

    void LoadFormDDS(SimpleMessage *);

    void Test(SimpleMessage *);

    void SetKoeffCalibration(SimpleMessage *);

    void GetKoeffCalibration(SimpleMessage *);
}



void Handlers::Processing(SimpleMessage *msg)
{
    typedef void(*pFuncInterfaceVpM)(SimpleMessage *);

    static const pFuncInterfaceVpM funcs[Command::Count] =
    {
        /* RequestData               */ Handlers::SendData,
        /* EnableChannel             */ Handlers::EnableChannel,
        /* SetFormWave               */ Handlers::SetFormWave,
        /* SetFrequency              */ Handlers::SetFrequency,
        /* SetAmplitude              */ Handlers::SetAmplitude,
        /* SetOffset                 */ Handlers::SetOffset,
        /* SetDuration               */ Handlers::SetDuration,
        /* SetDutyRatio              */ Handlers::SetDutyRatio,
        /* SetPhase                  */ Handlers::SetPhase,
        /* RunReset                  */ Handlers::RunReset,
        /* ModeDebug                 */ Handlers::ModeDebug,
        /* SetDelay                  */ Handlers::SetDelay,
        /* WriteRegister             */ Handlers::WriteRegister,
        /* SetDurationRise           */ Handlers::E,
        /* SetDurationFall           */ Handlers::E,
        /* SetDurationStady          */ Handlers::E,
        /* SetDutyFactor             */ Handlers::E,
        /* SetManipulation           */ Handlers::SetManipulation,
        /* SetManipulationDuration   */ Handlers::SetManipulationDuration,
        /* SetManipulationPeriod     */ Handlers::SetManipulationPeriod,
        /* SetPacketPeriod           */ Handlers::SetPacketPeriod,
        /* SetPacketNumber           */ Handlers::SetPacketNumber,
        /* SetStartMode              */ Handlers::SetStartMode,
        /* SetPeriod                 */ Handlers::SetPeriod,
        /* SetPolarity               */ Handlers::SetPolarity,
        /* SetManipulationMode       */ Handlers::E,
        /* LoadFromDDS               */ Handlers::LoadFormDDS,
        /* FreqMeasure               */ Handlers::E,
        /* Log                       */ Handlers::E,
        /* FDrive_NumDirsAndFiles    */ FDrive::Handler::Processing,
        /* FDrive_Mount              */ FDrive::Handler::Processing,
        /* FDrive_RequestDir         */ FDrive::Handler::Processing,
        /* FDrive_RequestFile        */ FDrive::Handler::Processing,
        /* Test                      */ Handlers::Test,
        /* SetKoeffCalibration       */ Handlers::SetKoeffCalibration,
        /* GetKoeffCalibration       */ Handlers::GetKoeffCalibration,
        /* FDrive_RequestFileSize    */ FDrive::Handler::Processing,
        /* FDrive_RequestFileString  */ FDrive::Handler::Processing,
        /* FDrive_LoadFromExtStorage */ FDrive::Handler::Processing,
        /* FDrive_GetPictureDDS      */ FDrive::Handler::Processing,
        /* SCPI_RecvData             */ VCP::Handler::Processing
    };

    uint8 com = msg->TakeByte();

    funcs[com](msg);
}


void Handlers::SendData(SimpleMessage *)
{
    CPU::SetBusy();

    if (Interface::GetOutbox().Size() != 0)
    {
        Timer::PauseOnTime(2);

        CPU::SetReady();

        Transceiver::Transmit(Interface::GetOutbox().Front());

        CPU::SetBusy();

        Interface::GetOutbox().Pop();
    }
    else
    {
        Message::RequestData message;

        Timer::PauseOnTime(2);

        CPU::SetReady();

        Transceiver::Transmit(&message);

        CPU::SetBusy();
    }
}


void Handlers::EnableChannel(SimpleMessage *msg)
{
    Chan ch(msg->TakeByte());

    bool enable = (msg->TakeByte() == 1);

    Generator::EnableChannel(ch, enable);
}


void Handlers::SetFormWave(SimpleMessage *msg)
{
    Chan ch(msg->TakeByte());

    Form form(msg->TakeByte()); //-V656

    Generator::SetFormWave(ch, form);
}


static void SetGeneratorParameter(SimpleMessage *msg, void(*func)(Chan::E, ParamValue))
{
    Chan ch(msg->TakeByte());
    ParamValue value(msg->TakeDoubleWord());
    func(ch, value);
}


void Handlers::SetFrequency(SimpleMessage *msg)
{
    SetGeneratorParameter(msg, Generator::SetFrequency);
}


void Handlers::SetPeriod(SimpleMessage *msg)
{
    SetGeneratorParameter(msg, Generator::SetPeriod);
}


void Handlers::SetAmplitude(SimpleMessage *msg)
{
    SetGeneratorParameter(msg, Generator::SetAmplitude);
}


void Handlers::SetOffset(SimpleMessage *msg)
{
    SetGeneratorParameter(msg, Generator::SetOffset);
}


void Handlers::SetDuration(SimpleMessage *msg)
{
    SetGeneratorParameter(msg, Generator::SetDuration);
}


void Handlers::SetDutyRatio(SimpleMessage *msg)
{
    SetGeneratorParameter(msg, Generator::SetDutyRatio);
}


void Handlers::SetPhase(SimpleMessage *msg)
{
    SetGeneratorParameter(msg, Generator::SetPhase);
}


void Handlers::SetDelay(SimpleMessage *msg)
{
    SetGeneratorParameter(msg, Generator::SetDelay);
}


void Handlers::SetManipulationDuration(SimpleMessage *msg)
{
    SetGeneratorParameter(msg, Generator::SetManipulationDuration);
}


void Handlers::SetManipulationPeriod(SimpleMessage *msg)
{
    SetGeneratorParameter(msg, Generator::SetManipulationPeriod);
}


void Handlers::SetPacketPeriod(SimpleMessage *msg)
{
    SetGeneratorParameter(msg, Generator::SetPacketPeriod);
}


void Handlers::SetPacketNumber(SimpleMessage *msg)
{
    SetGeneratorParameter(msg, Generator::SetPacketNumber);
}


void Handlers::SetManipulation(SimpleMessage *msg)
{
    Chan ch(msg->TakeByte());

    AD9952::Manipulation::SetEnabled(ch, msg->TakeByte() != 0);
}


void Handlers::SetStartMode(SimpleMessage *msg)
{
    Chan ch(msg->TakeByte());

    StartMode mode = static_cast<StartMode>(msg->TakeByte());

    FPGA::SetStartMode(ch, mode);
}


void Handlers::SetPolarity(SimpleMessage *message)
{
    Chan ch(message->TakeByte());

    FPGA::SetPolarity(ch, message->TakeByte());
}


void Handlers::LoadFormDDS(SimpleMessage *msg)
{
    Chan ch(msg->TakeByte());

    msg->TakeRemainigData(FPGA::DataDDS(ch));
}


void Handlers::SetKoeffCalibration(SimpleMessage *msg)
{
    Chan ch(msg->TakeByte());

    static int16 * const values[KoeffCal::Count] =
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

    int16 *koeff = values[msg->TakeByte()];

    koeff[ch] = static_cast<int16>(msg->TakeHalfWord());
}


void Handlers::GetKoeffCalibration(SimpleMessage *)
{
}


void Handlers::WriteRegister(SimpleMessage *msg)
{
    Register reg(msg->TakeByte());

    uint64 value = msg->TakeDoubleWord();

    switch (reg)
    {
    case Register::OffsetA:
        AD5697::SetOffset(Chan::A, ParamValue(static_cast<float>(value)));
        break;

    case Register::OffsetB:
        AD5697::SetOffset(Chan::B, ParamValue(static_cast<float>(value)));
        break;

    case Register::FreqMeterLevel:
        AD5697::SetFreqLevel(static_cast<float>(value));
        break;

    case Register::FreqMeterHYS:
        AD5697::SetFreqHysteresys(static_cast<float>(value));
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
        Interface::ResetFreqForSend();
        break;

    case Register::FPGA_RG10_Offset:
        FPGA::WriteRegister(FPGA::RG::_10_Offset, value);
        break;

    case Register::FreqMeter_Resist:
        FreqMeter::SetResist(static_cast<FreqResist::E>(value));
        break;

    case Register::FreqMeter_Couple:
        FreqMeter::SetCouple(static_cast<FreqCouple::E>(value));
        break;

    case Register::FreqMeter_Filtr:
        FreqMeter::SetFiltr(static_cast<FreqFiltr::E>(value));
        break;

    case Register::deleted_Multiplexor1:
    case Register::deleted_Multiplexor2:
    case Register::Count:
    default:
        // Здесь ничего
        break;
    }
}


void Handlers::RunReset(SimpleMessage *)
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
void Handlers::Test(SimpleMessage *)
{

}


void Handlers::ModeDebug(SimpleMessage *)
{
}


void Handlers::E(SimpleMessage *)
{

}
