#pragma once
#include "common/Common.h"
#include "Display/Tuner.h"
#include "Display/Viewer.h"
#include "Generator/MathSupport.h"
#include "Settings/SettingsTypes.h"


#define DEFAULT_AMPLITUDE Value("1", Order::One)

#define IMPULSE_PERIOD_MIN   Value("20", Order::Nano)
#define IMPULSE_DURATION_MIN Value("10", Order::Nano)


class Form;
struct Key;
class Parameter;


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


class Parameter
{
public:

    Parameter(ParameterKind::E k, const char *nRU, const char *nEN);

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

protected:
    
    Form *form;         // Форма, для которой зада этот параметр
    Parameter *parent;  // Если параметр вложенный, то здесь адрес родителя
    ParameterKind::E kind;
    const char *names[2];
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
        Delay,                  // Задержка
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


class ParameterDouble : public Parameter
{
    friend struct LogicFloatValue;
    friend class Tuner;
    friend class TunerDisplay;

public:

    ParameterDouble(ParameterDoubleType::E t, const char *nameRU, const char *nameEN, const Value &_min, const Value &_max, const Value &_value);

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

    ParameterDoubleType::E GetType() const { return type; }

private:
    Tuner tuner;        // Используется для настройки 
    ParameterDoubleType::E type;
    Value min;
    Value value;
    Value resetValue;

    // Возвращает true, если параметр может принимать значение v
    bool InRange(double v) const;
    bool InRange(Value v) const;

protected:
    Value max;
};


struct ParameterIntegerType
{
    enum E
    {
        PacketNumber,           // Число импульсов в пачке пакетного режима
        Count
    };
};


class ParameterInteger : public Parameter
{
public:

    ParameterInteger(ParameterIntegerType::E t, const char *nameRU, const char *nameEN, const Value &min, const Value &max, const Value &value);

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
        Polarity,             // Полярность импульсов
        ModeStart,            // Режим запуска сигналов DDS и импульсных
        ManipulationEnabled,  // Включен или выключен режим манипуляции
        Count
    };

    static const char *Name(ParameterChoiceType::E type);
};


class ParameterChoice : public Parameter
{
public:

    ParameterChoice(ParameterChoiceType::E t, const char *nameRU, const char *nameEN, const char **_choices = nullptr) :
        Parameter(ParameterKind::Choice, nameRU, nameEN), type(t), choice(0), choices(_choices) { }

    ParameterChoiceType::E GetType() { return type; }

    int GetChoice() const            {  return choice; }

    void NextChoice();

    bool SetAndLoadChoice(int ch);

    virtual String ToString(String &units) const;

    virtual String ToString(Value) const { return String(""); };

    virtual void OnPressButtonTune();

    int GetHeightOpened() const;

    virtual void Reset() { SetAndLoadChoice(choice); };

private:
	ParameterChoiceType::E type;
    int choice;                     // Текущий выбор
    const char **choices;           // Идут так - 0(рус), 0(англ), 1(рус), 1(англ)...
    
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


class ParameterComposite : public Parameter
{
public:

    ParameterComposite(ParameterCompositeType::E v, const char *nameRU, const char *nameEN, Parameter **parameters) :
        Parameter(ParameterKind::Composite, nameRU, nameEN), params(parameters), type(v) { }

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


class ParameterButton : public Parameter
{
public:

    ParameterButton(const char *titleRU, const char *titleEN, pFuncVV f) : Parameter(ParameterKind::Button, titleRU, titleEN), func(f) {};

    virtual String ToString(String &) const { return String(""); };

    virtual String ToString(Value) const { return String(""); };

    virtual void OnPressButtonTune() { func(); };

    virtual void Reset() {}

private:

    pFuncVV func;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Value ///
class ParameterVoltage : public ParameterDouble
{
public:
    ParameterVoltage(ParameterDoubleType::E type, const char *nameRU, const char *nameEN, const Value &min, const Value &max, const Value &value) :
        ParameterDouble(type, nameRU, nameEN, min, max, value) { }
};


class ParameterAmplitude : public ParameterVoltage
{
public:
    ParameterAmplitude(const Value &min = Value("0", Order::One), const Value &max = Value("10", Order::One), const Value &value = DEFAULT_AMPLITUDE) :
        ParameterVoltage(ParameterDoubleType::Amplitude, "Размах", "Amplitude", min, max, value) { }

    virtual Value GetMax() const;
};


class ParameterOffset : public ParameterVoltage
{
public:
    ParameterOffset(const Value &min = Value("-5", Order::One), const Value &max = Value("5", Order::One), const Value &value = Value("0", Order::One)) :
        ParameterVoltage(ParameterDoubleType::Offset, "Смещение", "Offset", min, max, value) { }

    virtual Value GetMax() const;
    virtual Value GetMin() const;
};


class ParameterFrequency : public ParameterDouble
{
public:
    ParameterFrequency(const Value &min, const Value &max, const Value &value = Value("1", Order::Kilo)) :
        ParameterDouble(ParameterDoubleType::Frequency, "Частота", "Frequency", min, max, value) { }
};


class ParameterTime : public ParameterDouble
{
public:
    ParameterTime(ParameterDoubleType::E t, const char *nameRU, const char *nameEN, const Value &min, const Value &max, const Value &value) :
        ParameterDouble(t, nameRU, nameEN, min, max, value) { }
};


class ParameterPhase : public ParameterDouble
{
public:
    ParameterPhase() : ParameterDouble(ParameterDoubleType::Phase, "Фаза", "Phase", Value("0", Order::One), Value("360", Order::One), Value("0", Order::One)) { }
};


class ParameterPacketPeriod : public ParameterTime
{
public:
    ParameterPacketPeriod(const Value &max, const Value &value) : ParameterTime(ParameterDoubleType::PacketPeriod, "Период пак", "Packet per", IMPULSE_PERIOD_MIN, max, value) { }
};


class ParameterPeriod : public ParameterTime
{
public:
    ParameterPeriod(const Value &max, const Value &value, const char *nameRU = "Период", const char *nameEN = "Period") :
        ParameterTime(ParameterDoubleType::Period, nameRU, nameEN, IMPULSE_PERIOD_MIN, max, value) { }
};


class ParameterDuration : public ParameterTime
{
public:
    ParameterDuration(const Value &max, const Value &value, const char *nameRU = "Длит", const char *nameEN = "Dur") :
        ParameterTime(ParameterDoubleType::Duration, nameRU, nameEN, IMPULSE_PERIOD_MIN, max, value) { }
};


class ParameterManipulationDuration : public ParameterTime
{
public:
    ParameterManipulationDuration(const Value &min, const Value &max, const Value &value) :
        ParameterTime(ParameterDoubleType::ManipulationDuration, "Длит", "Duration", min, max, value) { }
};


class ParameterManipulationPeriod : public ParameterTime
{
public:
    ParameterManipulationPeriod(const Value &min, const Value &max, const Value &value) :
        ParameterTime(ParameterDoubleType::ManipulationPeriod, "Период", "Period", min, max, value) { }
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Choice ///
class ParameterModeStart : public ParameterChoice
{
public:
    ParameterModeStart(const char **names) : ParameterChoice(ParameterChoiceType::ModeStart, "Запуск", "Start", names) { }
};


class ParameterManipulationEnabled : public ParameterChoice
{
public:
    ParameterManipulationEnabled(const char **names) : 
        ParameterChoice(ParameterChoiceType::ManipulationEnabled, "Манипу", "Manip", names) { }

};


class ParameterPolarity : public ParameterChoice
{
public:
    ParameterPolarity(const char **names) : ParameterChoice(ParameterChoiceType::Polarity, "Полярность", "Polarity", names) { }
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Complex ///
class ParameterManipulation : public ParameterComposite
{
public:
    ParameterManipulation(Parameter **paramaters);
};

