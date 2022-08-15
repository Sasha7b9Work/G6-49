// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Common.h"
#include "Signals/Tuner.h"
#include "Signals/Viewer.h"
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

    virtual String ToString(String &units) const { return units; };
    // bool - ���� true - ������� ��������� ����
    virtual String ToString(Value, bool = false) const { return String(""); };

    // ��������� ��������� ���������
    virtual void StoreState() { };

    // ������������ ��������� ���������
    virtual void RestoreState() { };

    static bool EFuncActive() { return true; }
    static SMinMax EValueInRange(Form *) { return SMinMax(); }

    DParam *ToDouble();

    pFuncBV     funcOfActive;  // ������� �� ������ ��������

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
        AmplitudePic,           // ������
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
        PeriodPacket,           // ������ ���������� ����� ��������� � �������� ������
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
    virtual String ToString(Value value, bool delete_zeros = false) const;

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

    bool IsAmplitude() const { return type == TypeDParam::AmplitudePic; }
    bool IsFrequency() const { return type == TypeDParam::Frequency; }

    // ���������� ���������� �������� ��������� ����� ������� (�������� �������� ������). � �������������, ��� �������� ��������� � ������� order
    int GetNumberDigitsBeforeComma(Order::E order = Order::Count) const;

    // ���������� ���������� �������� ��������� ����� �������
    int GetNumberDigitsAfterComma(Order::E order = Order::Count) const;

    // ���������� �������� ������������� Value � numDigits ��������� ������� � �������.
    cstr GetIndicatedValue() const;

    // ���������� ������� ������� ���������� ������� "1" - ������� (1e1), "0" - ������� (1e0), "-1" - ������� (1e-1), "-2" - ����� (1e-2)
    int GetPositionFirstDigit(Order::E order = Order::Count) const;

    static DParam empty;

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
    virtual String ToString(Value value, bool delete_zeros = false) const;

    virtual void OnPressButtonTune();

    virtual Tuner *GetTuner()            { return &tuner; }

    virtual Value GetValue() const       { return value; }

    Value Max() const                    { return max; }

    Value Min() const                    { return min; }

    virtual Value GetMax() const         { return max;   }

    virtual Value GetMin() const         { return min;   }

    virtual SMinMax ValueInRange() const { return valueInRange(form); }

    TypeIParam::E GetType()   { return type; }

    bool SetAndLoadValue(Value val);

    bool SetAndLoadValue(int val);

    void LoadValue();

    bool InRange(Value v) const;

    // ���������� ���������� �������� ��� ��������� parameter (� ��� ������ ���������� ����������� ��������� �������� ��������� parameter)
    int GetMaxNumberDigits() const;

private:

    Tuner         tuner;
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
    virtual String ToString(Value, bool /*delete_zeros*/ = false) const { return String(""); };

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
        Param(KindParam::Composite, Param::EFuncActive, nameRU, nameEN), params(parameters), type(v) { }

    virtual void SetForm(Form *form);

    virtual void Reset() { }

    int NumParameters() const;
    Param **Parameters() { return params; }

    DParam *FindParameter(TypeDParam::E p);
    CParam *FindParameter(TypeCParam::E p);

    virtual String ToString(String &units) const;
    virtual String ToString(Value, bool /*delete_zeros*/ = false) const { return String(""); };

    virtual void OnPressButtonTune();

    TypeCMSParam::E GetType() { return type; }

private:
    
    Param **params; // ����� ��������� �������������� ��������� � ������, ���� ��� ���������
    TypeCMSParam::E type;
};


struct BParam : public Param
{
    BParam(pchar titleRU, pchar titleEN, pFuncVV f) :
        Param(KindParam::Button, Param::EFuncActive, titleRU, titleEN), func(f) {};

    virtual String ToString(String &) const { return String(""); };
    virtual String ToString(Value, bool /*delete_zeros*/ = false) const { return String(""); };

    virtual void OnPressButtonTune() { func(); };

    virtual void Reset() {}

private:

    pFuncVV func;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Value ///
struct PVoltage : public DParam
{
    PVoltage(TypeDParam::E type, pchar nameRU, pchar nameEN,
             const Value &min,
             const Value &max,
             pValueInRange valueInRange,
             const Value &value) :
        DParam(type, Param::EFuncActive, nameRU, nameEN, min, max, valueInRange, value) { }
};


struct PAmplitudePic : public PVoltage
{
    PAmplitudePic(const Value &min = Value("0", Order::One),
                  const Value &max = Value("10", Order::One),
                  const Value &value = PAmplitudePic::by_default) :
        PVoltage(TypeDParam::AmplitudePic, "������", "Amplitude", min, max, PAmplitudePic::InRange, value) { }

    static SMinMax InRange(Form *);
    static Value  by_default;
};


struct POffset : public PVoltage
{
    POffset(const Value &min = Value("-5", Order::One),
            const Value &max = Value("5", Order::One),
            const Value &value = Value("0", Order::One)) :
        PVoltage(TypeDParam::Offset, "��������", "Offset", min, max, POffset::InRange, value) { }

    static SMinMax InRange(Form *);
};


struct PFrequency : public DParam
{
    PFrequency(const Value &min,
               const Value &max,
               const Value &value = Value("1", Order::Kilo)) :
        DParam(TypeDParam::Frequency, Param::EFuncActive, "�������", "Frequency", min, max, PFrequency::InRange, value) { }

    static SMinMax InRange(Form *);
};


struct PTime : public DParam
{
    PTime(TypeDParam::E t, pFuncBV funcActive, pchar nameRU, pchar  const nameEN,
          const Value &min,
          const Value &max,
          pValueInRange valueInRange, const Value &value) :
        DParam(t, funcActive, nameRU, nameEN, min, max, valueInRange, value) { }
};


struct PPhase : public DParam
{
    PPhase() : DParam(TypeDParam::Phase, Param::EFuncActive, "����", "Phase",
                      Value("0", Order::One),
                      Value("360", Order::One),
                      Param::EValueInRange,
                      Value("0", Order::One)) { }
};


struct PPeriod : public PTime
{
    PPeriod(pFuncBV funcActive, const Value &max, const Value &value, pchar nameRU = "������", pchar  const nameEN = "Period") :
        PTime(TypeDParam::Period, funcActive, nameRU, nameEN, PPeriod::impulseMin, max, EValueInRange, value) { }

    static Value impulseMin;
};


struct PPeriodPacket : public PTime
{
    PPeriodPacket(const Value &max, const Value &value) :
        PTime(TypeDParam::PeriodPacket, Param::EFuncActive, "������ ���", "Packet per", PPeriod::impulseMin, max, PPeriodPacket::InRange, value) { }

    // ���� ������������� �������� �� ��������� ��������� � ���� ��� �������� ������, �� ��� ����� �����������
    // ���������� true, ���� �������� ����������
    bool RecalcualateValue();

    // ������������ ���������� ��������� �������� ��� ������ ����������
    Value CalculateMinValue() const;

    virtual Value GetMin() const { return CalculateMinValue(); }

    static SMinMax InRange(Form *form);
};


struct PDuration : public PTime
{
    PDuration(const Value &max, const Value &value, pchar nameRU = "����", pchar nameEN = "Dur") :
        PTime(TypeDParam::Duration, Param::EFuncActive, nameRU, nameEN, PPeriod::impulseMin, max, EValueInRange, value) { }
};


struct PDelay : public PTime
{
    PDelay(pFuncBV funcActive, const Value &max, const Value &value, pchar nameRU = "��������", pchar nameEN = "Delay") :
        PTime(TypeDParam::Delay, funcActive, nameRU, nameEN, PPeriod::impulseMin, max, EValueInRange, value) { }
};


struct PManipulationDuration : public PTime
{
    PManipulationDuration(const Value &min,
                          const Value &max,
                          pValueInRange valueInRange,
                          const Value &value) :
        PTime(TypeDParam::ManipulationDuration, Param::EFuncActive, "����", "Duration", min, max, valueInRange, value) { }
};


struct PManipulationPeriod : public PTime
{
    PManipulationPeriod(const Value &min,
                        const Value &max,
                        pValueInRange valueInRange,
                        const Value &value) :
        PTime(TypeDParam::ManipulationPeriod, Param::EFuncActive, "������", "Period", min, max, valueInRange, value) { }
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Choice ///
struct PModeStart : public CParam
{
    PModeStart(pFuncBV funcActive, pchar *names) : CParam(TypeCParam::ModeStart, funcActive, "������", "Start", names) { }
};


struct PModeStartStop : public CParam
{
    PModeStartStop(pFuncBV funcActive, pchar *names) : CParam(TypeCParam::ModeStartStop, funcActive, "�-�����,�-����", "A-Start,B-Stop", names) { }
};


struct PManipulationEnabled : public CParam
{
    PManipulationEnabled(pchar *names) : 
        CParam(TypeCParam::ManipulationEnabled, Param::EFuncActive, "�����", "Manip", names) { }

};


struct PPolarity : public CParam
{
    PPolarity(pchar *names) : CParam(TypeCParam::Polarity, Param::EFuncActive, "����������", "Polarity", names) { }
};


struct PClockImpulse : public CParam
{
    PClockImpulse(pchar *names) : CParam(TypeCParam::ClockImpulse, Param::EFuncActive, "��. �������", "Clock", names) { }
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Complex ///
struct PManipulation : public CMSParam
{
    PManipulation(Param **paramaters);
};

