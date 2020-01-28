#pragma once
#include "common/Common.h"
#include "common/CommonTypes.h"


struct Calibrator
{
    // Устанавливает коэффициент калибровки, одновременно устанавливая необоходимые для этого коэффициента настройки
    static void SetK(uint8 channel, uint8 signal, uint8 range, uint8 param, int16 k);
    /// Возвращает коэффициент калибровки амплитуды
    static float GetAmplitudeK(Chan::E ch, TypeForm::E form);
    /// Возвращает калибровочный коэффициент смещения для 0В
    static float GetOffsetK_Zero(Chan::E ch, TypeForm::E form);
    /// Возвращает калибровочный коэффициент смещения для -5В
    static float GetOffsetK_Negative(Chan::E ch, TypeForm::E form);
    /// Возвращает калбировочный коэффициент смещения для +5В
    static float GetOffsetK_Positive(Chan::E ch, TypeForm::E form);
    /// Возвращает калибровочный коэффициент уровня синхронизации частотомера
    static float GetFreqMeterK_Trig();

private:
    static uint8 CalculateRange(Chan::E ch);
    /// Устанавливает форму сигнала для соотвествующей формы сигнала
    static void SetFormWave(Chan::E ch, uint8 signal);
    /// Установить амплитуду для заданного диапазона
    static void SetAmplitude(Chan::E ch, uint8 range);
    /// Установить смещение для заданного параметра
    static void SetOffset(Chan::E ch, uint8 param);
};
