#include "defines.h"
#include "log.h"
#include "structs.h"
#include "Command.h"
#include "Transceiver.h"
#include "Handlers_d.h"
#include "FDrive/FDrive_d.h"
#include "FreqMeter/FreqMeter_d.h"
#include "Generator/Generator_d.h"
#include "Interface_d.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "Hardware/VCP_d.h"
#include "Hardware/HAL/HAL.h"
#include "Settings/CalibrationSettings.h"


void DHandlers::Processing(SimpleMessage *msg)
{
    typedef void(*pFuncInterfaceVpM)(SimpleMessage *);

    static const pFuncInterfaceVpM funcs[Command::Count] =
    {
        /* RequestData               */ DHandlers::SendData,
        /* EnableChannel             */ DHandlers::EnableChannel,
        /* SetFormWave               */ DHandlers::SetFormWave,
        /* SetFrequency              */ DHandlers::SetFrequency,
        /* SetAmplitude              */ DHandlers::SetAmplitude,
        /* SetOffset                 */ DHandlers::SetOffset,
        /* SetDuration               */ DHandlers::SetDuration,
        /* SetDutyRatio              */ DHandlers::SetDutyRatio,
        /* SetPhase                  */ DHandlers::SetPhase,
        /* RunReset                  */ DHandlers::RunReset,
        /* ModeDebug                 */ DHandlers::ModeDebug,
        /* SetDelay                  */ DHandlers::SetDelay,
        /* WriteRegister             */ DHandlers::WriteRegister,
        /* SetDurationRise           */ DHandlers::E,
        /* SetDurationFall           */ DHandlers::E,
        /* SetDurationStady          */ DHandlers::E,
        /* SetDutyFactor             */ DHandlers::E,
        /* SetManipulation           */ DHandlers::SetManipulation,
        /* SetManipulationDuration   */ DHandlers::SetManipulationDuration,
        /* SetManipulationPeriod     */ DHandlers::SetManipulationPeriod,
        /* SetPacketPeriod           */ DHandlers::SetPacketPeriod,
        /* SetPacketNumber           */ DHandlers::SetPacketNumber,
        /* SetStartMode              */ DHandlers::SetStartMode,
        /* SetPeriod                 */ DHandlers::SetPeriod,
        /* SetPolarity               */ DHandlers::SetPolarity,
        /* SetManipulationMode       */ DHandlers::E,
        /* LoadFromDDS               */ DHandlers::LoadFormDDS,
        /* FreqMeasure               */ DHandlers::E,
        /* Log                       */ DHandlers::E,
        /* FDrive_NumDirsAndFiles    */ DDrive::Handler::Processing,
        /* FDrive_Mount              */ DDrive::Handler::Processing,
        /* FDrive_RequestDir         */ DDrive::Handler::Processing,
        /* FDrive_RequestFile        */ DDrive::Handler::Processing,
        /* Test                      */ DHandlers::Test,
        /* SetKoeffCalibration       */ DHandlers::_SetKoeffCalibration,
        /* GetKoeffCalibration       */ DHandlers::_GetKoeffCalibration,
        /* FDrive_RequestFileSize    */ DDrive::Handler::Processing,
        /* FDrive_RequestFileString  */ DDrive::Handler::Processing,
        /* FDrive_LoadFromExtStorage */ DDrive::Handler::Processing,
        /* FDrive_GetPictureDDS      */ DDrive::Handler::Processing,
        /* SCPI_RecvData             */ DVCP::Handler::Processing,
        /* PortCPU                   */ DHandlers::SetPin
    };

    uint8 com = msg->TakeByte();

    funcs[com](msg);
}


void DHandlers::SendData(SimpleMessage *)
{
    CPU::SetBusy();

    if (DInterface::GetOutbox().Size() != 0)
    {
        HAL_TIM::Delay(2);

        CPU::SetReady();

        Transceiver::Transmit(DInterface::GetOutbox().Front());

        CPU::SetBusy();

        DInterface::GetOutbox().Pop();
    }
    else
    {
        Message::RequestData message;

        HAL_TIM::Delay(2);

        CPU::SetReady();

        Transceiver::Transmit(&message);

        CPU::SetBusy();
    }
}


void DHandlers::EnableChannel(SimpleMessage *msg)
{
    Chan ch(msg->TakeByte());

    bool enable = (msg->TakeByte() == 1);

    DGenerator::EnableChannel(ch, enable);
}


void DHandlers::SetFormWave(SimpleMessage *msg)
{
    Chan ch(msg->TakeByte());

    Form form(msg->TakeByte()); //-V656

    DGenerator::SetFormWave(ch, form);
}


static void SetGeneratorParameter(SimpleMessage *msg, void(*func)(Chan::E, FloatValue))
{
    Chan ch(msg->TakeByte());
    FloatValue value(msg->TakeDoubleWord());
    func(ch, value);
}


void DHandlers::SetFrequency(SimpleMessage *msg)
{
    SetGeneratorParameter(msg, DGenerator::SetFrequency);
}


void DHandlers::SetPeriod(SimpleMessage *msg)
{
    SetGeneratorParameter(msg, DGenerator::SetPeriod);
}


void DHandlers::SetAmplitude(SimpleMessage *msg)
{
    SetGeneratorParameter(msg, DGenerator::SetAmplitude);
}


void DHandlers::SetOffset(SimpleMessage *msg)
{
    SetGeneratorParameter(msg, DGenerator::SetOffset);
}


void DHandlers::SetDuration(SimpleMessage *msg)
{
    SetGeneratorParameter(msg, DGenerator::SetDuration);
}


void DHandlers::SetDutyRatio(SimpleMessage *msg)
{
    SetGeneratorParameter(msg, DGenerator::SetDutyRatio);
}


void DHandlers::SetPhase(SimpleMessage *msg)
{
    SetGeneratorParameter(msg, DGenerator::SetPhase);
}


void DHandlers::SetDelay(SimpleMessage *msg)
{
    SetGeneratorParameter(msg, DGenerator::SetDelay);
}


void DHandlers::SetManipulationDuration(SimpleMessage *msg)
{
    SetGeneratorParameter(msg, DGenerator::SetManipulationDuration);
}


void DHandlers::SetManipulationPeriod(SimpleMessage *msg)
{
    SetGeneratorParameter(msg, DGenerator::SetManipulationPeriod);
}


void DHandlers::SetPacketPeriod(SimpleMessage *msg)
{
    SetGeneratorParameter(msg, DGenerator::SetPacketPeriod);
}


void DHandlers::SetPacketNumber(SimpleMessage *msg)
{
    SetGeneratorParameter(msg, DGenerator::SetPacketNumber);
}


void DHandlers::SetManipulation(SimpleMessage *msg)
{
    Chan ch(msg->TakeByte());

    AD9952::Manipulation::SetEnabled(ch, msg->TakeByte() != 0);
}


void DHandlers::SetStartMode(SimpleMessage *msg)
{
    Chan ch(msg->TakeByte());

    StartMode mode = static_cast<StartMode>(msg->TakeByte());

    FPGA::SetStartMode(ch, mode);
}


void DHandlers::SetPolarity(SimpleMessage *message)
{
    Chan ch(message->TakeByte());

    FPGA::SetPolarity(ch, message->TakeByte());
}


void DHandlers::SetPin(SimpleMessage *message)
{
    uint8 port = message->TakeByte();
    uint8 pin = message->TakeByte();
    uint8 state = message->TakeByte();

    static const uint16 pins[] =
    {
        HPin::_0,
        HPin::_1,
        HPin::_2,
        HPin::_3,
        HPin::_4,
        HPin::_5,
        HPin::_6,
        HPin::_7,
        HPin::_8,
        HPin::_9,
        HPin::_10,
        HPin::_11,
        HPin::_12,
        HPin::_13,
        HPin::_14,
        HPin::_15
    };

    HAL_PIO::Write(static_cast<HPort::E>(port), pins[pin], state ? HState::Set : HState::Reset);
}


void DHandlers::LoadFormDDS(SimpleMessage *msg)
{
    Chan ch(msg->TakeByte());

    msg->TakeRemainigData(FPGA::DataDDS(ch));
}


void DHandlers::_SetKoeffCalibration(SimpleMessage *msg)
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


void DHandlers::_GetKoeffCalibration(SimpleMessage *)
{
}


void DHandlers::WriteRegister(SimpleMessage *msg)
{
    Register reg(msg->TakeByte());

    uint64 value = msg->TakeDoubleWord();

    switch (reg)
    {
    case Register::OffsetA:
        AD5697::SetOffset(Chan::A, FloatValue(static_cast<float>(value)));
        break;

    case Register::OffsetB:
        AD5697::SetOffset(Chan::B, FloatValue(static_cast<float>(value)));
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
        DInterface::ResetFreqForSend();
        break;

    case Register::FPGA_RG10_Offset:
        FPGA::WriteRegister(FPGA::RG::_10_Offset, value);
        break;

    case Register::FreqMeter_Resist:
        DFreqMeter::SetResist(static_cast<FreqResist::E>(value));
        break;

    case Register::FreqMeter_Couple:
        DFreqMeter::SetCouple(static_cast<FreqCouple::E>(value));
        break;

    case Register::FreqMeter_Filtr:
        DFreqMeter::SetFiltr(static_cast<FreqFiltr::E>(value));
        break;

    case Register::deleted_Multiplexor1:
    case Register::deleted_Multiplexor2:
    case Register::Count:
    default:
        // Здесь ничего
        break;
    }
}


void DHandlers::RunReset(SimpleMessage *)
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


void DHandlers::Test(SimpleMessage *)
{

}


void DHandlers::ModeDebug(SimpleMessage *)
{
}


void DHandlers::E(SimpleMessage *)
{

}
