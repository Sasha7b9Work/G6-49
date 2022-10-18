// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Common.h"
#include "common/CommonTypes.h"


struct CalSignal
{
    // Калибруем прибор для трёх трактов - синусоидального, произвольного сигнала и импульсного
    enum E
    {
        Sine,
        DDS,
        Impulse,
        Count
    };

    static TypeForm::E ToForm(E e)
    {
        if (e == Sine)
        {
            return TypeForm::Sine;
        }

        return (e == DDS) ? TypeForm::Triangle : TypeForm::Meander;
    }

    static E FromForm(TypeForm::E form)
    {
        if (form == TypeForm::Sine)
        {
            return Sine;
        }
        else if (form == TypeForm::Meander || form == TypeForm::Impulse || form == TypeForm::Packet)
        {
            return Impulse;
        }

        return DDS;
    }
};


namespace Calibrator
{
    // Устанавливает коэффициент калибровки, одновременно устанавливая необоходимые для этого коэффициента настройки
    // param : 0 - амплитуда
    //         1 - +2.5В / +5В
    //         2 - 0В
    //         3 - -2.5В / -5В

    void SetK(uint8 channel, CalSignal::E, uint8 range, uint8 param, int16 k);
    
    // Возвращает коэффициент калибровки амплитуды
    float GetAmplitudeK(const Chan &);
    
    // Возвращает калибровочный коэффициент смещения для 0В
    float GetOffsetK_Zero(const Chan &);
    
    // Возвращает калибровочный коэффициент смещения для -5В
    double GetOffsetK_Negative(const Chan &);
    
    // Возвращает калбировочный коэффициент смещения для +5В
    double GetOffsetK_Positive(const Chan &);
    
    // Установка калибровочного коэффицинета уровня синхронизации частотомера
    void SetFreqMeterK_Trig(int16 k);

    // Возвращает калибровочный коэффициент уровня синхронизации частотомера
    float GetFreqMeterK_Trig();
};
