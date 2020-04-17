#pragma once
#include "common/Common.h"
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
        Value,
        Exit,
        Choice,
        Complex,
        Page
    };

    Parameter(E k, const char *nRU, const char *nEN) : viewer(this), tuner(this), form(nullptr), parent(nullptr), kind(k)
    {
        name[0] = nRU;
        name[1] = nEN;
    }

    virtual ~Parameter() { }

    pString Name(uint lang) const;

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

    virtual pString GetStringValue() const = 0;

    Form *GetForm();
    
    // Обработчик нажатия кнопки "Изменить"
    void OnPressButtonTune();

    Viewer viewer;

    Tuner tuner;

protected:
    
    // Форма, для которой зада этот параметр
    Form *form;
    
    // Если параметр вложенный, то здесь адрес родителя
    Parameter *parent;

    E kind;

    const char *name[2];
};


class ParameterValue : public Parameter
{
    friend class ParameterPainter;
    friend struct LogicFloatValue;
public:

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

    ParameterValue(E t, const char *nameRU, const char *nameEN, const FloatValue &_min, const FloatValue &_max, const FloatValue &_value) :
        Parameter(Parameter::Value, nameRU, nameEN), type(t), min(_min), max(_max), value(_value) { }

    E Type() const { return type; }

    FloatValue GetValue() const { return value; };

    virtual pString GetStringValue() const;

    bool SetAndLoadValue(float val);
    
    // Возвращает true, если параметр имеет знак
    bool IsSigned() const
    {
        return (type == Offset);
    }

    void OnPressButtonTune();

private:
    E type;

    FloatValue min;
    FloatValue max;
    FloatValue value;

    pString MainUnits(uint lang) const;

    // Возвращает true, если параметр может принимать значение v
    bool InRange(float v) const;
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

    virtual pString GetStringValue() const;

    void OnPressButtonTune();

private:
	E type;
    
    // Текущий выбор
    int choice;

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

    ParameterValue *FindParameter(ParameterValue::E p);
    ParameterChoice *FindParameter(ParameterChoice::E p);

    virtual pString GetStringValue() const;

    void OnPressButtonTune();

private:
    
    // Здесь находятся дополнительные параметры в случае, если они требуются
    Parameter **params;
    
    // Число дополнительных параметров или 0, если таковых не имеется
    int numParams;

    E type;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Value ///
class ParameterVoltage : public ParameterValue
{
public:
    ParameterVoltage(ParameterValue::E type, const char *nameRU, const char *nameEN, const FloatValue &min, const FloatValue &max, const FloatValue &value) :
        ParameterValue(type, nameRU, nameEN, min, max, value) { }
};


class ParameterAmplitude : public ParameterVoltage
{
public:
    ParameterAmplitude(const FloatValue &min = FloatValue(0, 0), const FloatValue &max = FloatValue(10, 0), const FloatValue &value = FloatValue(10, 0)) :
        ParameterVoltage(ParameterValue::Amplitude, "Размах", "Amplitude", min, max, value) { }
};


class ParameterOffset : public ParameterVoltage
{
public:
    ParameterOffset(const FloatValue &min = FloatValue(-5, 0), const FloatValue &max = FloatValue(5, 0), const FloatValue &value = FloatValue(0, 0)) :
        ParameterVoltage(ParameterValue::Offset, "Смещение", "Offset", min, max, value) { }
};


class ParameterFrequency : public ParameterValue
{
public:
    ParameterFrequency(const FloatValue &min = FloatValue(0, 100), const FloatValue &max = FloatValue(100 * 1000 * 1000, 0), const FloatValue &value = FloatValue(1000, 0)) :
        ParameterValue(ParameterValue::Frequency, "Частота", "Frequency", min, max, value) { }
};


class ParameterTime : public ParameterValue
{
public:
    ParameterTime(ParameterValue::E t, const char *nameRU, const char *nameEN, const FloatValue &min, const FloatValue &max, const FloatValue &value) :
        ParameterValue(t, nameRU, nameEN, min, max, value) { }
};


class ParameterPhase : public ParameterValue
{
public:
    ParameterPhase() : ParameterValue(ParameterValue::Phase, "Фаза", "Phase", FloatValue(0, 0), FloatValue(360, 0), FloatValue(0, 0)) { }
};


class ParameterPacketPeriod : public ParameterTime
{
public:
    ParameterPacketPeriod(const FloatValue &min, const FloatValue &max, const FloatValue &value) : ParameterTime(ParameterValue::PacketPeriod, "Период", "Period", min, max, value) { }
};


class ParameterPeriod : public ParameterTime
{
public:
    ParameterPeriod(const FloatValue &min, const FloatValue &max, const FloatValue &value) : ParameterTime(ParameterValue::Period, "Период", "Period", min, max, value) { }
};


class ParameterDuration : public ParameterTime
{
public:
    ParameterDuration(const FloatValue &min, const FloatValue &max, const FloatValue &value) : ParameterTime(ParameterValue::Duration, "Длительность", "Duration", min, max, value) { }
};


class ParameterManipulationDuration : public ParameterTime
{
public:
    ParameterManipulationDuration(const FloatValue &min, const FloatValue &max, const FloatValue &value) :
        ParameterTime(ParameterValue::ManipulationDuration, "Длительность", "Duration", min, max, value) { }
};


class ParameterManipulationPeriod : public ParameterTime
{
public:
    ParameterManipulationPeriod(const FloatValue &min, const FloatValue &max, const FloatValue &value) :
        ParameterTime(ParameterValue::ManipulationPeriod, "Период", "Period", min, max, value) { }
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


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Complex ///
class ParameterManipulation : public ParameterComplex
{
public:
    ParameterManipulation(Parameter **paramaters);
};

