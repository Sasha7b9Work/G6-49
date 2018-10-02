#pragma once
#include "defines.h"
#include <string.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct Order
{
    enum E
    {
        Nano,       // нано
        Micro,      // микро
        Milli,      // милли
        One,        // единицы
        Kilo,       // кило
        Mega,       // мега
        Number
    } value;
    Order(E v = Number) : value(v)  { };
    operator uint8() const { return (uint8)value; };
    pString Name() const;
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define NUM_DIGITS          5

class Form;
class Wave;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ParameterBase
{
    friend class Form;

public:
    
    enum Type
    {
        Value,
        Exit,
        Choice,
        Complex
    };

    /// Возвращает указатель на форму, параметром которой является
    Form *GetForm() { return form; };
    /// Возвращает адрес родительского параметра
    ParameterBase *GetParent() { return parent; };

    bool IsValue() const { return type == Value; }
private:
    /// Указатель на фрорму, которой принадлежит параметр
    Form *form;

protected:
    ParameterBase(Type t) : type(t), parent(0) {};

    Type type;
    /// Если этот параметр вложенный, то здесь адрес родителя
    ParameterBase *parent;
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ParameterChoice : public ParameterBase
{
public:
    friend class Form;

    enum E
    {
        Polarity,   ///< Полярность импульсов
        Number
    } value;

    ParameterChoice(E v) : ParameterBase(Choice), value(v) {};
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ParameterValue : public ParameterBase
{
public:
    friend class Form;

    enum E
    {
        Frequency,              ///< Частота
        Period,                 ///< Период
        Amplitude,              ///< Амплитуда
        Offset,                 ///< Смещение
        Duration,               ///< Длительность
        DutyRatio,              ///< Скважность
        Phase,                  ///< Сдвиг фазы
        Delay,                  ///< Задержка
        DurationRise,           ///< Длительность нарастания
        DurationFall,           ///< Длительность спада
        DurationStady,          ///< Длительность установившего значения
        DutyFactor,             ///< Коэффициент заполнения
        Manipulation,           ///< Модуляция пилой для синусоиды
        ManipulationDuration,   ///< Длительность импульсов в режиме манипуляции
        ManipulationPeriod,     ///< Период следования импульсов в режиме манипуляции
        PacketPeriod,           ///< Период следования пачек импульсов в пакетном режиме
        PacketNumber,           ///< Число импульсов в пачке пакетного режима
        Exit,                   ///< Этот параметр закрывает открытый параметр
        Number
    } value;

    ParameterValue(int v = Number) : ParameterBase(Value), value((E)v), inNumLockMode(false) {};

    ParameterValue(int v, float _min, float _max, pString buf, int8 pos, Order o, ParameterBase *param[] = 0, 
        int num = 0, int8 hd = NUM_DIGITS - 1, char s = ' ') : ParameterBase(Value),
        value((E)v), order(o), hightLightDigit(hd), posComma(pos), sign(s), min(_min), max(_max), inNumLockMode(false), params(param), numParams(num)
    {
        strcpy(buffer, buf);
    };

    bool Is(ParameterValue::E v) { return value == v; };

    float MinValue() const { return min; };

    float MaxValue() const { return max; };

    float GetValue() const;

    pString GetStringValue() const;

    pString Name() const;
    /// Возвращает true, если параметр содержит несколько параметров
    bool IsComplexParameter() const;

    bool InNumLockMode() const { return inNumLockMode; };

    void SetNumLockMode(bool mode) { inNumLockMode = mode; };
    /// Возвращает true, если является параметром типа e
    bool Is(ParameterValue::E e) const { return value == e; };
    /// Возвращает true, если является непосредственно вводимым значением
    bool IsInputValue() const { return value != Manipulation && value != Exit; };
    /// Возвращает true, если этот параметр - кнопка выхода.
    bool IsExitParameter() const { return value == Exit; };
    /// Возвращает true, если сложный и открыт
    bool IsOpened();

    pString NameUnit(char buffer[10]);
   
    Order order;
    /// Позиция подсвеченного символа
    int8 hightLightDigit;
    /// После какой цифры воводить запятую
    int8 posComma;
    /// В этом буфере хранится знак числа, а затем цифры с точкой
    char buffer[NUM_DIGITS + 1];
    /// Знак числа. ' ' в случае, если число знака не имеет - строго положительное
    char sign;
private:

    float min;

    float max;
    /// Если true, то находимся в режиме клавиатурного ввода (кнопками 1...9)
    bool inNumLockMode;
    /// Здесь находятся дополнительные параметры в случае, если они требуются
    ParameterBase **params;
    /// Число дополнительных параметров. 0, если таковых не имеется
    int numParams;
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Form
{
public:
    enum E
    {
        Sine,           ///< Синус
        RampPlus,       ///< Пила+
        RampMinus,      ///< Пила-
        Meander,        ///< Меандр
        Impulse,        ///< Импульсы
        PacketImpuls,   ///< Пачка импульсов
        Number
    } value;

    Form(E v = Number) : value(v), wave(0), currentParam(0)   { };

    Form(E v, ParameterBase *param[], int numParams, Wave *w);
    /// Возвращает человеческое название формы сигнала
    pString Name(Language lang) const;
    /// Возвращает ссылку на текущий параметр
    ParameterBase *CurrentParameter();
    /// Возвращает количество доступных параметров
    int NumParameters() const;
    /// Возвращает ссылку на i-ый параметр из массива params
    ParameterBase *GetParameter(int i);
    /// Установить текущим следующй параметр
    void SetNextParameter();
    /// Настраивает генератор в соответствии с установленными параметрами
    void TuneGenerator(Chan ch);
    /// Возвращает true, если тип формы сигнала соответствует e
    bool Is(Form::E e) const { return e == value; };
    /// Раскрывает страницу текущего параметра
    void OpenCurrentParameter();
    /// Закрывает открытый параметр, если таковой имеется и возвращает true в этом случае
    bool CloseOpenedParameter();
    /// Возвращает указатель на родительский Wave
    Wave *GetWave() { return wave; };
    /// Возвращает true, если текущий параметр сложный и открыт.
    bool ParameterIsOpened() const;

private:
    /// Находит требуемый параметр. Возвращает 0, если такого параметра нет
    ParameterValue *FindParameter(ParameterValue::E p);
    /// Засыалет параметр в генератор
    void SendParameterToGenerator(ParameterValue::E p);
    /// Wave, к которому относится данный Form
    Wave *wave;
    /// Здесь хранятся параметры
    ParameterBase **params;
    /// Сколько всего параметров
    int numParams;
    /// Номер текущего параметра в массиве params
    int currentParam;

    /// Здесь сохраняется указатель на основные параметры в случае раскрытия сложного параметра
    ParameterBase **oldParams;
    /// Относится к oldParams
    int oldNumParams;
    /// Относится к oldParams
    int oldCurrentParams;
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Wave
{
public:

    Wave(Chan ch, Form *forms, int numForms);
    /// Возвращает установленную форму
    Form *GetCurrentForm();
    /// Установить текущей следующую форму
    void SetNextForm();

    int NumberOfForms() const;

    Form *GetForm(int i);

    Chan GetChannel() const { return channel; };

private:
    /// Какому каналу принадлежит сигнал
    Chan channel;
    /// Текущая форма сигнала - указывает на номер сигнала в массиве
    int currentForm;
    /// Список форм, которые могут быть назначены
    Form *forms;
    /// Количество возможных форм
    int numForms;

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

        static void DrawPacketImpulse(int x, int y, int width, int height);

        static void DrawParameters(Chan chan, int y0);

        static void DrawParameterValue(ParameterBase *parameter, int x, int y);

    };
};
