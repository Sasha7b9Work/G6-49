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

    pString GetStringDigits() const;

    Form *GetForm() { return form; }

private:
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
        ManipulationDuration,
        ManipulationPeriod,
        Frequency,
        Amplitude,
        Offset,
        Phase,
        Period,
        Duration,
        PacketNumber,
        PacketPeriod,
        Count
    };

    virtual pString Name() const override
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
        Polarity,
        StartMode,
        deleted_SetManipulationMode,
		ManipulationEnabled,
        Count
	};

	E Type() { return type; }

    int GetChoice() const;

private:
	E type;
    /// Текущий выбор
    int choice;
};


class ParameterStartMode : public ParameterChoice
{
public:
    ParameterStartMode(pString choice0 = " Авто", pString choice1 = " Однокр", pString choice2 = " Комп А", pString choice3 = " Форм B");
};


class ParameterManipulationEnabled : public ParameterChoice
{
public:
    ParameterManipulationEnabled(pString choice0, pString choice1);
    virtual pString Name() const override { return "Манипуляция"; }
};


class ParameterComplex : public Parameter
{
public:
    virtual void SetForm(Form *form) override;

    int NumParams() const { return numParams; }
    Parameter **Params() { return params; }

    ParameterValue *FindParameter(ParameterValue::E p);
    ParameterChoice *FindParameter(ParameterChoice::E p);
private:
    /// Здесь находятся дополнительные параметры в случае, если они требуются
    Parameter **params;
    /// Число дополнительных параметров или 0, если таковых не имеется
    int numParams;
};


class ParameterManipulation : public ParameterComplex
{
public:
    ParameterManipulation(Parameter **paramaters);
};
