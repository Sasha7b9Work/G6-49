#pragma once
#include "common/Common.h"
#include "Generator/MathSupport.h"
#include "Generator/Tuner.h"
#include "Generator/Viewer.h"
#include "Settings/SettingsTypes.h"


class Form;
struct Key;
class Parameter;


class Parameter
{
public:

    enum E
    {
        Double,     // Величина, выраженная числовым значением
        Exit,       // Закрыть составной параметр (манипуляция)
        Choice,     // Выбор из нескольких значений
        Composite   // Составной параметр, состоящий из нескольких простых (манипуляция)
    };

    Parameter(E k, const char *nRU, const char *nEN) : viewer(this), form(nullptr), parent(nullptr), kind(k)
    {
        name[0] = nRU;
        name[1] = nEN;
    }

    virtual ~Parameter() { }

    pString Name() const;

    virtual void SetForm(Form *form);

    void SetParent(Parameter *p);
    
    // Возвращает адрес родительского параметра
    Parameter *GetParent();

    // true, если параметр имеет тип Parameter::Value
    bool IsDouble() const;

    // true, если параметр имеет тип Parameter::Complex
    bool IsComplex() const;

    // true, если параметр имеет тип Parameter::Choice
    bool IsChoice() const;

    // Возвращает true, если параметр сложный и открыт
    bool IsOpened() const;

    virtual String ToString() const = 0;

    Form *GetForm();
    
    // Обработчик нажатия кнопки "Изменить"
    void OnPressButtonTune();

    Viewer viewer;

protected:
    
    Form *form;         // Форма, для которой зада этот параметр
    Parameter *parent;  // Если параметр вложенный, то здесь адрес родителя
    E kind;
    const char *name[2];
};


struct ParameterValueType
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
        PacketNumber,           // Число импульсов в пачке пакетного режима
        Exit,                   // Этот параметр закрывает открытый параметр
        Count
    };
};


class ParameterDouble : public Parameter
{
    friend struct LogicFloatValue;
    friend class Tuner;
    friend class TunerDisplay;

public:

    ParameterDouble(ParameterValueType::E t, const char *nameRU, const char *nameEN, const Value &_min, const Value &_max, const Value &_value);

    // Установить значение параметра и загрузить его в прибор
    bool SetAndLoadValue(double val);
    bool SetAndLoadValue(Value val);
    
    // Возвращает true, если параметр имеет знак
    bool IsSigned() const { return (type == ParameterValueType::Offset); }

    // Возвращает true, если параметр обозначает напряжение
    bool IsVoltage() const
    {
        return (type == ParameterValueType::Amplitude) || (type == ParameterValueType::Offset);
    }

    void OnPressButtonTune();

    // Возвращает строковое представление значения параметра
    virtual String ToString() const;
    String ToString(Value value) const;

    // Возвращает основные единицы измерения (без учёта порядка)
    pString GetMainUnits() const;

    // Возвращает единицы измерения, приведённые к порядку order. Если order == Order::Count, единциы будут рассчитываться исходя из текущего значения value
    pString GetUnits(Order::E order = Order::Count) const;

    // Возвращает максимальное значение, которое может иметь параметр
    Value GetMax() { return max; }
    
    // Возвращает минимальное значение, которое может иметь параметр
    Value GetMin() { return min; }

    Tuner *GetTuner()   { return &tuner; };

    // Возвращает текущее значение параметра
    Value GetValue() const { return value; };

    ParameterValueType::E GetType() const { return type; }

private:
    Tuner tuner;        // Используется для настройки 
    ParameterValueType::E type;
    Value min;
    Value max;
    Value value;

    // Возвращает true, если параметр может принимать значение v
    bool InRange(double v) const;
    bool InRange(Value v) const;
};


class ParameterChoice : public Parameter
{
public:
	enum E
	{
        Polarity,             // Полярность импульсов
        ModeStart,            // Режим запуска сигналов DDS и импульсных
        ManipulationEnabled,  // Включен или выключен режим манипуляции
        Count
	};

    ParameterChoice(E t, const char *nameRU, const char *nameEN) : Parameter(Parameter::Choice, nameRU, nameEN), type(t), choice(0), names(nullptr) { }

	E Type() { return type; }

    int GetChoice() const;

    void NextChoice();

    bool SetAndLoadChoice(int ch);

    bool DrawChoice(int x, int y) const;

    virtual String ToString() const;

    void OnPressButtonTune();

private:
	E type;
    int choice;             // Текущий выбор
    const char **names;
    
    // Количество вариантов выбора
    int NumChoices() const;
};


class ParameterComposite : public Parameter
{
public:
    enum E
    {
        Manipulation,   // НАСТРОЙКИ СИГНАЛОВ / Параметр / МАНИПУЛЯЦИЯ на форме СИНУС
        Count
    } value;

    ParameterComposite(E v, const char *nameRU, const char *nameEN, Parameter **parameters) :
        Parameter(Parameter::Composite, nameRU, nameEN), value(Count), params(parameters), numParams(0), type(v) { }

    virtual void SetForm(Form *form);

    int NumParams() const { return numParams; }
    Parameter **Params() { return params; }

    ParameterDouble *FindParameter(ParameterValueType::E p);
    ParameterChoice *FindParameter(ParameterChoice::E p);

    virtual String ToString() const;

    void OnPressButtonTune();

private:
    
    Parameter **params; // Здесь находятся дополнительные параметры в случае, если они требуются
    int numParams;      // Число дополнительных параметров или 0, если таковых не имеется
    E type;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Value ///
class ParameterVoltage : public ParameterDouble
{
public:
    ParameterVoltage(ParameterValueType::E type, const char *nameRU, const char *nameEN, const Value &min, const Value &max, const Value &value) :
        ParameterDouble(type, nameRU, nameEN, min, max, value) { }
};


class ParameterAmplitude : public ParameterVoltage
{
public:
    ParameterAmplitude(const Value &min = Value("0"), const Value &max = Value("10"), const Value &value = Value("0.123")) :
        ParameterVoltage(ParameterValueType::Amplitude, "Размах", "Amplitude", min, max, value) { }
};


class ParameterOffset : public ParameterVoltage
{
public:
    ParameterOffset(const Value &min = Value("-5"), const Value &max = Value("5"), const Value &value = Value("0")) :
        ParameterVoltage(ParameterValueType::Offset, "Смещение", "Offset", min, max, value) { }
};


class ParameterFrequency : public ParameterDouble
{
public:
    ParameterFrequency(const Value &min = Value("0.1"), const Value &max = Value("100", Order::Mega), const Value &value = Value("1", Order::Kilo)) :
        ParameterDouble(ParameterValueType::Frequency, "Частота", "Frequency", min, max, value) { }
};


class ParameterTime : public ParameterDouble
{
public:
    ParameterTime(ParameterValueType::E t, const char *nameRU, const char *nameEN, const Value &min, const Value &max, const Value &value) :
        ParameterDouble(t, nameRU, nameEN, min, max, value) { }
};


class ParameterPhase : public ParameterDouble
{
public:
    ParameterPhase() : ParameterDouble(ParameterValueType::Phase, "Фаза", "Phase", Value("0"), Value("360"), Value("0")) { }
};


class ParameterPacketPeriod : public ParameterTime
{
public:
    ParameterPacketPeriod(const Value &min, const Value &max, const Value &value) : ParameterTime(ParameterValueType::PacketPeriod, "Период", "Period", min, max, value) { }
};


class ParameterPeriod : public ParameterTime
{
public:
    ParameterPeriod(const Value &min, const Value &max, const Value &value) :  ParameterTime(ParameterValueType::Period, "Период", "Period", min, max, value) { }
};


class ParameterDuration : public ParameterTime
{
public:
    ParameterDuration(const Value &min, const Value &max, const Value &value) : ParameterTime(ParameterValueType::Duration, "Длительность", "Duration", min, max, value) { }
};


class ParameterManipulationDuration : public ParameterTime
{
public:
    ParameterManipulationDuration(const Value &min, const Value &max, const Value &value) :
        ParameterTime(ParameterValueType::ManipulationDuration, "Длительность", "Duration", min, max, value) { }
};


class ParameterManipulationPeriod : public ParameterTime
{
public:
    ParameterManipulationPeriod(const Value &min, const Value &max, const Value &value) :
        ParameterTime(ParameterValueType::ManipulationPeriod, "Период", "Period", min, max, value) { }
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Choice ///
class ParameterModeStart : public ParameterChoice
{
public:
    //ParameterModeStart(pString choice0 = " Авто", pString choice1 = " Однокр", pString choice2 = " Комп А", pString choice3 = " Форм B");

    ParameterModeStart() : ParameterChoice(ParameterChoice::ModeStart, "Запуск", "Start") { }
};


class ParameterManipulationEnabled : public ParameterChoice
{
public:
    ParameterManipulationEnabled(pString, pString) : ParameterChoice(ParameterChoice::ManipulationEnabled, "Манипуляция", "Manipulation") { }
};


class ParameterPolarity : public ParameterChoice
{
public:
    ParameterPolarity() : ParameterChoice(ParameterChoice::Polarity, "Полярность", "Polarity") { }
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Complex ///
class ParameterManipulation : public ParameterComposite
{
public:
    ParameterManipulation(Parameter **paramaters);
};

