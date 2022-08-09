// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Signals/Wave.h"
#include "common/Command.h"


#define START_MODE(ch)          (set.fpga_startMode[ch])
#define START_MODE_A            START_MODE(Chan::A)
#define START_MODE_B            START_MODE(Chan::B)
#define START_MODE_A_IS_SINGLE  (START_MODE_A == StartMode::Single)
#define START_MODE_B_IS_SINGLE  (START_MODE_B == StartMode::Single)


namespace PGenerator
{
    static const int DDS_NUM_POINTS = 8 * 1024;

    void EnableChannel(const Chan &, bool enable);

    void SingleStart();

    void SetFormWave(const Wave *w);

    void SetFormWave(const Chan &, TypeForm::E form);

    void SetParameterDouble(ParameterDouble *param);

    void SetParameterChoice(ParameterChoice *param);

    void SetParameterInteger(ParameterInteger *param);

    void SetParameter(Param *param);

    void SetOffset(const Chan &, float offset);

    void SetAmplitude(const Chan &, float amplitude);

    void SetDebugMode(bool enable);

    void LoadRegister(Register::E reg, uint64 value);

    void LoadSettings();

    void TuneChannel(const Chan &);

    // Загружает режим запуска из set в аппаратуру для произвольного (signal == 0) или импульсного (signal == 1) сигнала
    void LoadStartMode(const Chan &, int signal, int mode);

    // Загружает изображение произвольного сигнала, хранящееся в ППЗУ
    void LoadPictureDDSfromStorage(const Chan &);

    void Reset();
};
