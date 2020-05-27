#pragma once


class Parameter;


struct PageTuneParameter
{
    static Page *self;

    // Устанавливает параметр для редактирования
    static void SetParameter(ParameterValue *);

    // Установить режим ввода значения
    static void SetModeEntering();

    // Сбросить режим ввода значения
    static void ResetModeEntering();
};
