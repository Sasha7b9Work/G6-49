#pragma once
#include "common/Common.h"
#include "Display/Tuner.h"
#include "Display/Viewer.h"
#include "Generator/MathSupport.h"
#include "Settings/SettingsTypes.h"


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

    // Возвращает true, если параметр сложный и открыт
    bool IsOpened() const;

    virtual String ToString(String &units) const = 0;

    Form *GetForm();
    
    ParameterKind::E GetKind() { return kind; }

    Viewer viewer;

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

    // Установить значение параметра и загрузить его в прибор
    bool SetAndLoadValue(double val);
    bool SetAndLoadValue(Value val);
    
    // Возвращает true, если параметр имеет знак
    bool IsSigned() const { return (type == ParameterDoubleType::Offset); }

    // Возвращает true, если параметр имеет размерность "вольт"
    bool IsVoltage() const;

    // Возвращает true, если параметр имеетр размерность "секунда"
    bool IsTime() const;

    virtual void OnPressButtonTune();

    // Возвращает строковое представление значения параметра
    virtual String ToString(String &units) const;
    String ToString(Value value) const;

    // Возвращает основные единицы измерения (без учёта порядка)
    pString GetMainUnits() const;

    // Возвращает единицы измерения, приведённые к порядку order. Если order == Order::Count, единциы будут рассчитываться исходя из текущего значения value
    pString GetUnits(Order::E order = Order::Count) const;

    // Возвращает максимальное значение, которое может иметь параметр
    Value GetMax() { return max; }
    
    // Возвращает минимальное значение, которое может иметь параметр
    Value GetMin() { return min; }

    virtual Tuner *_GetTuner()   { return &tuner; };

    // Возвращает текущее значение параметра
    Value GetValue() const { return value; };

    ParameterDoubleType::E GetType() const { return type; }

private:
    Tuner tuner;        // Используется для настройки 
    ParameterDoubleType::E type;
    Value min;
    Value max;
    Value value;

    // Возвращает true, если параметр может принимать значение v
    bool InRange(double v) const;
    bool InRange(Value v) const;
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

    ParameterInteger(ParameterIntegerType::E t, const char *nameRU, const char *nameEN, const int min, const int max, const int value);

    virtual String ToString(String &units) const;

    virtual void OnPressButtonTune();

    virtual Tuner *_GetTuner()  { return &tuner; }

private:

    Tuner tuner;
    ParameterIntegerType::E type;
    int min;
    int max;
    int value;
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

    virtual void OnPressButtonTune();

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

    int NumParameters() const;
    Parameter **Parameters() { return params; }

    ParameterDouble *FindParameter(ParameterDoubleType::E p);
    ParameterChoice *FindParameter(ParameterChoiceType::E p);

    virtual String ToString(String &units) const;

    virtual void OnPressButtonTune();

private:
    
    Parameter **params; // Здесь находятся дополнительные параметры в случае, если они требуются
    ParameterCompositeType::E type;
};


class ParameterButton : public Parameter
{
public:

    ParameterButton(const char *titleRU, const char *titleEN, pFuncVV f) : Parameter(ParameterKind::Button, titleRU, titleEN), func(f) {};

    virtual String ToString(String &) const { return String(""); };

    virtual void OnPressButtonTune() { func(); };

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
    ParameterAmplitude(const Value &min = Value("0"), const Value &max = Value("10"), const Value &value = Value("10")) :
        ParameterVoltage(ParameterDoubleType::Amplitude, "Размах", "Amplitude", min, max, value) { }
};


class ParameterOffset : public ParameterVoltage
{
public:
    ParameterOffset(const Value &min = Value("-5"), const Value &max = Value("5"), const Value &value = Value("0")) :
        ParameterVoltage(ParameterDoubleType::Offset, "Смещение", "Offset", min, max, value) { }
};


class ParameterFrequency : public ParameterDouble
{
public:
    ParameterFrequency(const Value &min = Value("0.1"), const Value &max = Value("100", Order::Mega), const Value &value = Value("1", Order::Kilo)) :
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
    ParameterPhase() : ParameterDouble(ParameterDoubleType::Phase, "Фаза", "Phase", Value("0"), Value("360"), Value("0")) { }
};


class ParameterPacketPeriod : public ParameterTime
{
public:
    ParameterPacketPeriod(const Value &min, const Value &max, const Value &value) : ParameterTime(ParameterDoubleType::PacketPeriod, "Период пакета", "Packet period", min, max, value) { }
};


class ParameterPeriod : public ParameterTime
{
public:
    ParameterPeriod(const Value &min, const Value &max, const Value &value) :  ParameterTime(ParameterDoubleType::Period, "Период", "Period", min, max, value) { }
};


class ParameterDuration : public ParameterTime
{
public:
    ParameterDuration(const Value &min, const Value &max, const Value &value) : ParameterTime(ParameterDoubleType::Duration, "Длительность", "Duration", min, max, value) { }
};


class ParameterManipulationDuration : public ParameterTime
{
public:
    ParameterManipulationDuration(const Value &min, const Value &max, const Value &value) :
        ParameterTime(ParameterDoubleType::ManipulationDuration, "Длительность", "Duration", min, max, value) { }
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
        ParameterChoice(ParameterChoiceType::ManipulationEnabled, "Манипуляция", "Manipulation", names) { }
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

