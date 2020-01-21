#pragma once
#include "common/Common.h"
#include "Settings/SettingsTypes.h"


class Form;


class Parameter
{
public:

    enum E
    {
        Value,
        Exit,
        Choice,
        Complex,
        Page
    };

    Parameter(E k) : kind(k) { }

    virtual pString Name() const { return ""; }

    virtual void SetForm(Form *form);

    void SetParent(Parameter *p) { parent = p; }
    /// Возвращает адрес родительского параметра
    Parameter *GetParent() { return parent; }

    bool IsValue() const
    {
        return (kind == Value);
    }

    bool IsComplex() const
    {
        return (kind == Complex);
    }

    bool IsChoice() const
    {
        return (kind == Choice);
    }
    /// Возвращает true, если параметр сложный и открыт
    bool IsOpened() const { return parent != 0; }

    virtual pString GetStringDigits() const;

    Form *GetForm() { return form; }

    pString NameUnit(char buffer[10]) const;

protected:
    /// Форма, для которой зада этот параметр
    Form *form;
    /// Если параметр вложенный, то здесь адрес родителя
    Parameter *parent;

    E kind;
};


class ParameterValue : public Parameter
{
public:

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
        Count
    };

    ParameterValue(E t, const FloatValue &_min, const FloatValue &_max, const FloatValue &_value) : Parameter(Parameter::Value), type(t), min(_min), max(_max), value(_value) { }

    virtual pString Name() const
    {
        return "";
    }

    E Type() const { return type; }

    FloatValue GetValue() const { return value; };

    pString GetStringValue(Language::E lang) const;

    bool SetAndLoadValue(float val);

private:
    E type;

    FloatValue min;
    FloatValue max;
    FloatValue value;

    pString MainUnits(Language::E lang) const;
    /// Возвращает true, если параметр может принимать значение v
    bool InRange(float v) const;
};


class ParameterVoltage : public ParameterValue
{
public:
    ParameterVoltage(ParameterValue::E type, const FloatValue &min, const FloatValue &max, const FloatValue &value) :  ParameterValue(type, min, max, value) { }
};


class ParameterAmplitude : public ParameterVoltage
{
public:
    ParameterAmplitude(const FloatValue &min = FloatValue(0, 0), const FloatValue &max = FloatValue(10, 0), const FloatValue &value = FloatValue(10, 0)) :
        ParameterVoltage(ParameterValue::Amplitude, min, max, value) { }
};


class ParameterOffset : public ParameterVoltage
{
public:
    ParameterOffset(const FloatValue &min = FloatValue(-5, 0), const FloatValue &max = FloatValue(5, 0), const FloatValue &value = FloatValue(0, 0)) :
        ParameterVoltage(ParameterValue::Offset, min, max, value) { }
};


class ParameterFrequency : public ParameterValue
{
public:
    ParameterFrequency(const FloatValue &min = FloatValue(0, 100), const FloatValue &max = FloatValue(100 * 1000 * 1000, 0), const FloatValue &value = FloatValue(1000, 0)) :
        ParameterValue(ParameterValue::Frequency, min, max, value) { }
};


class ParameterTime : public ParameterValue
{
public:
    ParameterTime(ParameterValue::E t, const FloatValue &min, const FloatValue &max, const FloatValue &value) :  ParameterValue(t, min, max, value) { }
};


class ParameterPhase : public ParameterValue
{
public:
    ParameterPhase() : ParameterValue(ParameterValue::Phase, FloatValue(0, 0), FloatValue(360, 0), FloatValue(0, 0)) { }
};


class ParameterPacketPeriod : public ParameterTime
{
public:
    ParameterPacketPeriod(const FloatValue &min, const FloatValue &max, const FloatValue &value) : ParameterTime(ParameterValue::PacketPeriod, min, max, value) { }
};


class ParameterPeriod : public ParameterTime
{
public:
    ParameterPeriod(const FloatValue &min, const FloatValue &max, const FloatValue &value) : ParameterTime(ParameterValue::Period, min, max, value) { }
};


class ParameterDuration : public ParameterTime
{
public:
    ParameterDuration(const FloatValue &min, const FloatValue &max, const FloatValue &value) : ParameterTime(ParameterValue::Duration, min, max, value) { }
};


class ParameterManipulationDuration : public ParameterTime
{
public:
    ParameterManipulationDuration(const FloatValue &min, const FloatValue &max, const FloatValue &value) : ParameterTime(ParameterValue::ManipulationDuration, min, max, value) { }
};


class ParameterManipulationPeriod : public ParameterTime
{
public:
    ParameterManipulationPeriod(const FloatValue &min, const FloatValue &max, const FloatValue &value) : ParameterTime(ParameterValue::ManipulationPeriod, min, max, value) { }
};


class ParameterChoice : public Parameter
{
public:
	enum E
	{
        Polarity,                   ///< Полярность импульсов
        ModeStart,                  ///< Режим запуска сигналов DDS и импульсных
        deleted_ManipulationMode,   ///< Режим амплитудной манипуляции - со сглаживанием фронта или без
        ManipulationEnabled,        ///< Включен или выключен режим манипуляции
        Count
	};

    ParameterChoice(E t) : Parameter(Parameter::Choice), type(t) { }

	E Type() { return type; }

    int GetChoice() const;

    virtual pString Name() const;

    virtual pString GetStringDigits() const;

    void NextChoice();

    bool SetAndLoadChoice(int ch);

    bool DrawChoice(int x, int y) const;

private:
	E type;
    /// Текущий выбор
    int choice;

    const char **names;
    /// Количество вариантов выбора
    int NumChoices() const;
};


class ParameterModeStart : public ParameterChoice
{
public:
    //ParameterModeStart(pString choice0 = " Авто", pString choice1 = " Однокр", pString choice2 = " Комп А", pString choice3 = " Форм B");

    ParameterModeStart() : ParameterChoice(ParameterChoice::ModeStart) { }
};


class ParameterManipulationEnabled : public ParameterChoice
{
public:
    ParameterManipulationEnabled(pString, pString) : ParameterChoice(ParameterChoice::ManipulationEnabled) { }
    virtual pString Name() const { return "Манипуляция"; }
};


class ParameterPolarity : public ParameterChoice
{
public:
    ParameterPolarity() : ParameterChoice(ParameterChoice::Polarity) { }
};


class ParameterComplex : public Parameter
{
public:
    enum E
    {
        Manipulation,       ///< НАСТРОЙКИ СИГНАЛОВ / Параметр / МАНИПУЛЯЦИЯ на форме СИНУС
        Count
    } value;

    ParameterComplex(E v, Parameter **parameters) : Parameter(Parameter::Complex), params(parameters), type(v) { }

    virtual void SetForm(Form *form);

    virtual pString Name() const;

    int NumParams() const { return numParams; }
    Parameter **Params() { return params; }

    virtual pString GetStringDigits() const;

    ParameterValue *FindParameter(ParameterValue::E p);
    ParameterChoice *FindParameter(ParameterChoice::E p);
private:
    /// Здесь находятся дополнительные параметры в случае, если они требуются
    Parameter **params;
    /// Число дополнительных параметров или 0, если таковых не имеется
    int numParams;

    E type;
};


class ParameterManipulation : public ParameterComplex
{
public:
    ParameterManipulation(Parameter **paramaters);
};
