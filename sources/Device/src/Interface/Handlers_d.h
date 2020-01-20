#pragma once
#include "common/Message.h"


struct DHandlers
{
    static void Processing(SimpleMessage *message);

private:
    static void E(SimpleMessage *);

    static void SendData(SimpleMessage *);

    static void EnableChannel(SimpleMessage *);

    static void SetFormWave(SimpleMessage *);

    static void SetFrequency(SimpleMessage *);

    static void SetAmplitude(SimpleMessage *);

    static void SetOffset(SimpleMessage *);

    static void SetDuration(SimpleMessage *);

    static void SetDutyRatio(SimpleMessage *);

    static void SetPhase(SimpleMessage *);

    static void RunReset(SimpleMessage *);

    static void ModeDebug(SimpleMessage *);

    static void SetDelay(SimpleMessage *);

    static void WriteRegister(SimpleMessage *);

    static void SetManipulation(SimpleMessage *);

    static void SetManipulationDuration(SimpleMessage *);

    static void SetManipulationPeriod(SimpleMessage *);

    static void SetPacketPeriod(SimpleMessage *);

    static void SetPacketNumber(SimpleMessage *);

    static void SetStartMode(SimpleMessage *);

    static void SetPeriod(SimpleMessage *);

    static void SetPolarity(SimpleMessage *);

    static void SetPin(SimpleMessage *);

    static void LoadFormDDS(SimpleMessage *);

    static void Test(SimpleMessage *);

    static void _SetKoeffCalibration(SimpleMessage *);

    static void _GetKoeffCalibration(SimpleMessage *);

    static void CalibrationLoad(SimpleMessage *);

    static void CalibrationSet(SimpleMessage *);
};
