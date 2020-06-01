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
        Value,      // Величина, выраженная числовым значением
        Exit,       // Закрыть составной параметр (манипуляция)
        Choice,     // Выбор из нескольких значений
        Complex     // Составной параметр, состоящий из нескольких простых (манипуляция)
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
    bool IsValue() const;

    // true, если параметр имеет тип Parameter::Complex
    bool IsComplex() const;

    // true, если параметр имеет тип Parameter::Choice
    bool IsChoice() const;

    // Возвращает true, если параметр сложный и открыт
    bool IsOpened() const;

    virtual pString ToString() const = 0;

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


class ParameterValue : public Parameter
{
    friend struct LogicFloatValue;
    friend class Tuner;
    friend class TunerDisplay;

public:

    ParameterValue(ParameterValueType::E t, const char *nameRU, const char *nameEN, const DoubleValue &_min, const DoubleValue &_max, const DoubleValue &_value);

    // Установить значение параметра и загрузить его в прибор
    bool SetAndLoadValue(double val);
    bool SetAndLoadValue(DoubleValue val);
    
    // Возвращает true, если параметр имеет знак
    bool IsSigned() const { return (type == ParameterValueType::Offset); }

    // Возвращает true, если параметр обозначает напряжение
    bool IsVoltage() const
    {
        return (type == ParameterValueType::Amplitude) || (type == ParameterValueType::Offset);
    }

    void OnPressButtonTune();

    // Возвращает строковое представление значения параметра
    virtual pString ToString() const;
    virtual pString ToString(DoubleValue value) const;

    // Возвращает основные единицы измерения (без учёта порядка)
    pString GetMainUnits() const;

    // Возвращает единицы измерения, приведённые к порядку order. Если order == Order::Count, единциы будут рассчитываться исходя из текущего значения value
    pString GetUnits(Order::E order = Order::Count) const;

    // Возвращает максимальное значение, которое может иметь параметр
    DoubleValue GetMax() { return max; }
    
    // Возвращает минимальное значение, которое может иметь параметр
    DoubleValue GetMin() { return min; }

    Tuner *GetTuner()   { return &tuner; };

    // Возвращает текущее значение параметра
    DoubleValue GetValue() const { return value; };

    ParameterValueType::E GetType() const { return type; }

private:
    Tuner tuner;        // Используется для настройки 
    ParameterValueType::E type;
    DoubleValue min;
    DoubleValue max;
    DoubleValue value;

    // Возвращает true, если параметр может принимать значение v
    bool InRange(double v) const;
    bool InRange(DoubleValue v) const;
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

    virtual pString ToString() const;
    virtual pString ToString(DoubleValue) const { return ""; }

    void OnPressButtonTune();

private:
	E type;
    int choice;             // Текущий выбор
    const char **names;
    
    // Количество вариантов выбора
    int NumChoices() const;
};


class ParameterComplex : public Parameter
{
public:
    enum E
    {
        Manipulation,   // НАСТРОЙКИ СИГНАЛОВ / Параметр / МАНИПУЛЯЦИЯ на форме СИНУС
        Count
    } value;

    ParameterComplex(E v, const char *nameRU, const char *nameEN, Parameter **parameters) :
        Parameter(Parameter::Complex, nameRU, nameEN), value(Count), params(parameters), numParams(0), type(v) { }

    virtual void SetForm(Form *form);

    int NumParams() const { return numParams; }
    Parameter **Params() { return params; }

    ParameterValue *FindParameter(ParameterValueType::E p);
    ParameterChoice *FindParameter(ParameterChoice::E p);

    virtual pString ToString() const;

    void OnPressButtonTune();

private:
    
    Parameter **params; // Здесь находятся дополнительные параметры в случае, если они требуются
    int numParams;      // Число дополнительных параметров или 0, если таковых не имеется
    E type;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Value ///
class ParameterVoltage : public ParameterValue
{
public:
    ParameterVoltage(ParameterValueType::E type, const char *nameRU, const char *nameEN, const DoubleValue &min, const DoubleValue &max, const DoubleValue &value) :
        ParameterValue(type, nameRU, nameEN, min, max, value) { }
};


class ParameterAmplitude : public ParameterVoltage
{
public:
    ParameterAmplitude(const DoubleValue &min = DoubleValue("0"), const DoubleValue &max = DoubleValue("10"), const DoubleValue &value = DoubleValue("10")) :
        ParameterVoltage(ParameterValueType::Amplitude, "Размах", "Amplitude", min, max, value) { }
};


class ParameterOffset : public ParameterVoltage
{
public:
    ParameterOffset(const DoubleValue &min = DoubleValue("-5"), const DoubleValue &max = DoubleValue("5"), const DoubleValue &value = DoubleValue("0")) :
        ParameterVoltage(ParameterValueType::Offset, "Смещение", "Offset", min, max, value) { }
};


class ParameterFrequency : public ParameterValue
{
public:
    ParameterFrequency(const DoubleValue &min = DoubleValue("0.1"), const DoubleValue &max = DoubleValue("100", 6), const DoubleValue &value = DoubleValue("1000")) :
        ParameterValue(ParameterValueType::Frequency, "Частота", "Frequency", min, max, value) { }
};


class ParameterTime : public ParameterValue
{
public:
    ParameterTime(ParameterValueType::E t, const char *nameRU, const char *nameEN, const DoubleValue &min, const DoubleValue &max, const DoubleValue &value) :
        ParameterValue(t, nameRU, nameEN, min, max, value) { }
};


class ParameterPhase : public ParameterValue
{
public:
    ParameterPhase() : ParameterValue(ParameterValueType::Phase, "Фаза", "Phase", DoubleValue("0"), DoubleValue("360"), DoubleValue("0")) { }
};


class ParameterPacketPeriod : public ParameterTime
{
public:
    ParameterPacketPeriod(const DoubleValue &min, const DoubleValue &max, const DoubleValue &value) : ParameterTime(ParameterValueType::PacketPeriod, "Период", "Period", min, max, value) { }
};


class ParameterPeriod : public ParameterTime
{
public:
    ParameterPeriod(const DoubleValue &min, const DoubleValue &max, const DoubleValue &value) :  ParameterTime(ParameterValueType::Period, "Период", "Period", min, max, value) { }
};


class ParameterDuration : public ParameterTime
{
public:
    ParameterDuration(const DoubleValue &min, const DoubleValue &max, const DoubleValue &value) : ParameterTime(ParameterValueType::Duration, "Длительность", "Duration", min, max, value) { }
};


class ParameterManipulationDuration : public ParameterTime
{
public:
    ParameterManipulationDuration(const DoubleValue &min, const DoubleValue &max, const DoubleValue &value) :
        ParameterTime(ParameterValueType::ManipulationDuration, "Длительность", "Duration", min, max, value) { }
};


class ParameterManipulationPeriod : public ParameterTime
{
public:
    ParameterManipulationPeriod(const DoubleValue &min, const DoubleValue &max, const DoubleValue &value) :
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
class ParameterManipulation : public ParameterComplex
{
public:
    ParameterManipulation(Parameter **paramaters);
};

