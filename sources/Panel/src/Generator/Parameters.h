// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
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
        Double,     // ��������, ���������� �������� ���������
        Choice,     // ����� �� ���������� ��������
        Composite,  // ��������� ��������, ��������� �� ���������� ������� (�����������)
        Button,     // ������ - �� � ������� ���������� �����-���� ��������
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

    // ���������� ������� ������ "��������"
    virtual void OnPressButtonTune() = 0;

    virtual Tuner *GetTuner() { return nullptr; }

    void SetParent(Parameter *p);
    
    // ���������� ����� ������������� ���������
    Parameter *GetParent();

    bool IsDouble() const;
    bool IsInteger() const;
    bool IsComposite() const;
    bool IsChoice() const;

    virtual void Reset() = 0;

    // ���������� true, ���� �������� ������� � ������
    bool IsOpened() const;

    Form *GetForm();
    
    ParameterKind::E GetKind() { return kind; }

    Viewer viewer;

    virtual Value GetMax() const { return Value("1", Order::One); }

    virtual Value GetMin() const { return Value("0", Order::One); }

    virtual Value GetValue() const { return Value("1", Order::One); } //-V524

    virtual String ToString(String &) const { return String(""); };

    virtual String ToString(Value) const { return String(""); };

    // ��������� ��������� ���������
    virtual void SaveState() { };

    // ������������ ��������� ���������
    virtual void RestoreState() { };

protected:
    
    Form             *form;         // �����, ��� ������� ���� ���� ��������
    Parameter        *parent;       // ���� �������� ���������, �� ����� ����� ��������
    ParameterKind::E  kind;
    pCHAR             names[2];
};


struct ParameterDoubleType
{
    enum E
    {
        Frequency,              // �������
        Period,                 // ������
        Amplitude,              // ���������
        Offset,                 // ��������
        Duration,               // ������������
        DutyRatio,              // ����������
        Phase,                  // ����� ����
        Delay,                  // ��������
        DurationRise,           // ������������ ����������
        DurationFall,           // ������������ �����
        DurationStady,          // ������������ ������������� ��������
        DutyFactor,             // ����������� ����������
        ManipulationDuration,   // ������������ ��������� � ������ �����������
        ManipulationPeriod,     // ������ ���������� ��������� � ������ �����������
        PacketPeriod,           // ������ ���������� ����� ��������� � �������� ������
        Count
    };
};


class ParameterDouble : public Parameter
{
    friend struct LogicFloatValue;
    friend class Tuner;
    friend class TunerDisplay;

public:

    ParameterDouble(ParameterDoubleType::E t, pCHAR const nameRU, pCHAR const nameEN, const Value &_min, const Value &_max, const Value &_value);

    ParameterDouble(const ParameterDouble &);

    virtual void Reset();

    // ���������� �������� ��������� � ��������� ��� � ������
    bool SetAndLoadValue(double val);
    bool SetAndLoadValue(Value val);
    
    // ���������� true, ���� �������� ����� ����
    bool IsSigned() const { return (type == ParameterDoubleType::Offset); }

    // ���������� true, ���� �������� �� ����� �������� �����������
    bool IsNotOrdered() const;

    bool IsPhase() const;

    // ���������� true, ���� �������� ������ ����������� "�������"
    bool IsTime() const;

    virtual void OnPressButtonTune();

    // ���������� ��������� ������������� �������� ���������
    virtual String ToString(String &units) const;
    virtual String ToString(Value value) const;

    // ���������� �������� ������� ��������� (��� ����� �������)
    pString GetMainUnits() const;

    // ���������� ������� ���������, ���������� � ������� order. ���� order == Order::Count, ������� ����� �������������� ������ �� �������� �������� value
    pString GetUnits(Order::E order = Order::Count) const;

    // ���������� ������������ ��������, ������� ����� ����� ��������
    virtual Value GetMax() const { return max; }
    
    // ���������� ����������� ��������, ������� ����� ����� ��������
    virtual Value GetMin() const { return min; }

    virtual Tuner *GetTuner()   { return &tuner; };

    // ���������� ������� �������� ���������
    virtual Value GetValue() const { return value; };

    void SetValue(Value val) { value = val; }
     
    ParameterDoubleType::E GetType() const { return type; }

    virtual void SaveState();

    virtual void RestoreState();

    ParameterDouble &operator=(const ParameterDouble &);

private:
    Tuner tuner;        // ������������ ��� ��������� 
    const ParameterDoubleType::E type;
    Value min;
    Value value;
    Value resetValue;

    // ���������� true, ���� �������� ����� ��������� �������� v
    bool InRange(double v) const;
    bool InRange(Value v) const;

    // ��� ������ ��������� ����� ��������� ������� ��� ������������ ��������� ����� ������� ��� � ����� ���������
    void LoadNumberImpulsesIfNeed();

protected:
    Value max;

private:

    struct Saved
    {
        Saved(const Value &val) : value(val) { }
        Value value;
    } saved;
};


struct ParameterIntegerType
{
    enum E
    {
        PacketNumber,           // ����� ��������� � ����� ��������� ������
        Count
    };
};


class ParameterInteger : public Parameter
{
public:

    ParameterInteger(ParameterIntegerType::E t, const char * const nameRU, const char * const nameEN, const Value &min, const Value &max, const Value &value);

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
        Polarity,             // ���������� ���������
        ModeStart,            // ����� ������� �������� DDS � ����������
        ManipulationEnabled,  // ������� ��� �������� ����� �����������
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

    int GetChoice() const;

    void NextChoice();

//    void TuneCurrentChoice();

    bool SetAndLoadChoice(int ch);

    virtual String ToString(String &units) const;

    virtual String ToString(Value) const { return String(""); };

    virtual void OnPressButtonTune();

    virtual void Reset() { SetAndLoadChoice(0); };

private:
	ParameterChoiceType::E type;
    int choice;                         // ������� �����. � ����� ��� ������ ������� ���������� ��������
    static int choiceModeStartFree;     // ����� ������ ������� ������������ ��������
    const char **choices;               // ���� ��� - 0(���), 0(����), 1(���), 1(����)...
    
    // ���������� ��������� ������
    int NumChoices() const;

    // ��� ������� ���������� �� ��������� ������ ������� ������������� �������
    void NextChoiceModeStart();
};


struct ParameterCompositeType
{
    enum E
    {
        Manipulation,   // ��������� �������� / �������� / ����������� �� ����� �����
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
    
    Parameter **params; // ����� ��������� �������������� ��������� � ������, ���� ��� ���������
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
        ParameterVoltage(ParameterDoubleType::Amplitude, "������", "Amplitude", min, max, value) { }

    virtual Value GetMax() const;
};


class ParameterOffset : public ParameterVoltage
{
public:
    ParameterOffset(const Value &min = Value("-5", Order::One), const Value &max = Value("5", Order::One), const Value &value = Value("0", Order::One)) :
        ParameterVoltage(ParameterDoubleType::Offset, "��������", "Offset", min, max, value) { }

    virtual Value GetMax() const;
    virtual Value GetMin() const;
};


class ParameterFrequency : public ParameterDouble
{
public:
    ParameterFrequency(const Value &min, const Value &max, const Value &value = Value("1", Order::Kilo)) :
        ParameterDouble(ParameterDoubleType::Frequency, "�������", "Frequency", min, max, value) { }
};


class ParameterTime : public ParameterDouble
{
public:
    ParameterTime(ParameterDoubleType::E t, const char *nameRU, const char * const nameEN, const Value &min,
        const Value &max, const Value &value) : ParameterDouble(t, nameRU, nameEN, min, max, value) { }
};


class ParameterPhase : public ParameterDouble
{
public:
    ParameterPhase() : ParameterDouble(ParameterDoubleType::Phase, "����", "Phase", Value("0", Order::One),
        Value("360", Order::One), Value("0", Order::One)) { }
};


class ParameterPacketPeriod : public ParameterTime
{
public:
    ParameterPacketPeriod(const Value &max, const Value &value) : ParameterTime(ParameterDoubleType::PacketPeriod,
        "������ ���", "Packet per", IMPULSE_PERIOD_MIN, max, value) { }

    // ���� ������������� �������� �� ��������� ��������� � ���� ��� �������� ������, �� ��� ����� �����������
    // ���������� true, ���� �������� ����������
    bool RecalcualateValue();

    // ������������ ���������� ��������� �������� ��� ������ ����������
    Value CalculateMinValue() const;

    virtual Value GetMin() const { return CalculateMinValue(); }
};


class ParameterPeriod : public ParameterTime
{
public:
    ParameterPeriod(const Value &max, const Value &value, const char *nameRU = "������", const char * const nameEN = "Period") :
        ParameterTime(ParameterDoubleType::Period, nameRU, nameEN, IMPULSE_PERIOD_MIN, max, value) { }
};


class ParameterDuration : public ParameterTime
{
public:
    ParameterDuration(const Value &max, const Value &value, const char *nameRU = "����", const char *nameEN = "Dur") :
        ParameterTime(ParameterDoubleType::Duration, nameRU, nameEN, IMPULSE_PERIOD_MIN, max, value) { }
};


class ParameterManipulationDuration : public ParameterTime
{
public:
    ParameterManipulationDuration(const Value &min, const Value &max, const Value &value) :
        ParameterTime(ParameterDoubleType::ManipulationDuration, "����", "Duration", min, max, value) { }
};


class ParameterManipulationPeriod : public ParameterTime
{
public:
    ParameterManipulationPeriod(const Value &min, const Value &max, const Value &value) :
        ParameterTime(ParameterDoubleType::ManipulationPeriod, "������", "Period", min, max, value) { }
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Choice ///
class ParameterModeStart : public ParameterChoice
{
public:
    ParameterModeStart(const char **names) : ParameterChoice(ParameterChoiceType::ModeStart, "������", "Start", names) { }
};


class ParameterManipulationEnabled : public ParameterChoice
{
public:
    ParameterManipulationEnabled(const char **names) : 
        ParameterChoice(ParameterChoiceType::ManipulationEnabled, "�����", "Manip", names) { }

};


class ParameterPolarity : public ParameterChoice
{
public:
    ParameterPolarity(const char **names) : ParameterChoice(ParameterChoiceType::Polarity, "����������", "Polarity", names) { }
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Complex ///
class ParameterManipulation : public ParameterComposite
{
public:
    ParameterManipulation(Parameter **paramaters);
};

