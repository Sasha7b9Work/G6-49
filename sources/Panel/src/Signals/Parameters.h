// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Common.h"
#include "Signals/Tuner.h"
#include "Signals/Viewer.h"
#include "Signals/MathSupport.h"
#include "Settings/SettingsTypes.h"


class Form;
struct Key;
struct Param;


struct SMinMax
{
    SMinMax() : min(1), max(1), valid(true) { }
    Value min;      // Минимально допустимое значение
    Value max;      // Максимально допустимое значение
    bool valid;     // Если true - текущее значение допустимо
};

typedef SMinMax (*pValueInRange)(Form *);

inline SMinMax EValueInRange(Form *) { return SMinMax(); }


#define DEFAULT_AMPLITUDE Value("1", Order::One)

#define IMPULSE_PERIOD_MIN   Value("20", Order::Nano)
#define IMPULSE_DURATION_MIN Value("10", Order::Nano)


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
    Param(KindParam::E k, pFuncBV funcActive, pchar nRU, pchar nEN);

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
    
    KindParam::E GetKind() { return kind; }

    Viewer viewer;

    // Это "самое максимальное" значение параметра. Больше его быть не может
    virtual Value GetMax() const { return Value("1", Order::One); }

    // Это "самое минимальное" значение параметра. Меньше его быть не может
    virtual Value GetMin() const { return Value("0", Order::One); }

    virtual SMinMax ValueInRange() const { return EValueInRange(form); }

    virtual Value GetValue() const { return Value("1", Order::One); } //-V524

    virtual String ToString(String &) const { return String(""); };

    virtual String ToString(Value) const { return String(""); };

    // Сохранить состояние параметра
    virtual void StoreState() { };

    // Восстановить состояине параметра
    virtual void RestoreState() { };

    static bool FuncActive() { return true; }

    DParam *ReinterpretToDouble() { return (DParam *)this; }

    pFuncBV          funcOfActive;  // Активен ли данный параметр

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
        Amplitude,              // Амплитуда
        Offset,                 // Смещение
        Duration,               // Длительность
        DutyRatio,              // Скважность
        Phase,                  // Сдвиг фазы
        Delay,                  // Задержка между каналами А и В при включённом режиме СТАРТ/СТОП
        DurationRise,           // Длительность нарастания
        DurationFall,           // Длительность спада
        DurationStady,          // Длительность установившего значения
        DutyFactor,             // Коэффициент заполнения
        ManipulationDuration,   // Длительность импульсов в режиме манипуляции
        ManipulationPeriod,     // Период следования импульсов в режиме манипуляции
        PacketPeriod,           // Период следования пачек импульсов в пакетном режиме
        Count
    };
};


struct DParam : public Param
{
    friend struct LogicFloatValue;
    friend class Tuner;
    friend class TunerDisplay;

    DParam(TypeDParam::E t, pFuncBV funcActive, pchar const nameRU, pchar const nameEN,
                    const Value  &min,
                    const Value  &max,
                    pValueInRange valueInRange,
                    const Value  &value);

    DParam(const DParam &);

    virtual void Reset();

    // Установить значение параметра и загрузить его в прибор
    bool SetAndLoadValue(double val);
    bool SetAndLoadValue(Value val);
    
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
    virtual String ToString(Value value) const;

    // Возвращает основные единицы измерения (без учёта порядка)
    cstr GetMainUnits() const;

    // Возвращает единицы измерения, приведённые к порядку order. Если order == Order::Count, единциы будут рассчитываться исходя из текущего значения value
    cstr GetUnits(Order::E order = Order::Count) const;

    Value Max() const { return max; }

    // Возвращает максимальное значение, которое может иметь параметр
    virtual Value GetMax() const { return max; }
    
    // Возвращает минимальное значение, которое может иметь параметр
    virtual Value GetMin() const { return min; }

    // Возвращает стрктуру, которая описывает максимальное и минимальное 
    virtual SMinMax ValueInRange() const { return valueInRange(form); }

    virtual Tuner *GetTuner()   { return &tuner; };

    // Возвращает текущее значение параметра
    virtual Value GetValue() const { return value; };

    void SetValue(Value val) { value = val; }
     
    TypeDParam::E GetType() const { return type; }

    virtual void StoreState();

    virtual void RestoreState();

    DParam &operator=(const DParam &);

    bool IsAmplitude() const { return type == TypeDParam::Amplitude; }

private:
    Tuner tuner;        // Используется для настройки 
    const TypeDParam::E type;
    Value         min;
    Value         max;
    pValueInRange valueInRange;
    Value         value;
    Value         resetValue;

    // Возвращает true, если параметр может принимать значение v
    bool InRange(double v) const;
    bool InRange(Value v) const;

    // Для пакета импульсов после изменения периода или длительности импульсов нужно заслать ещё и число импульсов
    void LoadNumberImpulsesIfNeed();

    struct Stored
    {
        Stored(const Value &val) : value(val) { }
        Value value;
    } stored;
};


struct TypeIParam
{
    enum E
    {
        PacketNumber,           // Число импульсов в пачке пакетного режима
        Count
    };
};


struct IParam : public Param
{
    IParam(TypeIParam::E t, pchar  const nameRU, pchar const nameEN,
        const Value &min, const Value &max, pValueInRange, const Value &);

    virtual void Reset() { SetAndLoadValue(resetValue); }

    virtual String ToString(String &units) const;

    virtual void OnPressButtonTune();

    virtual Tuner *GetTuner()                 { return &tuner; }

    virtual Value GetValue() const            { return value; }

    Value Max() const { return max; }

    Value Min() const { return min; }

    virtual Value GetMax() const              { return max;   }

    virtual Value GetMin() const              { return min;   }

    virtual SMinMax ValueInRange() const { return valueInRange(form); }

    virtual String ToString(Value value) const;

    TypeIParam::E GetType()   { return type; }

    bool SetAndLoadValue(Value val);

    bool SetAndLoadValue(int val);

    void LoadValue();

    bool InRange(Value v) const;

private:

    Tuner tuner;
    TypeIParam::E type;
    Value         min;
    Value         max;
    pValueInRange valueInRange;
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
        ClockImpulse,           // Тактовая частота в режиме импульсов и пакетов
        Count
    };

    static pchar Name(TypeCParam::E type);
};


struct CParam : public Param
{
    CParam(TypeCParam::E t, pFuncBV funcActive, pchar nameRU, pchar nameEN, pchar *_choices = nullptr) :
        Param(KindParam::Choice, funcActive, nameRU, nameEN), type(t), choice(0), choices(_choices) { }

    TypeCParam::E GetType() { return type; }

    int GetChoice() const;

    void NextChoice();

    bool SetAndLoadChoice(int ch);

    virtual String ToString(String &units) const;

    virtual String ToString(Value) const { return String(""); };

    virtual void OnPressButtonTune();

    virtual void Reset() { SetAndLoadChoice(0); };

private:
	TypeCParam::E type;
    int choice;                         // Текущий выбор. И выбор для режима запуска импульсных сигналов
    static int choiceModeStartFree;     // Выбор режима запуска произвольных сигналов
    pchar *choices;               // Идут так - 0(рус), 0(англ), 1(рус), 1(англ)...
    
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


struct CMSParam : public Param
{
    CMSParam(TypeCMSParam::E v, pchar nameRU, pchar nameEN, Param **parameters) :
        Param(KindParam::Composite, Param::FuncActive, nameRU, nameEN), params(parameters), type(v) { }

    virtual void SetForm(Form *form);

    virtual void Reset() { }

    int NumParameters() const;
    Param **Parameters() { return params; }

    DParam *FindParameter(TypeDParam::E p);
    CParam *FindParameter(TypeCParam::E p);

    virtual String ToString(String &units) const;

    virtual String ToString(Value) const { return String(""); };

    virtual void OnPressButtonTune();

    TypeCMSParam::E GetType() { return type; }

private:
    
    Param **params; // Здесь находятся дополнительные параметры в случае, если они требуются
    TypeCMSParam::E type;
};


struct BParam : public Param
{
    BParam(pchar titleRU, pchar titleEN, pFuncVV f) :
        Param(KindParam::Button, Param::FuncActive, titleRU, titleEN), func(f) {};

    virtual String ToString(String &) const { return String(""); };

    virtual String ToString(Value) const { return String(""); };

    virtual void OnPressButtonTune() { func(); };

    virtual void Reset() {}

private:

    pFuncVV func;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Value ///
struct ParameterVoltage : public DParam
{
    ParameterVoltage(TypeDParam::E type, pchar nameRU, pchar nameEN,
                     const Value &min,
                     const Value &max,
                     pValueInRange valueInRange,
                     const Value &value) :
        DParam(type, Param::FuncActive, nameRU, nameEN, min, max, valueInRange, value) { }
};


struct ParameterAmplitude : public ParameterVoltage
{
    ParameterAmplitude(const Value &min = Value("0", Order::One),
                       const Value &max = Value("10", Order::One),
                       pValueInRange valueInRange = EValueInRange,
                       const Value &value = DEFAULT_AMPLITUDE) :
        ParameterVoltage(TypeDParam::Amplitude, "Размах", "Amplitude", min, max, valueInRange, value) { }

    virtual Value GetMax() const;
};


struct ParameterOffset : public ParameterVoltage
{
    ParameterOffset(pValueInRange valueInRange = EValueInRange,
                    const Value &min = Value("-5", Order::One),
                    const Value &max = Value("5", Order::One),
                    const Value &value = Value("0", Order::One)) :
        ParameterVoltage(TypeDParam::Offset, "Смещение", "Offset", min, max, valueInRange, value) { }
};


struct ParameterFrequency : public DParam
{
    ParameterFrequency(const Value &min,
                       const Value &max,
                       pValueInRange valueInRange = EValueInRange,
                       const Value &value = Value("1", Order::Kilo)) :
        DParam(TypeDParam::Frequency, Param::FuncActive, "Частота", "Frequency", min, max, valueInRange, value) { }
};


struct ParameterTime : public DParam
{
    ParameterTime(TypeDParam::E t, pFuncBV funcActive, pchar nameRU, pchar  const nameEN,
                  const Value &min,
                  const Value &max,
                  pValueInRange valueInRange,
                  const Value &value) : DParam(t, funcActive, nameRU, nameEN, min, max, valueInRange, value) { }
};


struct ParameterPhase : public DParam
{
    ParameterPhase() : DParam(TypeDParam::Phase, Param::FuncActive, "Фаза", "Phase",
                                       Value("0", Order::One),
                                       Value("360", Order::One),
                                       EValueInRange,
                                       Value("0", Order::One)) { }
};


struct ParameterPacketPeriod : public ParameterTime
{
    ParameterPacketPeriod(const Value &max, const Value &value) :
        ParameterTime(TypeDParam::PacketPeriod, Param::FuncActive, "Период пак", "Packet per", IMPULSE_PERIOD_MIN, max, EValueInRange, value) { }

    // Если установленное значение не позволяет поместить в себя все импульсы пакета, то его нужно пересчитать
    // Возвращает true, если значение изменилось
    bool RecalcualateValue();

    // Рассчитывает минимально возможное значение при данных параметрах
    Value CalculateMinValue() const;

    virtual Value GetMin() const { return CalculateMinValue(); }
};


struct ParameterPeriod : public ParameterTime
{
    ParameterPeriod(pFuncBV funcActive, const Value &max, const Value &value, pchar nameRU = "Период", pchar  const nameEN = "Period") :
        ParameterTime(TypeDParam::Period, funcActive, nameRU, nameEN, IMPULSE_PERIOD_MIN, max, EValueInRange, value) { }
};


struct ParameterDuration : public ParameterTime
{
    ParameterDuration(const Value &max, const Value &value, pchar nameRU = "Длит", pchar nameEN = "Dur") :
        ParameterTime(TypeDParam::Duration, Param::FuncActive, nameRU, nameEN, IMPULSE_PERIOD_MIN, max, EValueInRange, value) { }
};


struct ParameterDelay : public ParameterTime
{
    ParameterDelay(pFuncBV funcActive, const Value &max, const Value &value, pchar nameRU = "Задержка", pchar nameEN = "Delay") :
        ParameterTime(TypeDParam::Delay, funcActive, nameRU, nameEN, IMPULSE_PERIOD_MIN, max, EValueInRange, value) { }
};


struct ParameterManipulationDuration : public ParameterTime
{
    ParameterManipulationDuration(const Value &min,
                                  const Value &max,
                                  pValueInRange valueInRange,
                                  const Value &value) :
        ParameterTime(TypeDParam::ManipulationDuration, Param::FuncActive, "Длит", "Duration", min, max, valueInRange, value) { }
};


struct ParameterManipulationPeriod : public ParameterTime
{
    ParameterManipulationPeriod(const Value &min,
                                const Value &max,
                                pValueInRange valueInRange,
                                const Value &value) :
        ParameterTime(TypeDParam::ManipulationPeriod, Param::FuncActive, "Период", "Period", min, max, valueInRange, value) { }
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Choice ///
struct ParameterModeStart : public CParam
{
    ParameterModeStart(pFuncBV funcActive, pchar *names) : CParam(TypeCParam::ModeStart, funcActive, "Запуск", "Start", names) { }
};


struct ParameterModeStartStop : public CParam
{
    ParameterModeStartStop(pFuncBV funcActive, pchar *names) : CParam(TypeCParam::ModeStartStop, funcActive, "А-Старт,В-Стоп", "A-Start,B-Stop", names) { }
};


struct ParameterManipulationEnabled : public CParam
{
    ParameterManipulationEnabled(pchar *names) : 
        CParam(TypeCParam::ManipulationEnabled, Param::FuncActive, "Манип", "Manip", names) { }

};


struct ParameterPolarity : public CParam
{
    ParameterPolarity(pchar *names) : CParam(TypeCParam::Polarity, Param::FuncActive, "Полярность", "Polarity", names) { }
};


struct ParameterClockImpulse : public CParam
{
    ParameterClockImpulse(pchar *names) : CParam(TypeCParam::ClockImpulse, Param::FuncActive, "Оп. частота", "Clock", names) { }
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Complex ///
struct ParameterManipulation : public CMSParam
{
    ParameterManipulation(Param **paramaters);
};

