// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Display/Colors.h"
#include "Display/Text.h"
#include "Keyboard/Controls.h"
#include "Utils/Stack.h"


class DisplayCorrection;
struct Param;
struct DParam;
struct IParam;
class Tuner;


class Digit
{
public:

    static const char COMMA = '.';

    Digit(char v = '0') : value(v) { }

    bool IsNumber() const  { return (value >= '0' && value <= '9'); }

    bool IsEmpty() const   { return (value == '\0'); }

    bool IsSigned() const  { return (value == '-') || (value == '+'); }

    char value;

    bool operator !=(char symbol) const { return value != symbol; }

    bool operator ==(char symbol) const { return value == symbol; }
};


class BufferDigit
{
public:
    const Digit &operator[](int i) const;

    Digit &operator[](int i);

    void Fill(char symbol);

    int Size() const { return MAX_NUM_DIGITS; }

private:

    static const int MAX_NUM_DIGITS = 25;

    Digit digits[MAX_NUM_DIGITS];       // ����� ��������� ��� �������

    static Digit null_digit;
};


// ���������� ����������, ����������� � �����������, � ������� ��� ������������� ��������� ����������
class Indicator
{
    friend class DisplayCorrection;

public:

    Indicator(DisplayCorrection *);

    int Draw(int x, int y, int width, const cstr units) const;

    bool OnControlKey(const Control &);

    void InitHighlight();

private:

    BufferDigit digits;

    int indexHighlight;                     // ������ ������������� ����������. ���� ������ ������ �� �������� � �������� ��������

    DisplayCorrection *display;

    // ���������� ������ ���������
    void HighlightSymbol(int x, int y, const Color &) const;

    // ���������� �������������� ������ ������������� ������� (����� ������� � ������� digits)
    int IndexHighlightReal() const;

    // ���������� ��������, ������� ����� ���� ����������. �� ����������� �������
    int NumberHighligthingDigits() const;

    // ���������� true, ���� � ������� pos - ���������� �����
    bool CommaInPosition(int pos) const;

    // ���������� ������� ������� � ������
    int PositionComma() const;

    // ���������� true, ���� � ������� pos - ������ �������� �����
    bool FirstSignedDigitInPosition(int pos) const;

    // ���������� ������� ���������� ����� ��������� ���������� �� pos. -1, ���� �������� �� �������.
    int FindPositionLeftDigit(int pos) const;

    // ������� �������� ������ ����� �� ��������
    void HighlightToLeft();

    // ������� �������� ������ ������ �� ��������
    void HighlightToRight();

    // ��������� �������� � ��������� �������
    void IncreaseInPosition(int pos);
    void IncreaseInPositionDouble(int pos);
    void IncreaseInPositionInteger(int pos);

    // ��������� �������� � ��������� �������
    void DecreaseInPosition(int pos);

    // ���������� true, ���� �������� �������� �� �������
    bool IsSigned() const;

    // ���������� "���� �������" � ������� pos
    Value StepPosition(int pos) const;

    // �������� ���� � ��������� �� ������. ���������� true, ���� pos == 0
    bool ChangeSign(Value *value, int pos);

    // ���������� ����, ������� ����� ������������ ������ � ������� pos. ���� test == true, �� ������� ������ ����� ���������� ���� ����
    Color CalculateColorDouble(int pos, bool test) const;

    Color CalculateColorInteger(int pos, bool test) const;

    // ���������� true, ���� ����� �������������� �������� ����� ���������
    int AdditionShiftForDigit(int pos) const;

    // ���������� true, ���� ��� �������� ������� ����� ����� ����
    bool AllNumbersOfLeftIsZero(int pos) const;

    // true, ���� ������ �� pos ��������� �����
    bool RightDigitIzComma(int pos) const;
    
    // ���������� true, ���� ��� �������� ������� ������ ����� ����
    bool AllNumberOfRightIsZero(int pos) const;

    int DrawDouble(int x, int y, int wdidth, const cstr units) const;

    int DrawDouble(int x, int y, const cstr units, bool test) const;

    int DrawInteger(int x, int y, int width) const;

    int DrawInteger(int x, int y, bool test) const;
};


// ����� ��� ����� � ����������� ���������
namespace DisplayEntering
{
    // ��������� ������� ������� ������ ��������� ��������
    bool OnEnteringKey(const Control &);

    void Draw(int x, int y, int width);

    // ���������� ������� ������ "��������� �������"
    void OnButtonOrderMore();

    // ���������� ������� ������ "��������� �������"
    void OnButtonOrderLess();

    void Init();

    // ���������� ��������, ������� �������� ������ � DisplayEntering ( c ������ ������� )
    Value ToValue();

    // ����� ��� �������� �������� ������ � ������ ����������������� �����
    class EnterBuffer
    {
    public:
        EnterBuffer() : stack(30), param(nullptr) { }
        void Prepare(DParam *parameter);
        void Push(const Key::E key);
        // ������� ��������� ����������� ������
        void Pop();
        int Size() const;
        char At(const int i) const;
        // ���������� true, ���� ���������� ���������� �����
        bool ConsistComma() const;
        String ToString() const;
        // ���������� true, ���� � ������ ��� ������, �����, ��������, �����
        bool IsEmpty() const;
        // ���������� ���������� ���� ����� ���������� �����
        int NumDigitsAfterComma() const;
        // ���������� ������� ���������� �����
        int PosComma() const;

    private:
        Stack<char> stack;
        DParam *param;
    };

    class Cursor
    {
    public:
        Cursor() : timeInit(0U) { }
        void Init();
        void Draw(int x, int y);
    private:
        uint timeInit;
    };
};


// ���������� ����������� Tuner
class DisplayCorrection
{
    friend class Tuner;

public:
   
    DisplayCorrection(Tuner *tuner);

    void Init();

    void Init(const Value &value);

    void Draw();

    bool OnControlKey(const Control &);

    // ����� ��������� � ������ �� ������� ����������� ���������, ���� ����������
    // ���������� false, ���� ������� �� ������� ��������� ���������� ��������
    static bool ShowMessageOutRangIfNeed(const Param *);

    // ������������, � ������ ������� ����� �������� �������� ��� ��������� (��� ������� � ������� ������� ���������, � ��� ��������� � ���������� - ����������, ������ Order::One)
    static Order::E CalculateOrderForIndication();

private:

    // ������ ���� ��� ������ ������ ���������
    static const int WIDTH_UNITS = 25;

    Tuner *tuner;

    Indicator indicator;

    void DrawTitle(int x, int y, int width);

    // ��������� ������� ����� ����� ���������
    void FillDigitsIntegerPartForDouble();

    // ��������� ������� ������� ����� ���������
    void FillDigitsFractPartForDouble();

    void FillDigitsForInteger();

    void InitDouble();

    void InitInteger();

    void DrawDouble(int x, int y);

    void DrawInteger(int x, int y);
};


// ������������ ��� ���������� ��������� ���������. �������� ��������������� ������ DParam
class Tuner
{
public:

    struct ModeTuning
    {
        enum E
        {
            Correction,     // ����� ���������� �������� ��������� (������)
            Entering        // ����� ����� ��������
        };
    };

    Param *param;           // ������������� ��������

    static Tuner *current;

    Tuner(Param *param);

    void Init();

    void Draw();

    bool OnControlKey(const Control &);

    // ���������� ������� ������ "������"
    void OnButtonCancel();

    // ���������� ������� ������ "���������"
    void OnButtonApply();

    static bool InModeCorrection() { return (mode == ModeTuning::Correction); }

    static bool InModeEntering()   { return (mode == ModeTuning::Entering);   }

    static void SetModeEntering();

    // ���������� true, ���� ������������� �������� - ��������
    bool IsOffset();

    // ���������� true, ���� ������������� �������� ����� ������� ��������� "�����"
    bool IsNotOrdered();

private:

    DisplayCorrection display;

    static ModeTuning::E mode;                         // ������� ����� ���������
};
