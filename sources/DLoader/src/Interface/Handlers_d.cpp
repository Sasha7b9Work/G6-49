#include "defines.h"
#include "structs.h"
#include "common/Command.h"
#include "common/Transceiver.h"
#include "Handlers_d.h"
#include "FDrive/FDrive_d.h"
#include "Interface_d.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Settings/CalibrationSettings.h"


void DHandlers::Processing(SimpleMessage *msg)
{
    typedef void(*pFuncInterfaceVpM)(SimpleMessage *);

    static const pFuncInterfaceVpM funcs[Command::Count] =
    {
        /* RequestData               */ DHandlers::E,
        /* EnableChannel             */ DHandlers::E,
        /* SetFormWave               */ DHandlers::E,
        /* SetFrequency              */ DHandlers::E,
        /* SetAmplitude              */ DHandlers::E,
        /* SetOffset                 */ DHandlers::E,
        /* SetDuration               */ DHandlers::E,
        /* SetDutyRatio              */ DHandlers::E,
        /* SetPhase                  */ DHandlers::E,
        /* RunReset                  */ DHandlers::E,
        /* ModeDebug                 */ DHandlers::E,
        /* SetDelay                  */ DHandlers::E,
        /* WriteRegister             */ DHandlers::E,
        /* SetDurationRise           */ DHandlers::E,
        /* SetDurationFall           */ DHandlers::E,
        /* SetDurationStady          */ DHandlers::E,
        /* SetDutyFactor             */ DHandlers::E,
        /* SetManipulation           */ DHandlers::E,
        /* SetManipulationDuration   */ DHandlers::E,
        /* SetManipulationPeriod     */ DHandlers::E,
        /* SetPacketPeriod           */ DHandlers::E,
        /* SetPacketNumber           */ DHandlers::E,
        /* SetStartMode              */ DHandlers::E,
        /* SetPeriod                 */ DHandlers::E,
        /* SetPolarity               */ DHandlers::E,
        /* SetManipulationMode       */ DHandlers::E,
        /* LoadFromDDS               */ DHandlers::E,
        /* FreqMeasure               */ DHandlers::E,
        /* Log                       */ DHandlers::E,
        /* FDrive_NumDirsAndFiles    */ DHandlers::E,
        /* FDrive_Mount              */ DHandlers::E,
        /* FDrive_RequestDir         */ DHandlers::E,
        /* FDrive_RequestFile        */ DHandlers::E,
        /* Test                      */ DHandlers::E,
        /* SetKoeffCalibration       */ DHandlers::E,
        /* GetKoeffCalibration       */ DHandlers::E,
        /* FDrive_RequestFileSize    */ DHandlers::E,
        /* FDrive_RequestFileString  */ DHandlers::E,
        /* FDrive_LoadFromExtStorage */ DHandlers::E,
        /* FDrive_GetPictureDDS      */ DHandlers::E,
        /* SCPI_RecvData             */ DHandlers::E,
        /* PortCPU                   */ DHandlers::E,
        /* CalibrationLoad           */ DHandlers::E,
        /* CalibrationSet            */ DHandlers::E,
        /* StartApplication          */ DHandlers::E
    };

    uint8 com = msg->TakeByte();

    pFuncInterfaceVpM func = funcs[com];

//    if(func != DHandlers::SendData && func != DHandlers::E)
//    {
//        func = func;
//    }

    func(msg);
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


void DHandlers::E(SimpleMessage *)
{

}
