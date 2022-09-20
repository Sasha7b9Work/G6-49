// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Common.h"
#include "Signals/Tuner.h"
#include "Signals/Viewer.h"
#include "Settings/SettingsTypes.h"


class Form;
struct Key;
struct Param;


typedef bool (*pFuncActive)();


struct SMinMax
{
    SMinMax(bool val) : min(1), max(1), valid(val) { }
    SMinMax(const Value &_min, const Value &_max, const Value &_value) : min(_min), max(_max)
    {
        valid = (_value >= min) && (_value <= max);
    }

    Value min;      // ���������� ���������� ��������
    Value max;      // ����������� ���������� ��������
    bool valid;     // ���� true - ������� �������� ���������
};


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
    Param(KindParam::E, pFuncActive, pchar nRU, pchar nEN);

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

    Form *GetForm() const { return form; };

    KindParam::E GetKind() const { return kind; }

    Viewer viewer;

    // ��� "����� ������������" �������� ���������. ������ ��� ���� �� �����
    virtual Value Max() const { return Value("1", Order::One); }

    // ��� "����� �����������" �������� ���������. ������ ��� ���� �� �����
    virtual Value Min() const { return Value("0", Order::One); }

    virtual SMinMax ValueInRange() const;

    virtual Value GetValue() const { return Value("1", Order::One); } //-V524

    virtual String ToString(String &units) const { return units; };

    // delete_zeros - ���� true - ������� ��������� ����
    virtual String ToString(Value, bool /* delete_zeros */ = false) const { return String(""); };

    // ��������� ��������� ���������
    virtual void StoreState() { };

    // ������������ ��������� ���������
    virtual void RestoreState() { };

    static bool EFuncActive() { return true; }

    DParam *ToDouble();
    IParam *ToInteger();

    pFuncActive   funcOfActive;  // ������� �� ������ ��������

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
        Phase,                  // ����� ����
        Delay,                  // �������� ����� �������� � � � ��� ���������� ������ �����/����
        DurationManipulation,   // ������������ ��������� � ������ �����������
        PeriodManipulation,     // ������ ���������� ��������� � ������ �����������
        PeriodPacket,           // ������ ���������� ����� ��������� � �������� ������
        Count
    };
};


// Double
struct DParam : public Param
{
    friend struct LogicFloatValue;
    friend class Tuner;
    friend class TunerDisplay;

    DParam(TypeDParam::E t, pFuncActive, pchar const nameRU, pchar const nameEN, const Value &);

    DParam(const DParam &);

    virtual void Reset();

    // ���������� �������� ��������� � ��������� ��� � ������
    bool SetAndLoadToGenerator(double);
    bool SetAndLoadToGenerator(const Value &);

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

    virtual Value Min() const { return Value(-1); };
    virtual Value Max() const { return Value(1); };
    virtual Value GetValue() const { return value; };

    virtual Tuner *GetTuner() { return &tuner; };

    void SetValue(Value val) { value = val; }

    TypeDParam::E GetType() const { return type; }

    virtual void StoreState();

    virtual void RestoreState();

    DParam &operator=(const DParam &);

    bool IsAmplitude() const { return type == TypeDParam::AmplitudePic; }
    bool IsFrequency() const { return type == TypeDParam::Frequency; }
    bool IsOffset()    const { return type == TypeDParam::Offset; }

    // ���������� ���������� �������� ��������� ����� ������� (�������� �������� ������). � �������������, ��� �������� ��������� � ������� order
    int GetNumberDigitsBeforeComma(Order::E order = Order::Count) const;

    // ���������� ���������� �������� ��������� ����� �������
    int GetNumberDigitsAfterComma(Order::E order = Order::Count) const;

    // ���������� �������� ������������� Value � numDigits ��������� ������� � �������.
    cstr GetIndicatedValue() const;

    // ���������� ������� ������� ���������� ������� "1" - ������� (1e1), "0" - ������� (1e0), "-1" - ������� (1e-1), "-2" - ����� (1e-2)
    int GetPositionFirstDigit(Order::E order = Order::Count) const;

    static DParam empty;

    bool Exist() const { return this != &empty; }

private:
    Tuner               tuner;          // ������������ ��� ��������� 
    const TypeDParam::E type;
    Value               value;
    Value               resetValue;

    // ��� ������ ��������� ����� ��������� ������� ��� ������������ ��������� ����� ������� ��� � ����� ���������
    void LoadNumberImpulsesIfNeed();

    struct Stored
    {
        Stored(const Value &val) : value(val) { }
        Value value;
    } stored;

public:

    // ��� �������� ������������ ��� ���������� �������� �� ���� �� ������ ���������
    struct PrevOrder
    {
        PrevOrder() : order(Order::One) {}
        void Set(Order::E _order) { order = _order; }
        Order::E Get()            { return order; };
    private:
        Order::E order;
    } prev_order;
};


struct TypeIParam
{
    enum E
    {
        PacketNumber,           // ����� ��������� � ����� ��������� ������
        Count
    };
};


// Integer
struct IParam : public Param
{
    IParam(TypeIParam::E t, pchar  const nameRU, pchar const nameEN, const Value &);

    virtual void Reset() { SetAndLoadToGenerator(resetValue); }

    virtual String ToString(String &units) const;
    virtual String ToString(Value value, bool delete_zeros = false) const;

    virtual void OnPressButtonTune();

    virtual Tuner *GetTuner() { return &tuner; }

    virtual Value Min() const;
    virtual Value Max() const;
    virtual Value GetValue() const { return value; }

    TypeIParam::E GetType() const { return type; }

    bool SetAndLoadToGenerator(const Value &val);

    bool SetAndLoadToGenerator(const int val);

    void LoadToGenerator() const;

    // ���������� ���������� �������� ��� ��������� parameter (� ��� ������ ���������� ����������� ��������� �������� ��������� parameter)
    int GetMaxNumberDigits() const;

    static IParam empty;

    bool Exist() const { return this != &empty; }

private:

    Tuner         tuner;
    TypeIParam::E type;
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
        Count
    };

    static pchar Name(TypeCParam::E type);
};


// Choice
struct CParam : public Param
{
    CParam(TypeCParam::E t, pFuncActive funcActive, pchar nameRU, pchar nameEN, pchar *_choices = nullptr) :
        Param(KindParam::Choice, funcActive, nameRU, nameEN), type(t), choice(0), choices(_choices) { }

    TypeCParam::E GetType() const { return type; }

    int GetChoice() const;

    void NextChoice();

    bool SetAndLoadChoice(int ch);

    virtual String ToString(String &units) const;
    virtual String ToString(Value, bool /*delete_zeros*/ = false) const { return String(""); };

    virtual void OnPressButtonTune();

    virtual void Reset() { SetAndLoadChoice(0); };

    static CParam empty;

    bool Exist() const { return this != &empty; }

private:
    TypeCParam::E  type;
    int            choice;                  // ������� �����. � ����� ��� ������ ������� ���������� ��������
    static int     choiceModeStartFree;     // ����� ������ ������� ������������ ��������
    pchar         *choices;                 // ���� ��� - 0(���), 0(����), 1(���), 1(����)...

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

// Composite
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

    static CMSParam empty;

    bool Exist() const { return this != &empty; }

private:

    Param **params; // ����� ��������� �������������� ��������� � ������, ���� ��� ���������
    TypeCMSParam::E type;
};


// Button
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
    PVoltage(TypeDParam::E type, pchar nameRU, pchar nameEN, const Value &value) :
        DParam(type, Param::EFuncActive, nameRU, nameEN, value) { }
};


struct PAmplitudePic : public PVoltage
{
    PAmplitudePic(const Value &value = PAmplitudePic::by_default) : PVoltage(TypeDParam::AmplitudePic, "������", "Amplitude", value) { }

    static const Value by_default;

    virtual Value Min() const;
    virtual Value Max() const;
};


struct POffset : public PVoltage
{
    POffset(const Value &value = Value("0", Order::One)) : PVoltage(TypeDParam::Offset, "��������", "Offset", value) { }

    // ���������� ��������� �������� ��� ����� �����
    virtual Value AbsLimit() const;

    virtual Value Min() const;
    virtual Value Max() const;
};


struct PFrequency : public DParam
{
    PFrequency(const Value &value = Value("1", Order::Kilo)) : DParam(TypeDParam::Frequency, Param::EFuncActive, "�������", "Frequency", value) { }

    static const Value min_sin;
    static const Value max_sin;
    static const Value min_DDS;
    static const Value max_DDS;

    virtual Value Min() const;
    virtual Value Max() const;
};


struct PTime : public DParam
{
    PTime(TypeDParam::E t, pFuncActive funcActive, pchar nameRU, pchar  const nameEN, const Value &value) :
        DParam(t, funcActive, nameRU, nameEN, value) { }
};


struct PPhase : public DParam
{
    PPhase() : DParam(TypeDParam::Phase, Param::EFuncActive, "����", "Phase", Value("0", Order::One)) { }

    virtual Value Min() const;
    virtual Value Max() const;
};


struct PPeriod : public PTime
{
    PPeriod(pFuncActive funcActive, const Value &value, pchar nameRU = "������", pchar  const nameEN = "Period") :
        PTime(TypeDParam::Period, funcActive, nameRU, nameEN, value) { }

    static const Value min_impulse;

    virtual Value Min() const;
    virtual Value Max() const;
};


struct PPeriodPacket : public PTime
{
    PPeriodPacket(pFuncActive funcActive, const Value &value) : PTime(TypeDParam::PeriodPacket, funcActive, "������ ���", "Packet per", value) { }

    // ���� ������������� �������� �� ��������� ��������� � ���� ��� �������� ������, �� ��� ����� �����������
    // ���������� true, ���� �������� ����������
    bool RecalcualateValue();

    virtual Value Min() const;
    virtual Value Max() const;
};


struct PDuration : public PTime
{
    PDuration(const Value &value, pchar nameRU = "����", pchar nameEN = "Dur") : PTime(TypeDParam::Duration, Param::EFuncActive, nameRU, nameEN, value) { }

    virtual Value Min() const;
    virtual Value Max() const;
};


struct PDelay : public PTime
{
    PDelay(pFuncActive funcActive, const Value &value, pchar nameRU = "��������", pchar nameEN = "Delay") : PTime(TypeDParam::Delay, funcActive, nameRU, nameEN, value) { }

    virtual Value Min() const;
    virtual Value Max() const;
};


struct PDurationManipulation : public PTime
{
    PDurationManipulation(const Value &value) : PTime(TypeDParam::DurationManipulation, Param::EFuncActive, "����", "Duration", value) { }

    virtual Value Min() const;
    virtual Value Max() const;
};


struct PPeriodManipulation : public PTime
{
    PPeriodManipulation(const Value &value) : PTime(TypeDParam::PeriodManipulation, Param::EFuncActive, "������", "Period", value) { }

    virtual Value Min() const;
    virtual Value Max() const;
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Choice ///
struct PModeStart : public CParam
{
    PModeStart(pFuncActive funcActive, pchar *names) : CParam(TypeCParam::ModeStart, funcActive, "������", "Start", names) { }
};


struct PModeStartStop : public CParam
{
    PModeStartStop(pFuncActive funcActive, pchar *names) : CParam(TypeCParam::ModeStartStop, funcActive, "�-�����,�-����", "A-Start,B-Stop", names) { }
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


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Complex ///
struct PManipulation : public CMSParam
{
    PManipulation(Param **paramaters);
};

