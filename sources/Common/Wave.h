#pragma once


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Wave
{
public:
    struct Form
    {
        enum E
        {
            Sine,       ///< Синус
            RampPlus,   ///< Пила+
            RampMinus,  ///< Пила-
            Meander,    ///< Меандр
            Impulse,    ///< Импульсы
            Number
        } value;
        Form(E v = Number) : value((E)v) { };
        operator uint8() const           { return (uint8)value; };
        pString Name(Language lang) const;
        bool NameIsEqual(Form &form) const;

        struct Parameter
        {
            enum E
            {
                Frequency,          ///< Частота
                Period,             ///< Период
                Amplitude,          ///< Амплитуда
                Offset,             ///< Смещение
                Duration,           ///< Длительность
                DutyRatio,          ///< Скважность
                Phase,              ///< Сдвиг фазы
                Delay,              ///< Задержка
                DepthModulation,    ///< Глубина амплитудной модуляции
                Polarity,           ///< Полярность
                DurationRise,       ///< Длительность нарастания
                DurationFall,       ///< Длительность спада
                DurationStady,      ///< Длительность установившего значения
                DutyFactor,         ///< Коэффициент заполнения
                Modulation,         ///< Модуляция
                Number
            } value;

            Parameter(int v = Number) : value((E)v) { };
            operator uint8() const                  { return (uint8)value; }
            float MinValue() const;
            float MaxValue() const;
            pString Name() const;
            /// Возвращает true, если параметр является страницей параметров
            bool IsPage() const;
        } parameters[Wave::Form::Parameter::Number];

    } forms[Wave::Form::Number];

    /// Возвращает установленную форму
    Form GetCurrentForm();

    void SetCurrentForm(Wave::Form form);

    int NumberOfForms() const;

    Form GetForm(int i);

    Wave(Chan ch, Form form[Form::Number]);
    /// Какому каналу принадлежит сигнал
    Chan channel;

private:
    /// Текущая форма сигнала - указывает на номер сигнала в массиве
    int currentForm;

public:

    class Graphics
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

        static void DrawUGO(Chan chan, int y0);

        static void DrawSine(int x, int y, int width, int height);

        static void DrawRampPlus(int x, int y, int width, int height);

        static void DrawRampMinus(int x, int y, int width, int height);

        static void DrawMeander(int x, int y, int width, int height);

        static void DrawImpulse(int x, int y, int width, int height);

        static void DrawParameters(Chan chan, int y0);

        static void DrawParameterValue(Chan chan, Form::Parameter parameter, int x, int y);
    };
};
