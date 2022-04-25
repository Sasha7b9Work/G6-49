#pragma once
#include "Generator/Wave.h"
#include "common/Command.h"


#define START_MODE(ch)          (set.fpga_startMode[ch])
#define START_MODE_A            START_MODE(Chan::A)
#define START_MODE_B            START_MODE(Chan::B)
#define START_MODE_A_IS_SINGLE  (START_MODE_A == StartMode::Single)
#define START_MODE_B_IS_SINGLE  (START_MODE_B == StartMode::Single)

class SimpleMessage;


struct PGenerator
{
    static const int DDS_NUM_POINTS = 8 * 1024;

    static void EnableChannel(Chan::E ch, bool enable);

    static void SingleStart();

    static void SetFormWave(const Wave *w);

    static void SetFormWave(Chan::E ch, TypeForm::E form);

    static void SetParameterDouble(ParameterDouble *param);

    static void SetParameterChoice(ParameterChoice *param);

    static void SetParameterInteger(ParameterInteger *param);

    static void SetParameterCommon(Parameter *param);

    static void SetOffset(Chan::E ch, float offset);

    static void SetAmplitude(Chan::E ch, float amplitude);

    static void SetDebugMode(bool enable);

    static void LoadRegister(Register::E reg, uint64 value);

    static void LoadSettings();

    static void TuneChannel(Chan::E ch);

    // Загружает режим запуска из set в аппаратуру для произвольного (signal == 0) или импульсного (signal == 1) сигнала
    static void LoadStartMode(Chan::E ch, int signal, int mode);

    // Загружает изображение произвольного сигнала, хранящееся в ППЗУ
    static void LoadPictureDDSfromStorage(Chan::E ch);

    static void Reset();

private:

    // Загружает форму произвольного сигнала
    static void LoadFormDDS(Form *form);

    // Преобразует данные, записанные в относительных единицах [-1.0f;1.0f] в данные, пригодные для отправки в ПЛИС
    static void TransformDataToCodeAndTransmit(const float data[DDS_NUM_POINTS], Form *form);
};
