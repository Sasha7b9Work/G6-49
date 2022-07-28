// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
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

    static void EnableChannel(const Chan &, bool enable);

    static void SingleStart();

    static void SetFormWave(const Wave *w);

    static void SetFormWave(const Chan &, TypeForm::E form);

    static void SetParameterDouble(ParameterDouble *param);

    static void SetParameterChoice(ParameterChoice *param);

    static void SetParameterInteger(ParameterInteger *param);

    static void SetParameter(Parameter *param);

    static void SetOffset(const Chan &, float offset);

    static void SetAmplitude(const Chan &, float amplitude);

    static void SetDebugMode(bool enable);

    static void LoadRegister(Register::E reg, uint64 value);

    static void LoadSettings();

    static void TuneChannel(const Chan &);

    // ��������� ����� ������� �� set � ���������� ��� ������������� (signal == 0) ��� ����������� (signal == 1) �������
    static void LoadStartMode(const Chan &, int signal, int mode);

    // ��������� ����������� ������������� �������, ���������� � ����
    static void LoadPictureDDSfromStorage(const Chan &);

    static void Reset();

private:

    // ��������� ����� ������������� �������
    static void LoadFormDDS(Form *form);

    // ����������� ������, ���������� � ������������� �������� [-1.0f;1.0f] � ������, ��������� ��� �������� � ����
    static void TransformDataToCodeAndTransmit(const float data[DDS_NUM_POINTS], Form *form);
};
