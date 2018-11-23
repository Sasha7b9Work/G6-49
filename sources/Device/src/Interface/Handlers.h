#pragma once
#include "Message.h"


class Handlers
{
public:
    static void Processing(Message *message);

private:

    static void E(Message *);

    static void SendData(Message *);

    static void EnableChannel(Message *);

    static void SetFormWave(Message *);

    static void SetFrequency(Message *);

    static void SetAmplitude(Message *);

    static void SetOffset(Message *);

    static void SetDuration(Message *);

    static void SetDutyRatio(Message *);

    static void SetPhase(Message *);

    static void RunReset(Message *);

    static void ModeDebug(Message *);

    static void SetDelay(Message *);

    static void WriteRegister(Message *);

    static void SetManipulation(Message *);

    static void SetManipulationDuration(Message *);

    static void SetManipulationPeriod(Message *);

    static void SetPacketPeriod(Message *);

    static void SetPacketNumber(Message *);

    static void SetStartMode(Message *);

    static void SetPeriod(Message *);

    static void SetPolarity(Message *);

    static void SetManipulationMode(Message *);

    static void LoadFormDDS(Message *);

    static void Test(Message *);

    static void SetKoeffCalibration(Message *);

    static void GetKoeffCalibration(Message *);
};
