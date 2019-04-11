#pragma once
#include "defines.h"
#include <cstring>
#include <limits>
#include "Common.h"


class ParameterValue;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
    Order& operator++(int);
    Order& operator--(int);
    static Order::E Min(ParameterValue *param);
    static Order::E Max(ParameterValue *param);
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Form;
class Wave;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

    bool IsChoice() const { return type == Choice; }
    
    bool IsComplex() const { return type == Complex; };

    bool IsExit() const;

    pString Name() const;

    pString GetStringValue() const;

    pString NameUnit(char buffer[10]) const;
    /// Возвращает true, если параметру можно изменить порядок
    bool AssumeArbitaryOrder() const;

protected:
    /// Указатель на фрорму, которой принадлежит параметр
    Form *form;

    ParameterBase(Type t) : form(0), type(t), parent(0) {};

    Type type;
    /// Если этот параметр вложенный, то здесь адрес родителя
    ParameterBase *parent;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef void (*funcDraw)(int, int);

class ParameterChoice : public ParameterBase
{
public:
    friend class Form;

    enum E
    {
        Polarity,               ///< Полярность импульсов
        ModeStart,              ///< Режим запуска сигналов DDS и импульсных
        ManipulationMode,       ///< Режим амплитудной манипуляции - со сглаживанием фронта или без
        ManipulationEnabled,    ///< Включен или выключен режим манипуляции
        Number
    } value;

    pString Name() const;

    ParameterChoice(E v, pString var0ru, pString var0en, pString var1ru, pString var1en, funcDraw func0 = 0, funcDraw func1 = 0);

    ParameterChoice(E v, pString var0ru, pString var0en, pString var1ru, pString var1en, 
                         pString var2ru, pString var2en, pString var3ru, pString var3en);

    pString GetStringValue() const;

    void NextChoice();

    int GetChoice() const { return choice; };
    /// Нарисовать текущий вариант в указанных координатах
    bool DrawChoice(int x, int y);

private:
    /// Текущий вариант
    int choice;
    /// Количество вариантов
    int num;
    /// Варианты на русском языке
    char *names[4][2];

    funcDraw func[4];
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ParameterComplex : public ParameterBase
{
    friend class Form;
public:

    enum E
    {
        Manipulation,
        Number
    } value;

    ParameterComplex(E v, ParameterBase **param);

    pString Name() const;
    
    pString GetStringValue() const;

    bool Is(E v) const { return value == v; };

private:
    /// Здесь находятся дополнительные параметры в случае, если они требуются
    ParameterBase **params;
    /// Число дополнительных параметров. 0, если таковых не имеется
    int numParams;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ParameterValue : public ParameterBase
{
private:
    static int const NUM_DIGITS = 5;
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
        ManipulationDuration,   ///< Длительность импульсов в режиме манипуляции
        ManipulationPeriod,     ///< Период следования импульсов в режиме манипуляции
        PacketPeriod,           ///< Период следования пачек импульсов в пакетном режиме
        PacketNumber,           ///< Число импульсов в пачке пакетного режима
        Exit,                   ///< Этот параметр закрывает открытый параметр
        Number
    } value;

    ParameterValue(int v = Number);

    ParameterValue(int v, float _min, float _max, pString buf, int8 pos, Order o, int8 hd = NUM_DIGITS - 1, char s = ' ');

    bool Is(ParameterValue::E v) { return value == v; };

    float MinValue() const { return min; };

    float MaxValue() const { return max; };

    ParamValue GetValueNano() const;

    pString GetStringValue() const;
    /// Возвращает true, если порядок величины параметра может быть разным
    bool AssumeArbitaryOrder() const;
    /// Увеличить порядок
    void IncreaseOrder();
    /// Уменьшить порядок
    void DecreaseOrder();

    pString Name() const;
    /// Возвращает true, если является параметром типа e
    bool Is(ParameterValue::E e) const { return value == e; };
    /// Возвращает true, если этот параметр - кнопка выхода.
    bool IsExit() const { return value == Exit; };

    bool IsInputValue() const { return IsValue() && value != Exit; };
    /// Возвращает true, если сложный и открыт
    bool IsOpened();

    pString NameUnit(char buffer[10]) const;
   
    Order order;
    /// Позиция подсвеченного символа
    int8 hightLightDigit;
    /// После какой цифры воводить запятую
    int8 posComma;
    /// В этом буфере хранится знак числа, а затем цифры с точкой
    char buffer[NUM_DIGITS + 1];
    /// Знак числа. ' ' в случае, если число знака не имеет - строго положительное
    char sign;

    int numDigits;
private:

    float min;

    float max;
    /// Если true, то находимся в режиме клавиатурного ввода (кнопками 1...9)
    bool inNumLockMode;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Form
{
    friend class Wave;

public:
    enum E
    {
        Sine,           ///< Синус
        RampPlus,       ///< Пила+
        RampMinus,      ///< Пила-
        Triangle,       ///< Треугольник
        Meander,        ///< Меандр
        Impulse,        ///< Импульсы
        PacketImpuls,   ///< Пачка импульсов
        DDS,            ///< Произвольный сигнал
        Number
    } value;

    Form(E v = Number) :
        value(v),
        wave(0),
		params(nullptr),
		numParams(0),
        currentParam(0),
        oldParams(0),
        oldNumParams(0),
        oldCurrentParams(0)
    { };

    Form(E v, ParameterBase **param, Wave *w);
    /// Возвращает человеческое название формы сигнала
    pString Name(Language::E lang) const;
    /// Возвращает ссылку на текущий параметр
    ParameterBase *CurrentParameter();
    /// Возвращает количество доступных параметров
    int NumParameters() const;
    /// Возвращает ссылку на i-ый параметр из массива params
    ParameterBase *GetParameter(int i);
    /// Установить текущим следующй параметр
    void SetNextParameter();
    /// Настраивает генератор в соответствии с установленными параметрами
    void TuneGenerator(Chan::E ch);
    /// Возвращает true, если тип формы сигнала соответствует e
    bool Is(Form::E e) const { return e == value; };
    /// Возвращает true, если форма сигнала реализуется с помощью ПЛИС
    bool IsDDS() const;
    /// Возвращает указатель на родительский Wave
    Wave *GetWave() { return wave; };
    /// Возвращает true, если текущий параметр сложный и открыт.
    bool ParameterIsOpened() const;

    ParameterValue *GetParameterValue(ParameterValue::E value);
    /// Функция вызывается при нажатии кнопки "Изменить параметр". Фактически активизирует текущий параметр
    void ChangeParameter();
    /// Закрывает открытый параметр, если таковой имеется и возвращает true в этом случае
    bool CloseOpenedParameter();
    /// Нарисовать изображение сигнала
    void DrawUGO(Chan::E ch, int y0);
    /// Установить данные произвольного сигнала для отрисовки
    static void SetFormFlash(Chan::E ch, const uint8 data[300]);
    /// Возвращает указатель на данные произвольного сигнала для отрисовки
    static uint8 *GetFormFlash(Chan::E ch);
    /// Получить смещение сигнала
    float GetOffset();
    /// Получить размах сигнала
    float GetAmplitude();

private:
    /// Находит требуемый параметр. Возвращает 0, если такого параметра нет
    ParameterBase *FindParameter(ParameterValue::E p);
    ParameterBase *FindParameter(ParameterChoice::E p);
    /// Засыалет параметр в генератор
    void SendParameterToGenerator(ParameterValue::E p);
    void SendParameterToGenerator(ParameterChoice::E p);
    /// Раскрывает страницу текущего параметра
    void OpenCurrentParameter();
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

    static void DrawSine(Chan::E ch, int x, int y, int width, int height);

    static void DrawRampPlus(Chan::E ch, int x, int y, int width, int height);

    static void DrawRampMinus(Chan::E ch, int x, int y, int width, int height);

    static void DrawTriangle(Chan::E ch, int x, int y, int width, int height);

    static void DrawMeander(Chan::E ch, int x, int y, int width, int height);

    static void DrawImpulse(Chan::E ch, int x, int y, int width, int height);

    static void DrawPacketImpulse(Chan::E ch, int x, int y, int width, int height);

    static void DrawDDS(Chan::E ch, int x, int y, int width, int height);


};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Wave
{
public:

    Wave(Chan::E ch, Form **forms);
    /// Возвращает установленную форму
    Form *GetCurrentForm();
    /// Установить текущей следующую форму
    void SetForm(int8 num);

    int NumberOfForms() const;

    Form *GetForm(int i);

    Form *GetForm(Form::E form);

    Chan::E GetChannel() const { return channel; };
    /// Возвращает true, если установлен ручной режим запуска
    bool StartModeIsSingle();

private:
    /// Какому каналу принадлежит сигнал
    Chan::E channel;
    /// Текущая форма сигнала - указывает на номер сигнала в массиве
    int currentForm;
    /// Список форм, которые могут быть назначены
    Form **forms;
    /// Количество возможных форм
    int numForms;

public:

    class Graphics
    {
    public:
        /// Нарисовать информацию о параметрах сигнала на экране
        static void Draw(Chan::E ch);
        /// Позиция x области отрисовки сигнала
        static int X();
        /// Позиция y области отрисовки сигнала
        static int Y(Chan::E ch);
        /// Ширина области отрисовки сигнала
        static int Width();
        /// Высота области отрисовки сигнала
        static int Height();
    private:

        static void DrawParameters(Chan::E chan, int y0);

        static void DrawParameterValue(ParameterBase *parameter, int x, int y);
    };
};
