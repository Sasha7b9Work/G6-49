#pragma once
#include "common/Common.h"
#include "Settings/SettingsTypes.h"


class Form;


class Parameter
{
public:
    virtual pString Name() const { return ""; }

    virtual void SetForm(Form *form);

    void SetParent(Parameter *p) { parent = p; }
    /// Возвращает адрес родительского параметра
    Parameter *GetParent() { return parent; }

    bool IsValue();

    bool IsComplex();

    bool IsChoice();
    /// Возвращает true, если параметр сложный и открыт
    bool IsOpened() const { return parent != 0; }

    virtual pString GetStringDigits() const;

    Form *GetForm() { return form; }

    pString NameUnit(char buffer[10]);

protected:
    /// Форма, для которой зада этот параметр
    Form *form;
    /// Если параметр вложенный, то здесь адрес родителя
    Parameter *parent;
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

    ParameterValue(const FloatValue &min, const FloatValue &max, const FloatValue &value);

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

    FloatValue value;

    pString MainUnits(Language::E lang) const;
    /// Возвращает true, если параметр может принимать значение v
    bool InRange(float v) const;
};


class ParameterVoltage : public ParameterValue
{
public:
    ParameterVoltage(const FloatValue &min, const FloatValue &max, const FloatValue &value);
};


class ParameterAmplitude : public ParameterVoltage
{
public:
    ParameterAmplitude(const FloatValue &min = FloatValue(0, 0), const FloatValue &max = FloatValue(10, 0), const FloatValue &value = FloatValue(10, 0));
};


class ParameterOffset : public ParameterVoltage
{
public:
    ParameterOffset(const FloatValue &min = FloatValue(-5, 0), const FloatValue &max = FloatValue(5, 0), const FloatValue &value = FloatValue(0, 0));
};


class ParameterFrequency : public ParameterValue
{
public:
    ParameterFrequency(const FloatValue &min = FloatValue(0, 100), const FloatValue &max = FloatValue(100 * 1000 * 1000, 0), const FloatValue &value = FloatValue(1000, 0));
};


class ParameterTime : public ParameterValue
{
public:
    ParameterTime(const FloatValue &min, const FloatValue &max, const FloatValue &value);
};


class ParameterPhase : public ParameterValue
{
public:
    ParameterPhase();
};


class ParameterPacketPeriod : public ParameterTime
{
public:
    ParameterPacketPeriod(const FloatValue &min, const FloatValue &max, const FloatValue &value);
};


class ParameterPeriod : public ParameterTime
{
public:
    ParameterPeriod(const FloatValue &min, const FloatValue &max, const FloatValue &value);
};


class ParameterDuration : public ParameterTime
{
public:
    ParameterDuration(const FloatValue &min, const FloatValue &max, const FloatValue &value);
};


class ParameterManipulationDuration : public ParameterTime
{
public:
    ParameterManipulationDuration(const FloatValue &min, const FloatValue &max, const FloatValue &value);
};


class ParameterManipulationPeriod : public ParameterTime
{
public:
    ParameterManipulationPeriod(const FloatValue &min, const FloatValue &max, const FloatValue &value);
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
    ParameterModeStart(pString choice0 = " Авто", pString choice1 = " Однокр", pString choice2 = " Комп А", pString choice3 = " Форм B");
};


class ParameterManipulationEnabled : public ParameterChoice
{
public:
    ParameterManipulationEnabled(pString choice0, pString choice1);
    virtual pString Name() const { return "Манипуляция"; }
};


class ParameterPolarity : public ParameterChoice
{
public:
    ParameterPolarity();
};


class ParameterComplex : public Parameter
{
public:
    enum E
    {
        Manipulation,       ///< НАСТРОЙКИ СИГНАЛОВ / Параметр / МАНИПУЛЯЦИЯ на форме СИНУС
        Count
    } value;

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
