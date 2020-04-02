#include "defines.h"
#include "common/Command.h"
#include "common/Messages.h"
#include "Updater_dl.h"
#include "Hardware/HAL/HAL.h"


// ¬рем€ окончани€ работы Updater. ≈сли до этого времени не поступил запрос на обновление - будем продолжать без обновлени€
static uint timeEnd = 0xFFFFFFFF;


struct State
{
    enum E
    {
        Idle,       // Ќачальное состо€ние
        Update,     // Ќаходимс€ в состо€нии обновлени€
        Ended,      // ќбновление окончено
        Count
    };
};


static State::E state = State::Idle;


static void E(SimpleMessage *);

static void OnRequestUpdate(SimpleMessage *);


bool Updater::Process()
{
    if(timeEnd == 0xFFFFFFFF)
    {
        timeEnd = HAL_TIM::TimeMS() + 1000;
    }

    if(state == State::Idle && HAL_TIM::TimeMS() >= timeEnd)
    {
        return false;
    }

    if(state == State::Ended)
    {
        return false;
    }

    return true;
}


void Updater::Handler(SimpleMessage *message)
{
    typedef void(*pFunc)(SimpleMessage *);

    static const pFunc funcs[Command::Count] =
    {
        /* RequestData               */ E,
        /* EnableChannel             */ E,
        /* SetFormWave               */ E,
        /* SetFrequency              */ E,
        /* SetAmplitude              */ E,
        /* SetOffset                 */ E,
        /* SetDuration               */ E,
        /* SetDutyRatio              */ E,
        /* SetPhase                  */ E,
        /* RunReset                  */ E,
        /* ModeDebug                 */ E,
        /* SetDelay                  */ E,
        /* WriteRegister             */ E,
        /* SetDurationRise           */ E,
        /* SetDurationFall           */ E,
        /* SetDurationStady          */ E,
        /* SetDutyFactor             */ E,
        /* SetManipulation           */ E,
        /* SetManipulationDuration   */ E,
        /* SetManipulationPeriod     */ E,
        /* SetPacketPeriod           */ E,
        /* SetPacketNumber           */ E,
        /* SetStartMode              */ E,
        /* SetPeriod                 */ E,
        /* SetPolarity               */ E,
        /* SetManipulationMode       */ E,
        /* LoadFromDDS               */ E,
        /* FreqMeasure               */ E,
        /* Log                       */ E,
        /* FDrive_NumDirsAndFiles    */ E,
        /* FDrive_Mount              */ E,
        /* FDrive_RequestDir         */ E,
        /* FDrive_RequestFile        */ E,
        /* Test                      */ E,
        /* SetKoeffCalibration       */ E,
        /* GetKoeffCalibration       */ E,
        /* FDrive_RequestFileSize    */ E,
        /* FDrive_RequestFileString  */ E,
        /* FDrive_LoadFromExtStorage */ E,
        /* FDrive_GetPictureDDS      */ E,
        /* SCPI_RecvData             */ E,
        /* PortCPU                   */ E,
        /* CalibrationLoad           */ E,
        /* CalibrationSet            */ E,
        /* StartApplication          */ E,
        /* RequestUpdate             */ OnRequestUpdate
    };

    message->ResetPointer();

    uint8 com = message->TakeByte();

    if(com < Command::Count)
    {
        pFunc func = funcs[com];

        func(message);
    }
}


static void E(SimpleMessage *)
{

}


static void OnRequestUpdate(SimpleMessage *)
{
}
