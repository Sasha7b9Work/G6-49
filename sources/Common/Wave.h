#pragma once


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

    Parameter(int v = Number) : value((E)v), inNumLockMode(false) {};

    Parameter(int v, float _min, float _max) : value((E)v), min(_min), max(_max), inNumLockMode(false) { };

    operator uint8() const                  { return (uint8)value; }

    float MinValue() const { return min; };

    float MaxValue() const { return max; };

    pString Name() const;
    /// Возвращает true, если параметр является страницей параметров
    bool IsPage() const;

    bool InNumLockMode() const { return inNumLockMode; };

    void SetNumLockMode(bool mode) { inNumLockMode = mode; };
private:

    float min;

    float max;
    /// Если true, то находимся в режиме клавиатурного ввода (кнопками 1...9)
    bool inNumLockMode;
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
    Form(E v = Number) : value(v), currentParam(0)   { };
    Form(E v, Parameter param[Parameter::Number]);
    operator uint8() const    { return (uint8)value;  };
    /// Возвращает человеческое название формы сигнала
    pString Name(Language lang) const;
    /// Возвращает ссылку на текущий параметр
    Parameter *CurrentParameter();
    /// Возвращает количество доступных параметров
    int NumParameters() const;
    /// Возвращает ссылку на i-ый параметр из массива params
    Parameter *GetParameter(int i);
    /// Установить текущим следующй параметр
    void SetNextParameter();
    /// Номер текущего параметра в массиве params
    int currentParam;

    Parameter params[Parameter::Number];
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Wave
{
public:

    Wave(Chan ch, Form form[Form::Number]);

    Form forms[Form::Number];

    /// Возвращает установленную форму
    Form *GetCurrentForm();
    /// Установить текущей следующую форму
    void SetNextForm();

    int NumberOfForms() const;

    Form *GetForm(int i);
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

        static void DrawParameterValue(Chan chan, Parameter *parameter, int x, int y);
    };
};
