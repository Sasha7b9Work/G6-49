// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "structs.h"
#include "common/Command.h"
#include "common/Interface_d.h"
#include "common/Handlers_d.h"
#include "common/Transceiver.h"
#include "FDrive/FDrive_d.h"
#include "FreqMeter/FreqMeter_d.h"
#include "Generator/Calibrator.h"
#include "Generator/Generator_d.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "Hardware/VCP_d.h"
#include "Hardware/HAL/HAL.h"
#include "Settings/CalibrationSettings.h"
#include "Log.h"


static void E(SimpleMessage *);

static void SendData(SimpleMessage *);

static void EnableChannel(SimpleMessage *);

static void SingleStart(SimpleMessage *);

static void SetFormWave(SimpleMessage *);

static void SetFrequency(SimpleMessage *);

static void SetAmplitude(SimpleMessage *);

static void SetOffset(SimpleMessage *);

static void SetDuration(SimpleMessage *);

static void SetPhase(SimpleMessage *);

static void ModeDebug(SimpleMessage *);

static void SetDelayStartStop(SimpleMessage *);

static void WriteRegister(SimpleMessage *);

static void SetManipulation(SimpleMessage *);

static void SetManipulationDuration(SimpleMessage *);

static void SetManipulationPeriod(SimpleMessage *);

static void SetPacketPeriod(SimpleMessage *);

static void SetPacketNumber(SimpleMessage *);

static void SetStartMode(SimpleMessage *);

static void SetStartStopMode(SimpleMessage *);

static void SetPeriod(SimpleMessage *);

static void SetPolarity(SimpleMessage *);

static void SetPin(SimpleMessage *);

static void LoadFormDDS(SimpleMessage *);

static void Test(SimpleMessage *);

static void CalibrationLoad(SimpleMessage *);

static void CalibrationSet(SimpleMessage *);

static void FDebug(SimpleMessage *);

static void SetFreqMeterTrigK(SimpleMessage *);

static void RequestPictureDDSfromStorage(SimpleMessage *);

static void LoadDDSfromStorage(SimpleMessage *);


void DHandlers::Processing(SimpleMessage *msg)
{
    typedef void(*pFuncInterfaceVpM)(SimpleMessage *);

    uint8 com = msg->TakeUINT8();

    pFuncInterfaceVpM func = E;

    switch(com)
    {
    case Command::RequestData:                  func = SendData;                     break;
    case Command::EnableChannel:                func = EnableChannel;                break;
    case Command::SetFormWave:                  func = SetFormWave;                  break;
    case Command::SetFrequency:                 func = SetFrequency;                 break;
    case Command::SetAmplitude:                 func = SetAmplitude;                 break;
    case Command::SetOffset:                    func = SetOffset;                    break;
    case Command::SetDuration:                  func = SetDuration;                  break;
    case Command::SetPhase:                     func = SetPhase;                     break;
    case Command::ModeDebug:                    func = ModeDebug;                    break;
    case Command::SetDelayStartStop:            func = SetDelayStartStop;            break;
    case Command::WriteRegister:                func = WriteRegister;                break;
    case Command::SetManipulation:              func = SetManipulation;              break;
    case Command::SetManipulationDuration:      func = SetManipulationDuration;      break;
    case Command::SetManipulationPeriod:        func = SetManipulationPeriod;        break;
    case Command::SetPacketPeriod:              func = SetPacketPeriod;              break;
    case Command::SetPacketNumber:              func = SetPacketNumber;              break;
    case Command::SetStartMode:                 func = SetStartMode;                 break;
    case Command::SetPeriod:                    func = SetPeriod;                    break;
    case Command::SetPolarity:                  func = SetPolarity;                  break;
    case Command::LoadFormDDS:                  func = LoadFormDDS;                  break;
    case Command::Test:                         func = Test;                         break;
    case Command::PortCPU:                      func = SetPin;                       break;
    case Command::CalibrationLoad:              func = CalibrationLoad;              break;
    case Command::CalibrationSet:               func = CalibrationSet;               break;
    case Command::SCPI_Data:                    func = DVCP::Handler::Processing;    break;
    case Command::Debug:                        func = FDebug;                       break;
    case Command::SetFreqMeterTrigK:            func = SetFreqMeterTrigK;            break;
    case Command::RequestPictureDDSfromStorage: func = RequestPictureDDSfromStorage; break;
    case Command::LoadDDSfromStorage:           func = LoadDDSfromStorage;           break;
    case Command::SetStartStopMode:             func = SetStartStopMode;             break;

    case Command::FDrive_NumDirsAndFiles:
    case Command::FDrive_Mount:
    case Command::FDrive_RequestDir:
    case Command::FDrive_RequestFile:
    case Command::FDrive_RequestFileString:
    case Command::FDrive_RequestFileSize:
    case Command::FDrive_LoadDDSfromFile:
    case Command::FDrive_RequestPictureDDSfromFile:
    case Command::FDrive_CreateFile:
    case Command::FDrive_WriteToFile:
    case Command::FDrive_CloseFile:
        func = DDrive::Handler::Processing;
        break;
    case Command::SingleStart : func = SingleStart; break;
    }

    func(msg);
}


static void SendData(SimpleMessage *)
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


static void EnableChannel(SimpleMessage *msg)
{
    Chan ch(msg->TakeUINT8());

    bool enable = (msg->TakeUINT8() == 1);

    DGenerator::EnableChannel(ch, enable);
}


static void SingleStart(SimpleMessage *)
{
    DGenerator::SingleStart();
}


static void SetFormWave(SimpleMessage *msg)
{
    Chan ch(msg->TakeUINT8());

    DGenerator::SetFormWave(ch, (TypeForm::E)msg->TakeUINT8());
}


static void SetGeneratorParameter(SimpleMessage *msg, void(*func)(const Chan &, const Value &))
{
    Chan ch(msg->TakeUINT8());

    Value value(0.0F);
    value.FromUINT64(msg->TakeUINT64());

    func(ch, value);
}


static void SetFrequency(SimpleMessage *msg)
{
    SetGeneratorParameter(msg, DGenerator::SetFrequency);
}


static void SetPeriod(SimpleMessage *msg)
{
    SetGeneratorParameter(msg, DGenerator::SetPeriod);
}


static void SetAmplitude(SimpleMessage *msg)
{
    SetGeneratorParameter(msg, DGenerator::SetAmplitude);
}


static void SetOffset(SimpleMessage *msg)
{
    SetGeneratorParameter(msg, DGenerator::SetOffset);
}


static void SetDuration(SimpleMessage *msg)
{
    SetGeneratorParameter(msg, DGenerator::SetDuration);
}


static void SetPhase(SimpleMessage *msg)
{
    SetGeneratorParameter(msg, DGenerator::SetPhase);
}


static void SetDelayStartStop(SimpleMessage *msg)
{
    SetGeneratorParameter(msg, DGenerator::SetDelayStartStop);
}


static void SetManipulationDuration(SimpleMessage *msg)
{
    SetGeneratorParameter(msg, DGenerator::SetManipulationDuration);
}


static void SetManipulationPeriod(SimpleMessage *msg)
{
    SetGeneratorParameter(msg, DGenerator::SetManipulationPeriod);
}


static void SetPacketPeriod(SimpleMessage *msg)
{
    SetGeneratorParameter(msg, DGenerator::SetPacketPeriod);
}


static void SetPacketNumber(SimpleMessage *msg)
{
    SetGeneratorParameter(msg, DGenerator::SetPacketNumber);
}


static void SetManipulation(SimpleMessage *msg)
{
    Chan ch(msg->TakeUINT8());

    AD9952::Manipulation::SetEnabled(ch, msg->TakeUINT8() != 0);
}


static void SetStartMode(SimpleMessage *msg)
{
    uint8 firstByte = msg->TakeUINT8();

    Chan ch((uint8)(firstByte & 1));

    StartMode::E mode = (StartMode::E)msg->TakeUINT8();

    FPGA::SetStartMode(ch, (uint8)((firstByte & 2) >> 1), mode);
}


static void SetStartStopMode(SimpleMessage *msg)
{
    FPGA::StartStop::Mode::Set((FPGA::StartStop::Mode::E)msg->TakeUINT8());
}


static void SetPolarity(SimpleMessage *message)
{
    Chan ch(message->TakeUINT8());

    FPGA::SetPolarity(ch, message->TakeUINT8());
}


static void SetPin(SimpleMessage *message)
{
    uint8 port = message->TakeUINT8();
    uint8 pin = message->TakeUINT8();
    uint8 state = message->TakeUINT8();

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

    HAL_PIO::Write((HPort::E)port, pins[pin], state ? HState::Set : HState::Reset);
}


static void LoadFormDDS(SimpleMessage *msg)
{
    Chan ch(msg->TakeUINT8());

    msg->TakeRemainigData(FPGA::DataDDS(ch));
}


static void CalibrationLoad(SimpleMessage *msg)
{
    uint8 channel = msg->TakeUINT8();
    uint8 signal = msg->TakeUINT8();
    uint8 range = msg->TakeUINT8();
    uint8 param = msg->TakeUINT8();
    
    int16 k = msg->TakeINT16();
    
    *setCal.GetK(channel, signal, range, param) = k;
}


static void CalibrationSet(SimpleMessage *msg)
{
    uint8 channel = msg->TakeUINT8();
    uint8 signal = msg->TakeUINT8();
    uint8 range = msg->TakeUINT8();
    uint8 param = msg->TakeUINT8();
    
    int16 k = msg->TakeINT16();
    
    Calibrator::SetK(channel, signal, range, param, k);
}


static void SetFreqMeterTrigK(SimpleMessage *msg)
{
    int16 k = msg->TakeINT16();

    Calibrator::SetFreqMeterK_Trig(k);
}


static void WriteRegister(SimpleMessage *msg)
{
    Register reg(msg->TakeUINT8());

    Value value(0.0F);
    value.FromUINT64(msg->TakeUINT64());

    switch (reg)
    {
    case Register::OffsetA:
        DGenerator::SetOffset(ChA, value);
        AD5697::SetOffset(ChA);
        break;

    case Register::OffsetB:
        DGenerator::SetOffset(ChB, value);
        AD5697::SetOffset(ChB);
        break;

    case Register::FreqMeterLevel:
        AD5697::SetFreqLevel(value.ToDouble());
        break;

    case Register::FreqMeterHYS:
        AD5697::SetFreqHysteresys(value.ToDouble());
        break;

    case Register::FPGA_RG0_Control:
        FPGA::Register::Write(FPGA::Register::_0_Control, value.GetRawValue());
        break;

    case Register::FPGA_RG1_Freq:
        FPGA::Register::Write(FPGA::Register::_1_Freq, value.GetRawValue());
        break;

    case Register::FPGA_RG2_Amplitude:
        FPGA::Register::Write(FPGA::Register::_2_Amplitude, value.GetRawValue());
        break;

    case Register::FPGA_RG3_RectA:
        FPGA::Register::Write(FPGA::Register::_3_RectA, value.GetRawValue());
        break;

    case Register::FPGA_RG4_RectB:
        FPGA::Register::Write(FPGA::Register::_4_RectB, value.GetRawValue());
        break;

    case Register::FPGA_RG5_PeriodImpulseA:
        FPGA::Register::Write(FPGA::Register::_5_PerImp_Freq_A_PerPack, value.GetRawValue());
        break;

    case Register::FPGA_RG6_DurationImpulseA:
        FPGA::Register::Write(FPGA::Register::_6_DurImp_A_NumbImp, value.GetRawValue());
        break;

    case Register::FPGA_RG7_PeriodImpulseB:
        FPGA::Register::Write(FPGA::Register::_7_PerImp_Freq_B_DelayStartStop, value.GetRawValue());
        break;

    case Register::FPGA_RG8_DurationImpulseB:
        FPGA::Register::Write(FPGA::Register::_8_DurImp_B, value.GetRawValue());
        break;

    case Register::FPGA_RG9_FreqMeter:
        FPGA::Register::Write(FPGA::Register::_9_FreqMeter, value.GetRawValue());
        DInterface::ResetFreqForSend();
        break;

    case Register::FPGA_RG10_Offset:
        FPGA::Register::Write(FPGA::Register::_10_Offset, value.GetRawValue());
        break;

    case Register::FreqMeter_Resist:
        DFreqMeter::SetResist((FreqResist::E)value.GetRawValue());
        break;

    case Register::FreqMeter_Couple:
        DFreqMeter::SetCouple((FreqCouple::E)value.GetRawValue());
        break;

    case Register::FreqMeter_Filtr:
        DFreqMeter::SetFiltr((FreqFiltr::E)value.GetRawValue());
        break;

    case Register::deleted_Multiplexor1:
    case Register::deleted_Multiplexor2:
    case Register::Count:
    default:
        // Здесь ничего
        break;
    }
}


static void Test(SimpleMessage *)
{

}


static void ModeDebug(SimpleMessage *)
{
}


static void E(SimpleMessage *)
{

}


static void FDebug(SimpleMessage *)
{
//    Amplifier::LogState();
}


static void RequestPictureDDSfromStorage(SimpleMessage *message)
{
    Chan ch(message->TakeUINT8());

    uint16 *code = HAL_EEPROM::Signal::Get(ch);

    float data[FPGA::NUM_POINTS];

    FPGA::TransformCodeToData((uint8 *)code, data);

    const int size = 240;

    float aveValue = 127.0F;

    float step = FPGA::NUM_POINTS / (float)size;

    uint8 picture[size];

    for (uint i = 0; i < size; i++)
    {
        float val = data[(int)((float)i * step)];

        picture[i] = (uint8)(aveValue + val * 125.0F);
    }

    Message::Storage::RequestPictureDDS((uint8)ch, picture).Transmit();
}


static void LoadDDSfromStorage(SimpleMessage *)
{

}
