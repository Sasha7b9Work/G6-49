// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Common.h"
#include "Signals/Tuner.h"
#include "Signals/Viewer.h"
#include "Signals/MathSupport.h"
#include "Settings/SettingsTypes.h"


inline bool FuncMin() { return true; }
inline bool FuncMax() { return true; }


#define DEFAULT_AMPLITUDE Value("1", Order::One)

#define IMPULSE_PERIOD_MIN   Value("20", Order::Nano)
#define IMPULSE_DURATION_MIN Value("10", Order::Nano)


class Form;
struct Key;
struct Parameter;


struct ParameterKind
{
    enum E
    {
        Double,     // Величина, выраженная числовым значением
        Choice,     // Выбор из нескольких значений
        Composite,  // Составной параметр, состоящий из нескольких простых (манипуляция)
        Button,     // Кнопка - по её нажатию происходит какое-либо действие
        Integer,
        Count
    };
};


struct Parameter
{
    Parameter(ParameterKind::E k, pFuncBV funcActive, pchar nRU, pchar nEN);

    virtual ~Parameter() { }

    pString Name() const;

    virtual void SetForm(Form *form);

    // Обработчик нажатия кнопки "Изменить"
    virtual void OnPressButtonTune() = 0;

    virtual Tuner *GetTuner() { return nullptr; }

    void SetParent(Parameter *p);
    
    // Возвращает адрес родительского параметра
    Parameter *GetParent();

    bool IsDouble() const;
    bool IsInteger() const;
    bool IsComposite() const;
    bool IsChoice() const;

    virtual void Reset() = 0;

    // Возвращает true, если параметр сложный и открыт
    bool IsOpened() const;

    Form *GetForm();
    
    ParameterKind::E GetKind() { return kind; }

    Viewer viewer;

    virtual Value GetMax() const { return Value("1", Order::One); }

    virtual Value GetMin() const { return Value("0", Order::One); }

    virtual Value GetValue() const { return Value("1", Order::One); } //-V524

    virtual String ToString(String &) const { return String(""); };

    virtual String ToString(Value) const { return String(""); };

    // Сохранить состояние параметра
    virtual void StoreState() { };

    // Восстановить состояине параметра
    virtual void RestoreState() { };

    static bool FuncActive() { return true; }

    pFuncBV              funcOfActive;  // Активен ли данный параметр

protected:
    
    Form                *form;          // Форма, для которой зада этот параметр
    Parameter           *parent;        // Если параметр вложенный, то здесь адрес родителя
    ParameterKind::E     kind;
    pchar                names[2];
};


struct ParameterDoubleType
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


struct ParameterDouble : public Parameter
{
    friend struct LogicFloatValue;
    friend class Tuner;
    friend class TunerDisplay;

    ParameterDouble(ParameterDoubleType::E t, pFuncBV funcActive, pchar const nameRU, pchar const nameEN,
                    const Value &min,
                    const Value &max,
                    pFuncBV funcMin,
                    pFuncBV funcMax,
                    const Value &value);

    ParameterDouble(const ParameterDouble &);

    virtual void Reset();

    // Установить значение параметра и загрузить его в прибор
    bool SetAndLoadValue(double val);
    bool SetAndLoadValue(Value val);
    
    // Возвращает true, если параметр имеет знак
    bool IsSigned() const { return (type == ParameterDoubleType::Offset); }

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
    pString GetMainUnits() const;

    // Возвращает единицы измерения, приведённые к порядку order. Если order == Order::Count, единциы будут рассчитываться исходя из текущего значения value
    pString GetUnits(Order::E order = Order::Count) const;

    // Возвращает максимальное значение, которое может иметь параметр
    virtual Value GetMax() const { return max; }
    
    // Возвращает минимальное значение, которое может иметь параметр
    virtual Value GetMin() const { return min; }

    virtual Tuner *GetTuner()   { return &tuner; };

    // Возвращает текущее значение параметра
    virtual Value GetValue() const { return value; };

    void SetValue(Value val) { value = val; }
     
    ParameterDoubleType::E GetType() const { return type; }

    virtual void StoreState();

    virtual void RestoreState();

    ParameterDouble &operator=(const ParameterDouble &);

private:
    Tuner tuner;        // Используется для настройки 
    const ParameterDoubleType::E type;
    Value min;
    pFuncBV funcMin;
    pFuncBV funcMax;
    Value value;
    Value resetValue;

    // Возвращает true, если параметр может принимать значение v
    bool InRange(double v) const;
    bool InRange(Value v) const;

    // Для пакета импульсов после изменения периода или длительности импульсов нужно заслать ещё и число импульсов
    void LoadNumberImpulsesIfNeed();

protected:
    Value max;

private:

    struct Stored
    {
        Stored(const Value &val) : value(val) { }
        Value value;
    } stored;
};


struct ParameterIntegerType
{
    enum E
    {
        PacketNumber,           // Число импульсов в пачке пакетного режима
        Count
    };
};


struct ParameterInteger : public Parameter
{
    ParameterInteger(ParameterIntegerType::E t, pchar  const nameRU, pchar  const nameEN, const Value &min, const Value &max, const Value &value);

    virtual void Reset() { SetAndLoadValue(resetValue); }

    virtual String ToString(String &units) const;

    virtual void OnPressButtonTune();

    virtual Tuner *GetTuner()  { return &tuner; }

    virtual Value GetValue() const { return value; }
                               
    virtual Value GetMax() const         { return max;   }
                               
    virtual Value GetMin() const         { return min;   }

    virtual String ToString(Value value) const;

    ParameterIntegerType::E GetType() { return type; }

    bool SetAndLoadValue(Value val);

    bool SetAndLoadValue(int val);

    void LoadValue();

    bool InRange(Value v) const;

private:

    Tuner tuner;
    ParameterIntegerType::E type;
    Value min;
    Value max;
    Value value;
    Value resetValue;
};


struct ParameterChoiceType
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

    static pchar Name(ParameterChoiceType::E type);
};


struct ParameterChoice : public Parameter
{
    ParameterChoice(ParameterChoiceType::E t, pFuncBV funcActive, pchar nameRU, pchar nameEN, pchar *_choices = nullptr) :
        Parameter(ParameterKind::Choice, funcActive, nameRU, nameEN), type(t), choice(0), choices(_choices) { }

    ParameterChoiceType::E GetType() { return type; }

    int GetChoice() const;

    void NextChoice();

    bool SetAndLoadChoice(int ch);

    virtual String ToString(String &units) const;

    virtual String ToString(Value) const { return String(""); };

    virtual void OnPressButtonTune();

    virtual void Reset() { SetAndLoadChoice(0); };

private:
	ParameterChoiceType::E type;
    int choice;                         // Текущий выбор. И выбор для режима запуска импульсных сигналов
    static int choiceModeStartFree;     // Выбор режима запуска произвольных сигналов
    pchar *choices;               // Идут так - 0(рус), 0(англ), 1(рус), 1(англ)...
    
    // Количество вариантов выбора
    int NumChoices() const;
};


struct ParameterCompositeType
{
    enum E
    {
        Manipulation,   // НАСТРОЙКИ СИГНАЛОВ / Параметр / МАНИПУЛЯЦИЯ на форме СИНУС
        Count
    };
};


struct ParameterComposite : public Parameter
{
    ParameterComposite(ParameterCompositeType::E v, pchar nameRU, pchar nameEN, Parameter **parameters) :
        Parameter(ParameterKind::Composite, Parameter::FuncActive, nameRU, nameEN), params(parameters), type(v) { }

    virtual void SetForm(Form *form);

    virtual void Reset() { }

    int NumParameters() const;
    Parameter **Parameters() { return params; }

    ParameterDouble *FindParameter(ParameterDoubleType::E p);
    ParameterChoice *FindParameter(ParameterChoiceType::E p);

    virtual String ToString(String &units) const;

    virtual String ToString(Value) const { return String(""); };

    virtual void OnPressButtonTune();

    ParameterCompositeType::E GetType() { return type; }

private:
    
    Parameter **params; // Здесь находятся дополнительные параметры в случае, если они требуются
    ParameterCompositeType::E type;
};


struct ParameterButton : public Parameter
{
    ParameterButton(pchar titleRU, pchar titleEN, pFuncVV f) :
        Parameter(ParameterKind::Button, Parameter::FuncActive, titleRU, titleEN), func(f) {};

    virtual String ToString(String &) const { return String(""); };

    virtual String ToString(Value) const { return String(""); };

    virtual void OnPressButtonTune() { func(); };

    virtual void Reset() {}

private:

    pFuncVV func;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Value ///
struct ParameterVoltage : public ParameterDouble
{
    ParameterVoltage(ParameterDoubleType::E type, pchar nameRU, pchar nameEN,
                     const Value &min,
                     const Value &max,
                     pFuncBV funcMin,
                     pFuncBV funcMax,
                     const Value &value) :
        ParameterDouble(type, Parameter::FuncActive, nameRU, nameEN, min, max, funcMin, funcMax, value) { }
};


struct ParameterAmplitude : public ParameterVoltage
{
    ParameterAmplitude(const Value &min = Value("0", Order::One),
                       const Value &max = Value("10", Order::One),
                       const Value &value = DEFAULT_AMPLITUDE,
                       pFuncBV funcMin = FuncMin,
                       pFuncBV funcMax = FuncMax) :
        ParameterVoltage(ParameterDoubleType::Amplitude, "Размах", "Amplitude", min, max, funcMin, funcMax, value) { }

    virtual Value GetMax() const;
};


struct ParameterOffset : public ParameterVoltage
{
    ParameterOffset(const Value &min = Value("-5", Order::One),
                    const Value &max = Value("5", Order::One),
                    pFuncBV funcMin = FuncMin,
                    pFuncBV funcMax = FuncMax,
                    const Value &value = Value("0", Order::One)
                    ) :
        ParameterVoltage(ParameterDoubleType::Offset, "Смещение", "Offset", min, max, funcMin, funcMax, value) { }

    virtual Value GetMax() const;
    virtual Value GetMin() const;
};


struct ParameterFrequency : public ParameterDouble
{
    ParameterFrequency(const Value &min,
                       const Value &max,
                       pFuncBV funcMin = FuncMin,
                       pFuncBV funcMax = FuncMax,
                       const Value &value = Value("1", Order::Kilo)) :
        ParameterDouble(ParameterDoubleType::Frequency, Parameter::FuncActive, "Частота", "Frequency", min, max, funcMin, funcMax, value) { }
};


struct ParameterTime : public ParameterDouble
{
    ParameterTime(ParameterDoubleType::E t, pFuncBV funcActive, pchar nameRU, pchar  const nameEN,
                  const Value &min,
                  const Value &max,
                  pFuncBV funcMin,
                  pFuncBV funcMax,
                  const Value &value) : ParameterDouble(t, funcActive, nameRU, nameEN, min, max, funcMin, funcMax, value) { }
};


struct ParameterPhase : public ParameterDouble
{
    ParameterPhase() : ParameterDouble(ParameterDoubleType::Phase, Parameter::FuncActive, "Фаза", "Phase",
                                       Value("0", Order::One),
                                       Value("360", Order::One),
                                       FuncMin,
                                       FuncMax,
                                       Value("0", Order::One)) { }
};


struct ParameterPacketPeriod : public ParameterTime
{
    ParameterPacketPeriod(const Value &max, const Value &value) :
        ParameterTime(ParameterDoubleType::PacketPeriod, Parameter::FuncActive, "Период пак", "Packet per", IMPULSE_PERIOD_MIN, max, FuncMin, FuncMax, value) { }

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
        ParameterTime(ParameterDoubleType::Period, funcActive, nameRU, nameEN, IMPULSE_PERIOD_MIN, max, FuncMin, FuncMax, value) { }
};


struct ParameterDuration : public ParameterTime
{
    ParameterDuration(const Value &max, const Value &value, pchar nameRU = "Длит", pchar nameEN = "Dur") :
        ParameterTime(ParameterDoubleType::Duration, Parameter::FuncActive, nameRU, nameEN, IMPULSE_PERIOD_MIN, max, FuncMin, FuncMax, value) { }
};


struct ParameterDelay : public ParameterTime
{
    ParameterDelay(pFuncBV funcActive, const Value &max, const Value &value, pchar nameRU = "Задержка", pchar nameEN = "Delay") :
        ParameterTime(ParameterDoubleType::Delay, funcActive, nameRU, nameEN, IMPULSE_PERIOD_MIN, max, FuncMin, FuncMax, value) { }
};


struct ParameterManipulationDuration : public ParameterTime
{
    ParameterManipulationDuration(const Value &min,
                                  const Value &max,
                                  pFuncBV funcMin,
                                  pFuncBV funcMax,
                                  const Value &value) :
        ParameterTime(ParameterDoubleType::ManipulationDuration, Parameter::FuncActive, "Длит", "Duration", min, max, funcMin, funcMax, value) { }
};


struct ParameterManipulationPeriod : public ParameterTime
{
    ParameterManipulationPeriod(const Value &min,
                                const Value &max,
                                pFuncBV funcMin,
                                pFuncBV funcMax,
                                const Value &value) :
        ParameterTime(ParameterDoubleType::ManipulationPeriod, Parameter::FuncActive, "Период", "Period", min, max, funcMin, funcMax, value) { }
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Choice ///
struct ParameterModeStart : public ParameterChoice
{
    ParameterModeStart(pFuncBV funcActive, pchar *names) : ParameterChoice(ParameterChoiceType::ModeStart, funcActive, "Запуск", "Start", names) { }
};


struct ParameterModeStartStop : public ParameterChoice
{
    ParameterModeStartStop(pFuncBV funcActive, pchar *names) : ParameterChoice(ParameterChoiceType::ModeStartStop, funcActive, "А-Старт,В-Стоп", "A-Start,B-Stop", names) { }
};


struct ParameterManipulationEnabled : public ParameterChoice
{
    ParameterManipulationEnabled(pchar *names) : 
        ParameterChoice(ParameterChoiceType::ManipulationEnabled, Parameter::FuncActive, "Манип", "Manip", names) { }

};


struct ParameterPolarity : public ParameterChoice
{
    ParameterPolarity(pchar *names) : ParameterChoice(ParameterChoiceType::Polarity, Parameter::FuncActive, "Полярность", "Polarity", names) { }
};


struct ParameterClockImpulse : public ParameterChoice
{
    ParameterClockImpulse(pchar *names) : ParameterChoice(ParameterChoiceType::ClockImpulse, Parameter::FuncActive, "Оп. частота", "Clock", names) { }
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Complex ///
struct ParameterManipulation : public ParameterComposite
{
    ParameterManipulation(Parameter **paramaters);
};

