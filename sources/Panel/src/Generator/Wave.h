#pragma once
#include "Settings/SettingsTypes.h"
#include "Settings/Settings.h"


class Wave
{
public:

    static pString Name(uint num, Language lang);

    /// Отвечает за отображение сигналов на экране
    class Graphic
    {
    public:
        /// Нарисовать информацию о параметрах сигнала на экране
        static void Draw(Chan ch);
        /// Позиция x области отрисовки сигнала
        static int X();
        /// Позиция y области отрисовки сигнала
        static int Y(Chan ch);
        /// Ширина области отрисовки сигнала
        static int Width();
        /// Высота области отрисовки сигнала
        static int Height();
    private:
    
        static void DrawSignalUGO(Chan chan, int y0);
    
        static void DrawSignalParameters(Chan chan, int y0);
    
        static void DrawParameterValue(Chan chan, WaveParameter parameter, int x, int y);
    };

    /// Отвечает за параметры сигнала
    class Signal
    {
    public:
        /// Возвращает текущую форму сигнала на данном канале
        static WaveForm Form(Chan ch = (Chan::E)CURRENT_CHANNEL);
        /// Устанавливает сигнал текущим на данный канал
        static void SetForm(Chan ch, WaveForm form);
    };
};
