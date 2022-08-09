// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Common.h"
#include "Signals/Tuner.h"
#include "Signals/Viewer.h"
#include "Signals/MathSupport.h"
#include "Settings/SettingsTypes.h"


class Form;
struct Key;
struct Param;


struct SMinMax
{
    SMinMax() : min(1), max(1), valid(true) { }
    Value min;      // ���������� ���������� ��������
    Value max;      // ����������� ���������� ��������
    bool valid;     // ���� true - ������� �������� ���������
};

typedef SMinMax (*pValueInRange)(Form *);

inline SMinMax EValueInRange(Form *) { return SMinMax(); }


#define DEFAULT_AMPLITUDE Value("1", Order::One)

#define IMPULSE_PERIOD_MIN   Value("20", Order::Nano)
#define IMPULSE_DURATION_MIN Value("10", Order::Nano)


struct KindParam
{
    enum E
    {
        Double,     // ��������, ���������� �������� ���������
        Choice,     // ����� �� ���������� ��������
        Composite,  // ��������� ��������, ��������� �� ���������� ������� (�����������)
        Button,     // ������ - �� � ������� ���������� �����-���� ��������
        Integer,    // ����� �����
        Count
    };
};


struct Param
{
    Param(KindParam::E k, pFuncBV funcActive, pchar nRU, pchar nEN);

    virtual ~Param() { }

    cstr Name() const;

    virtual void SetForm(Form *form);

    // ���������� ������� ������ "��������"
    virtual void OnPressButtonTune() = 0;

    virtual Tuner *GetTuner() { return nullptr; }

    void SetParent(Param *p);
    
    // ���������� ����� ������������� ���������
    Param *GetParent();

    bool IsDouble() const;
    bool IsInteger() const;
    bool IsComposite() const;
    bool IsChoice() const;

    virtual void Reset() = 0;

    // ���������� true, ���� �������� ������� � ������
    bool IsOpened() const;

    Form *GetForm();
    
    KindParam::E GetKind() { return kind; }

    Viewer viewer;

    // ��� "����� ������������" �������� ���������. ������ ��� ���� �� �����
    virtual Value GetMax() const { return Value("1", Order::One); }

    // ��� "����� �����������" �������� ���������. ������ ��� ���� �� �����
    virtual Value GetMin() const { return Value("0", Order::One); }

    virtual SMinMax ValueInRange() const { return EValueInRange(form); }

    virtual Value GetValue() const { return Value("1", Order::One); } //-V524

    virtual String ToString(String &) const { return String(""); };

    virtual String ToString(Value) const { return String(""); };

    // ��������� ��������� ���������
    virtual void StoreState() { };

    // ������������ ��������� ���������
    virtual void RestoreState() { };

    static bool FuncActive() { return true; }

    DParam *ReinterpretToDouble() { return (DParam *)this; }

    pFuncBV          funcOfActive;  // ������� �� ������ ��������

protected:
    
    Form         *form;         // �����, ��� ������� ���� ���� ��������
    Param        *parent;       // ���� �������� ���������, �� ����� ����� ��������
    KindParam::E  kind;
    pchar         names[2];
};


struct TypeDParam
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
        Delay,                  // �������� ����� �������� � � � ��� ���������� ������ �����/����
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


struct DParam : public Param
{
    friend struct LogicFloatValue;
    friend class Tuner;
    friend class TunerDisplay;

    DParam(TypeDParam::E t, pFuncBV funcActive, pchar const nameRU, pchar const nameEN,
                    const Value  &min,
                    const Value  &max,
                    pValueInRange valueInRange,
                    const Value  &value);

    DParam(const DParam &);

    virtual void Reset();

    // ���������� �������� ��������� � ��������� ��� � ������
    bool SetAndLoadValue(double val);
    bool SetAndLoadValue(Value val);
    
    // ���������� true, ���� �������� ����� ����
    bool IsSigned() const { return (type == TypeDParam::Offset); }

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
    cstr GetMainUnits() const;

    // ���������� ������� ���������, ���������� � ������� order. ���� order == Order::Count, ������� ����� �������������� ������ �� �������� �������� value
    cstr GetUnits(Order::E order = Order::Count) const;

    Value Max() const { return max; }

    // ���������� ������������ ��������, ������� ����� ����� ��������
    virtual Value GetMax() const { return max; }
    
    // ���������� ����������� ��������, ������� ����� ����� ��������
    virtual Value GetMin() const { return min; }

    // ���������� ��������, ������� ��������� ������������ � ����������� 
    virtual SMinMax ValueInRange() const { return valueInRange(form); }

    virtual Tuner *GetTuner()   { return &tuner; };

    // ���������� ������� �������� ���������
    virtual Value GetValue() const { return value; };

    void SetValue(Value val) { value = val; }
     
    TypeDParam::E GetType() const { return type; }

    virtual void StoreState();

    virtual void RestoreState();

    DParam &operator=(const DParam &);

    bool IsAmplitude() const { return type == TypeDParam::Amplitude; }

private:
    Tuner tuner;        // ������������ ��� ��������� 
    const TypeDParam::E type;
    Value         min;
    Value         max;
    pValueInRange valueInRange;
    Value         value;
    Value         resetValue;

    // ���������� true, ���� �������� ����� ��������� �������� v
    bool InRange(double v) const;
    bool InRange(Value v) const;

    // ��� ������ ��������� ����� ��������� ������� ��� ������������ ��������� ����� ������� ��� � ����� ���������
    void LoadNumberImpulsesIfNeed();

    struct Stored
    {
        Stored(const Value &val) : value(val) { }
        Value value;
    } stored;
};


struct TypeIParam
{
    enum E
    {
        PacketNumber,           // ����� ��������� � ����� ��������� ������
        Count
    };
};


struct IParam : public Param
{
    IParam(TypeIParam::E t, pchar  const nameRU, pchar const nameEN,
        const Value &min, const Value &max, pValueInRange, const Value &);

    virtual void Reset() { SetAndLoadValue(resetValue); }

    virtual String ToString(String &units) const;

    virtual void OnPressButtonTune();

    virtual Tuner *GetTuner()                 { return &tuner; }

    virtual Value GetValue() const            { return value; }

    Value Max() const { return max; }

    Value Min() const { return min; }

    virtual Value GetMax() const              { return max;   }

    virtual Value GetMin() const              { return min;   }

    virtual SMinMax ValueInRange() const { return valueInRange(form); }

    virtual String ToString(Value value) const;

    TypeIParam::E GetType()   { return type; }

    bool SetAndLoadValue(Value val);

    bool SetAndLoadValue(int val);

    void LoadValue();

    bool InRange(Value v) const;

private:

    Tuner tuner;
    TypeIParam::E type;
    Value         min;
    Value         max;
    pValueInRange valueInRange;
    Value         value;
    Value         resetValue;
};


struct TypeCParam
{
    enum E
    {
        Polarity,               // ���������� ���������
        ModeStart,              // ����� ������� �������� DDS � ����������
        ManipulationEnabled,    // ������� ��� �������� ����� �����������
        ModeStartStop,          // ����� �-�����/�-����
        ClockImpulse,           // �������� ������� � ������ ��������� � �������
        Count
    };

    static pchar Name(TypeCParam::E type);
};


struct CParam : public Param
{
    CParam(TypeCParam::E t, pFuncBV funcActive, pchar nameRU, pchar nameEN, pchar *_choices = nullptr) :
        Param(KindParam::Choice, funcActive, nameRU, nameEN), type(t), choice(0), choices(_choices) { }

    TypeCParam::E GetType() { return type; }

    int GetChoice() const;

    void NextChoice();

    bool SetAndLoadChoice(int ch);

    virtual String ToString(String &units) const;

    virtual String ToString(Value) const { return String(""); };

    virtual void OnPressButtonTune();

    virtual void Reset() { SetAndLoadChoice(0); };

private:
	TypeCParam::E type;
    int choice;                         // ������� �����. � ����� ��� ������ ������� ���������� ��������
    static int choiceModeStartFree;     // ����� ������ ������� ������������ ��������
    pchar *choices;               // ���� ��� - 0(���), 0(����), 1(���), 1(����)...
    
    // ���������� ��������� ������
    int NumChoices() const;
};


struct TypeCMSParam
{
    enum E
    {
        Manipulation,   // ��������� �������� / �������� / ����������� �� ����� �����
        Count
    };
};


struct CMSParam : public Param
{
    CMSParam(TypeCMSParam::E v, pchar nameRU, pchar nameEN, Param **parameters) :
        Param(KindParam::Composite, Param::FuncActive, nameRU, nameEN), params(parameters), type(v) { }

    virtual void SetForm(Form *form);

    virtual void Reset() { }

    int NumParameters() const;
    Param **Parameters() { return params; }

    DParam *FindParameter(TypeDParam::E p);
    CParam *FindParameter(TypeCParam::E p);

    virtual String ToString(String &units) const;

    virtual String ToString(Value) const { return String(""); };

    virtual void OnPressButtonTune();

    TypeCMSParam::E GetType() { return type; }

private:
    
    Param **params; // ����� ��������� �������������� ��������� � ������, ���� ��� ���������
    TypeCMSParam::E type;
};


struct BParam : public Param
{
    BParam(pchar titleRU, pchar titleEN, pFuncVV f) :
        Param(KindParam::Button, Param::FuncActive, titleRU, titleEN), func(f) {};

    virtual String ToString(String &) const { return String(""); };

    virtual String ToString(Value) const { return String(""); };

    virtual void OnPressButtonTune() { func(); };

    virtual void Reset() {}

private:

    pFuncVV func;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Value ///
struct ParameterVoltage : public DParam
{
    ParameterVoltage(TypeDParam::E type, pchar nameRU, pchar nameEN,
                     const Value &min,
                     const Value &max,
                     pValueInRange valueInRange,
                     const Value &value) :
        DParam(type, Param::FuncActive, nameRU, nameEN, min, max, valueInRange, value) { }
};


struct ParameterAmplitude : public ParameterVoltage
{
    ParameterAmplitude(const Value &min = Value("0", Order::One),
                       const Value &max = Value("10", Order::One),
                       pValueInRange valueInRange = EValueInRange,
                       const Value &value = DEFAULT_AMPLITUDE) :
        ParameterVoltage(TypeDParam::Amplitude, "������", "Amplitude", min, max, valueInRange, value) { }

    virtual Value GetMax() const;
};


struct ParameterOffset : public ParameterVoltage
{
    ParameterOffset(pValueInRange valueInRange = EValueInRange,
                    const Value &min = Value("-5", Order::One),
                    const Value &max = Value("5", Order::One),
                    const Value &value = Value("0", Order::One)) :
        ParameterVoltage(TypeDParam::Offset, "��������", "Offset", min, max, valueInRange, value) { }
};


struct ParameterFrequency : public DParam
{
    ParameterFrequency(const Value &min,
                       const Value &max,
                       pValueInRange valueInRange = EValueInRange,
                       const Value &value = Value("1", Order::Kilo)) :
        DParam(TypeDParam::Frequency, Param::FuncActive, "�������", "Frequency", min, max, valueInRange, value) { }
};


struct ParameterTime : public DParam
{
    ParameterTime(TypeDParam::E t, pFuncBV funcActive, pchar nameRU, pchar  const nameEN,
                  const Value &min,
                  const Value &max,
                  pValueInRange valueInRange,
                  const Value &value) : DParam(t, funcActive, nameRU, nameEN, min, max, valueInRange, value) { }
};


struct ParameterPhase : public DParam
{
    ParameterPhase() : DParam(TypeDParam::Phase, Param::FuncActive, "����", "Phase",
                                       Value("0", Order::One),
                                       Value("360", Order::One),
                                       EValueInRange,
                                       Value("0", Order::One)) { }
};


struct ParameterPacketPeriod : public ParameterTime
{
    ParameterPacketPeriod(const Value &max, const Value &value) :
        ParameterTime(TypeDParam::PacketPeriod, Param::FuncActive, "������ ���", "Packet per", IMPULSE_PERIOD_MIN, max, EValueInRange, value) { }

    // ���� ������������� �������� �� ��������� ��������� � ���� ��� �������� ������, �� ��� ����� �����������
    // ���������� true, ���� �������� ����������
    bool RecalcualateValue();

    // ������������ ���������� ��������� �������� ��� ������ ����������
    Value CalculateMinValue() const;

    virtual Value GetMin() const { return CalculateMinValue(); }
};


struct ParameterPeriod : public ParameterTime
{
    ParameterPeriod(pFuncBV funcActive, const Value &max, const Value &value, pchar nameRU = "������", pchar  const nameEN = "Period") :
        ParameterTime(TypeDParam::Period, funcActive, nameRU, nameEN, IMPULSE_PERIOD_MIN, max, EValueInRange, value) { }
};


struct ParameterDuration : public ParameterTime
{
    ParameterDuration(const Value &max, const Value &value, pchar nameRU = "����", pchar nameEN = "Dur") :
        ParameterTime(TypeDParam::Duration, Param::FuncActive, nameRU, nameEN, IMPULSE_PERIOD_MIN, max, EValueInRange, value) { }
};


struct ParameterDelay : public ParameterTime
{
    ParameterDelay(pFuncBV funcActive, const Value &max, const Value &value, pchar nameRU = "��������", pchar nameEN = "Delay") :
        ParameterTime(TypeDParam::Delay, funcActive, nameRU, nameEN, IMPULSE_PERIOD_MIN, max, EValueInRange, value) { }
};


struct ParameterManipulationDuration : public ParameterTime
{
    ParameterManipulationDuration(const Value &min,
                                  const Value &max,
                                  pValueInRange valueInRange,
                                  const Value &value) :
        ParameterTime(TypeDParam::ManipulationDuration, Param::FuncActive, "����", "Duration", min, max, valueInRange, value) { }
};


struct ParameterManipulationPeriod : public ParameterTime
{
    ParameterManipulationPeriod(const Value &min,
                                const Value &max,
                                pValueInRange valueInRange,
                                const Value &value) :
        ParameterTime(TypeDParam::ManipulationPeriod, Param::FuncActive, "������", "Period", min, max, valueInRange, value) { }
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Choice ///
struct ParameterModeStart : public CParam
{
    ParameterModeStart(pFuncBV funcActive, pchar *names) : CParam(TypeCParam::ModeStart, funcActive, "������", "Start", names) { }
};


struct ParameterModeStartStop : public CParam
{
    ParameterModeStartStop(pFuncBV funcActive, pchar *names) : CParam(TypeCParam::ModeStartStop, funcActive, "�-�����,�-����", "A-Start,B-Stop", names) { }
};


struct ParameterManipulationEnabled : public CParam
{
    ParameterManipulationEnabled(pchar *names) : 
        CParam(TypeCParam::ManipulationEnabled, Param::FuncActive, "�����", "Manip", names) { }

};


struct ParameterPolarity : public CParam
{
    ParameterPolarity(pchar *names) : CParam(TypeCParam::Polarity, Param::FuncActive, "����������", "Polarity", names) { }
};


struct ParameterClockImpulse : public CParam
{
    ParameterClockImpulse(pchar *names) : CParam(TypeCParam::ClockImpulse, Param::FuncActive, "��. �������", "Clock", names) { }
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Complex ///
struct ParameterManipulation : public CMSParam
{
    ParameterManipulation(Param **paramaters);
};

