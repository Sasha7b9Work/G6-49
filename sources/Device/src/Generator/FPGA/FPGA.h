// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Generator/Generator_d.h"
#include "common/Command.h"
#include "Generator/FPGA/RegistersFPGA.h"
#include "Generator/FPGA/ParametersFPGA.h"


namespace FPGA
{
    static const int NUM_POINTS = 1024 * 8;

    void Init();

    void SetWaveForm(const Chan &, TypeForm::E);

    void SetFrequency(const Chan &);

    void SetAmplitude();

    void SetDurationImpulse(const Chan &, const Value &duration);

    void SetPeriodImpulse(const Chan &, const Value &period);

    // Задержка между импульсами на выходах первого и второго каналов
    void SetDelayStartStop(const Value &delay);

    // 1 - положительная полярность, 0 - отрицательная полярность
    void SetPolarity(const Chan &, uint8 polarity);

    // Установка режима запуска для произвольного (0) сигнала и импульсного (1) сигнала signal
    void SetStartMode(const Chan &, uint8 signal, StartMode::E);

    void EnableStartStopMode(StartStopMode::E);

    // Делает однократный запуск
    void SingleStart();

    void TransformCodeToData(const uint8 codeIn[FPGA::NUM_POINTS * 2], float dataOut[FPGA::NUM_POINTS]);

    // Возвращает указатель на точки сигнала, загружаемого из флешки
    uint8 *DataFreeSignal(const Chan &);

    // Сохранить данные сигнала, загруженного с флешки
    void SaveExtSignal(const Chan &, uint8 *data); 

    // Возвращает указатель на точки произвольного сигнала (программно определёного)
    uint8 *DataDDS(const Chan &);

    // Запись управляющего регистра
    void WriteControlRegister();

    namespace PacketImpulse
    {
        // Устанавливает число импульсов в пачке
        void SetNumberImpulses(uint n);

        extern Value periodImpulse;
    };

    namespace ModeWork
    {
        enum E
        {
            None,
            DDS,            // Работает по загруженным точкам заранее рассчитанных сигналов - пила, треугольник
            Impulse,        // Режим, в котором импульcы могут иметь разную частоту
            Impulse2,       // Режим, в котором импульсы имеют одинаковую частоту. При этом можно регулировать задержку второго канала отн. первого
            Rectangle,
            Meander,
            PackedImpulse,  // Пакеты импульсов
            Sine,
            Free,
            Count
        };

        E Current(const Chan &);
    };
};
