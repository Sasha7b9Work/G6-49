// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Common.h"
#include "Signals/Tuner.h"
#include "Signals/Viewer.h"
#include "Settings/SettingsTypes.h"


class Form;
struct Key;
struct Param;


typedef bool (*pFuncActive)();


struct SMinMax
{
    SMinMax(bool val) : min(1), max(1), valid(val) { }
    SMinMax(const Value &_min, const Value &_max, const Value &_value) : min(_min), max(_max)
    {
        valid = (_value >= min) && (_value <= max);
    }

    Value min;      // Минимально допустимое значение
    Value max;      // Максимально допустимое значение
    bool valid;     // Если true - текущее значение допустимо
};


struct KindParam
{
    enum E
    {
        Double,     // Величина, выраженная числовым значением
        Choice,     // Выбор из нескольких значений
        Composite,  // Составной параметр, состоящий из нескольких простых (манипуляция)
        Button,     // Кнопка - по её нажатию происходит какое-либо действие
        Integer,    // Целое число
        Count
    };
};


struct Param
{
    Param(KindParam::E, pFuncActive, pchar nRU, pchar nEN);

    virtual ~Param() { }

    cstr Name() const;

    virtual void SetForm(Form *form);

    // Обработчик нажатия кнопки "Изменить"
    virtual void OnPressButtonTune() = 0;

    virtual Tuner *GetTuner() { return nullptr; }

    void SetParent(Param *p);

    // Возвращает адрес родительского параметра
    Param *GetParent();

    bool IsDouble() const;
    bool IsInteger() const;
    bool IsComposite() const;
    bool IsChoice() const;

    virtual void Reset() = 0;

    // Возвращает true, если параметр сложный и открыт
    bool IsOpened() const;

    Form *GetForm();

    Form *GetForm() const { return form; };

    KindParam::E GetKind() const { return kind; }

    Viewer viewer;

    // Это "самое максимальное" значение параметра. Больше его быть не может
    virtual Value Max() const { return Value("1", Order::One); }

    // Это "самое минимальное" значение параметра. Меньше его быть не может
    virtual Value Min() const { return Value("0", Order::One); }

    virtual SMinMax ValueInRange() const;

    virtual Value GetValue() const { return Value("1", Order::One); } //-V524

    virtual String ToString(String &units) const { return units; };

    // delete_zeros - если true - удалять последние нули
    virtual String ToString(Value, bool /* delete_zeros */ = false) const { return String(""); };

    // Сохранить состояние параметра
    virtual void StoreState() { };

    // Восстановить состояине параметра
    virtual void RestoreState() { };

    static bool EFuncActive() { return true; }

    DParam *ToDouble();
    IParam *ToInteger();

    pFuncActive   funcOfActive;  // Активен ли данный параметр

protected:

    Form         *form;         // Форма, для которой зада этот параметр
    Param        *parent;       // Если параметр вложенный, то здесь адрес родителя
    KindParam::E  kind;
    pchar         names[2];
};


struct TypeDParam
{
    enum E
    {
        Frequency,              // Частота
        Period,                 // Период
        AmplitudePic,           // Размах
        Offset,                 // Смещение
        Duration,               // Длительность
        Phase,                  // Сдвиг фазы
        Delay,                  // Задержка между каналами А и В при включённом режиме СТАРТ/СТОП
        DurationManipulation,   // Длительность импульсов в режиме манипуляции
        PeriodManipulation,     // Период следования импульсов в режиме манипуляции
        PeriodPacket,           // Период следования пачек импульсов в пакетном режиме
        Count
    };
};


// Double
struct DParam : public Param
{
    friend struct LogicFloatValue;
    friend class Tuner;
    friend class TunerDisplay;

    DParam(TypeDParam::E t, pFuncActive, pchar const nameRU, pchar const nameEN, const Value &);

    DParam(const DParam &);

    virtual void Reset();

    // Установить значение параметра и загрузить его в прибор
    bool SetAndLoadToGenerator(double);
    bool SetAndLoadToGenerator(const Value &);

    // Возвращает true, если параметр имеет знак
    bool IsSigned() const { return (type == TypeDParam::Offset); }

    // Возвращает true, если параметр не может изменять размерность
    bool IsNotOrdered() const;

    bool IsPhase() const;

    // Возвращает true, если параметр имеетр размерность "секунда"
    bool IsTime() const;

    virtual void OnPressButtonTune();

    // Возвращает строковое представление значения параметра
    virtual String ToString(String &units) const;
    virtual String ToString(Value value, bool delete_zeros = false) const;

    // Возвращает основные единицы измерения (без учёта порядка)
    cstr GetMainUnits() const;

    // Возвращает единицы измерения, приведённые к порядку order. Если order == Order::Count, единциы будут рассчитываться исходя из текущего значения value
    cstr GetUnits(Order::E order = Order::Count) const;

    virtual Value Min() const { return Value(-1); };
    virtual Value Max() const { return Value(1); };
    virtual Value GetValue() const { return value; };

    virtual Tuner *GetTuner() { return &tuner; };

    void SetValue(Value val) { value = val; }

    TypeDParam::E GetType() const { return type; }

    virtual void StoreState();

    virtual void RestoreState();

    DParam &operator=(const DParam &);

    bool IsAmplitude() const { return type == TypeDParam::AmplitudePic; }
    bool IsFrequency() const { return type == TypeDParam::Frequency; }
    bool IsOffset()    const { return type == TypeDParam::Offset; }

    // Возвращает количество значащих знакомест перед запятой (исключая знаковый разряд). В предположении, что значение приведено к порядку order
    int GetNumberDigitsBeforeComma(Order::E order = Order::Count) const;

    // Возвращает количество значащих знакомест после запятой
    int GetNumberDigitsAfterComma(Order::E order = Order::Count) const;

    // Возвращает числовое представление Value с numDigits значащими цифрами и запятой.
    cstr GetIndicatedValue() const;

    // Возвращает позицию первого ненулевого символа "1" - десятки (1e1), "0" - единицы (1e0), "-1" - десятые (1e-1), "-2" - сотые (1e-2)
    int GetPositionFirstDigit(Order::E order = Order::Count) const;

    static DParam empty;

    bool Exist() const { return this != &empty; }

private:
    Tuner               tuner;          // Используется для настройки 
    const TypeDParam::E type;
    Value               value;
    Value               resetValue;

    // Для пакета импульсов после изменения периода или длительности импульсов нужно заслать ещё и число импульсов
    void LoadNumberImpulsesIfNeed();

    struct Stored
    {
        Stored(const Value &val) : value(val) { }
        Value value;
    } stored;

public:

    // Это значение используется при уменьшении значения до нуля на экране коррекции
    struct PrevOrder
    {
        PrevOrder() : order(Order::One) {}
        void Set(Order::E _order) { order = _order; }
        Order::E Get()            { return order; };
    private:
        Order::E order;
    } prev_order;
};


struct TypeIParam
{
    enum E
    {
        PacketNumber,           // Число импульсов в пачке пакетного режима
        Count
    };
};


// Integer
struct IParam : public Param
{
    IParam(TypeIParam::E t, pchar  const nameRU, pchar const nameEN, const Value &);

    virtual void Reset() { SetAndLoadToGenerator(resetValue); }

    virtual String ToString(String &units) const;
    virtual String ToString(Value value, bool delete_zeros = false) const;

    virtual void OnPressButtonTune();

    virtual Tuner *GetTuner() { return &tuner; }

    virtual Value Min() const;
    virtual Value Max() const;
    virtual Value GetValue() const { return value; }

    TypeIParam::E GetType() const { return type; }

    bool SetAndLoadToGenerator(const Value &val);

    bool SetAndLoadToGenerator(const int val);

    void LoadToGenerator() const;

    // Возвращает количество разрядов для параметра parameter (в них должно помещаться максимально возможное значение параметра parameter)
    int GetMaxNumberDigits() const;

    static IParam empty;

    bool Exist() const { return this != &empty; }

private:

    Tuner         tuner;
    TypeIParam::E type;
    Value         value;
    Value         resetValue;
};


struct TypeCParam
{
    enum E
    {
        Polarity,               // Полярность импульсов
        ModeStart,              // Режим запуска сигналов DDS и импульсных
        ManipulationEnabled,    // Включен или выключен режим манипуляции
        ModeStartStop,          // Режим А-Старт/В-Стоп
        Count
    };

    static pchar Name(TypeCParam::E type);
};


// Choice
struct CParam : public Param
{
    CParam(TypeCParam::E t, pFuncActive funcActive, pchar nameRU, pchar nameEN, pchar *_choices = nullptr) :
        Param(KindParam::Choice, funcActive, nameRU, nameEN), type(t), choice(0), choices(_choices) { }

    TypeCParam::E GetType() const { return type; }

    int GetChoice() const;

    void NextChoice();

    bool SetAndLoadChoice(int ch);

    virtual String ToString(String &units) const;
    virtual String ToString(Value, bool /*delete_zeros*/ = false) const { return String(""); };

    virtual void OnPressButtonTune();

    virtual void Reset() { SetAndLoadChoice(0); };

    static CParam empty;

    bool Exist() const { return this != &empty; }

private:
    TypeCParam::E  type;
    int            choice;                  // Текущий выбор. И выбор для режима запуска импульсных сигналов
    static int     choiceModeStartFree;     // Выбор режима запуска произвольных сигналов
    pchar         *choices;                 // Идут так - 0(рус), 0(англ), 1(рус), 1(англ)...

    // Количество вариантов выбора
    int NumChoices() const;
};


struct TypeCMSParam
{
    enum E
    {
        Manipulation,   // НАСТРОЙКИ СИГНАЛОВ / Параметр / МАНИПУЛЯЦИЯ на форме СИНУС
        Count
    };
};

// Composite
struct CMSParam : public Param
{
    CMSParam(TypeCMSParam::E v, pchar nameRU, pchar nameEN, Param **parameters) :
        Param(KindParam::Composite, Param::EFuncActive, nameRU, nameEN), params(parameters), type(v) { }

    virtual void SetForm(Form *form);

    virtual void Reset() { }

    int NumParameters() const;
    Param **Parameters() { return params; }

    DParam *FindParameter(TypeDParam::E p);
    CParam *FindParameter(TypeCParam::E p);

    virtual String ToString(String &units) const;
    virtual String ToString(Value, bool /*delete_zeros*/ = false) const { return String(""); };

    virtual void OnPressButtonTune();

    TypeCMSParam::E GetType() { return type; }

    static CMSParam empty;

    bool Exist() const { return this != &empty; }

private:

    Param **params; // Здесь находятся дополнительные параметры в случае, если они требуются
    TypeCMSParam::E type;
};


// Button
struct BParam : public Param
{
    BParam(pchar titleRU, pchar titleEN, pFuncVV f) :
        Param(KindParam::Button, Param::EFuncActive, titleRU, titleEN), func(f) {};

    virtual String ToString(String &) const { return String(""); };
    virtual String ToString(Value, bool /*delete_zeros*/ = false) const { return String(""); };

    virtual void OnPressButtonTune() { func(); };

    virtual void Reset() {}

private:

    pFuncVV func;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Value ///
struct PVoltage : public DParam
{
    PVoltage(TypeDParam::E type, pchar nameRU, pchar nameEN, const Value &value) :
        DParam(type, Param::EFuncActive, nameRU, nameEN, value) { }
};


struct PAmplitudePic : public PVoltage
{
    PAmplitudePic(const Value &value = PAmplitudePic::by_default) : PVoltage(TypeDParam::AmplitudePic, "Размах", "Amplitude", value) { }

    static const Value by_default;

    virtual Value Min() const;
    virtual Value Max() const;
};


struct POffset : public PVoltage
{
    POffset(const Value &value = Value("0", Order::One)) : PVoltage(TypeDParam::Offset, "Смещение", "Offset", value) { }

    // Возвращает возможный максимум без учёта знака
    virtual Value AbsLimit() const;

    virtual Value Min() const;
    virtual Value Max() const;
};


struct PFrequency : public DParam
{
    PFrequency(const Value &value = Value("1", Order::Kilo)) : DParam(TypeDParam::Frequency, Param::EFuncActive, "Частота", "Frequency", value) { }

    static const Value min_sin;
    static const Value max_sin;
    static const Value min_DDS;
    static const Value max_DDS;

    virtual Value Min() const;
    virtual Value Max() const;
};


struct PTime : public DParam
{
    PTime(TypeDParam::E t, pFuncActive funcActive, pchar nameRU, pchar  const nameEN, const Value &value) :
        DParam(t, funcActive, nameRU, nameEN, value) { }
};


struct PPhase : public DParam
{
    PPhase() : DParam(TypeDParam::Phase, Param::EFuncActive, "Фаза", "Phase", Value("0", Order::One)) { }

    virtual Value Min() const;
    virtual Value Max() const;
};


struct PPeriod : public PTime
{
    PPeriod(pFuncActive funcActive, const Value &value, pchar nameRU = "Период", pchar  const nameEN = "Period") :
        PTime(TypeDParam::Period, funcActive, nameRU, nameEN, value) { }

    static const Value min_impulse;

    virtual Value Min() const;
    virtual Value Max() const;
};


struct PPeriodPacket : public PTime
{
    PPeriodPacket(pFuncActive funcActive, const Value &value) : PTime(TypeDParam::PeriodPacket, funcActive, "Период пак", "Packet per", value) { }

    // Если установленное значение не позволяет поместить в себя все импульсы пакета, то его нужно пересчитать
    // Возвращает true, если значение изменилось
    bool RecalcualateValue();

    virtual Value Min() const;
    virtual Value Max() const;
};


struct PDuration : public PTime
{
    PDuration(const Value &value, pchar nameRU = "Длит", pchar nameEN = "Dur") : PTime(TypeDParam::Duration, Param::EFuncActive, nameRU, nameEN, value) { }

    virtual Value Min() const;
    virtual Value Max() const;
};


struct PDelay : public PTime
{
    PDelay(pFuncActive funcActive, const Value &value, pchar nameRU = "Задержка", pchar nameEN = "Delay") : PTime(TypeDParam::Delay, funcActive, nameRU, nameEN, value) { }

    virtual Value Min() const;
    virtual Value Max() const;
};


struct PDurationManipulation : public PTime
{
    PDurationManipulation(const Value &value) : PTime(TypeDParam::DurationManipulation, Param::EFuncActive, "Длит", "Duration", value) { }

    virtual Value Min() const;
    virtual Value Max() const;
};


struct PPeriodManipulation : public PTime
{
    PPeriodManipulation(const Value &value) : PTime(TypeDParam::PeriodManipulation, Param::EFuncActive, "Период", "Period", value) { }

    virtual Value Min() const;
    virtual Value Max() const;
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Choice ///
struct PModeStart : public CParam
{
    PModeStart(pFuncActive funcActive, pchar *names) : CParam(TypeCParam::ModeStart, funcActive, "Запуск", "Start", names) { }
};


struct PModeStartStop : public CParam
{
    PModeStartStop(pFuncActive funcActive, pchar *names) : CParam(TypeCParam::ModeStartStop, funcActive, "А-Старт,В-Стоп", "A-Start,B-Stop", names) { }
};


struct PManipulationEnabled : public CParam
{
    PManipulationEnabled(pchar *names) :
        CParam(TypeCParam::ManipulationEnabled, Param::EFuncActive, "Манип", "Manip", names) { }

};


struct PPolarity : public CParam
{
    PPolarity(pchar *names) : CParam(TypeCParam::Polarity, Param::EFuncActive, "Полярность", "Polarity", names) { }
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Complex ///
struct PManipulation : public CMSParam
{
    PManipulation(Param **paramaters);
};

