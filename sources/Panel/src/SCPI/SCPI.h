// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/CommonTypes.h"
#include "Signals/Wave.h"
#include "SCPI/MacrosesSCPI.h"

/*
    ������ �������.
    1. ������� ������ ���������� � ������� ':'.
    2. ���� ����������� ��������� ':'.
    3. ������� ����������� ������ ����� ���������� � ���������� 1 ��.
    4. ������� ������ ������������� �������� � ����� 0x0D.
*/

struct DParam;
class String;
struct SimpleMessage;

typedef pchar (*FuncSCPI)(pchar);
typedef bool (*FuncTestSCPI)();
typedef void (*FuncHint)(String *);


// ���������, �������������� ���� ������.
struct StructSCPI
{
    pchar key;            // �������� ����� ���� (�������)

    const StructSCPI *strct;    // ���� ��������� ����� ��� Node, �� ����� �������� ������ �������� - StructSCPI *structs.

    FuncSCPI  func;             // ���� ��������� ����� ��� Leaf, �� ����� �������� ������� - ���������� ����� ���� FuncSCPI

    pchar hint;

    FuncHint funcHint;

    bool IsEmpty() const { return key[0] == '\0'; };
    bool IsNode() const { return strct != nullptr; };   // ��������� �������� "�����" ������, ����� ���� ������ �� ������ ����� structs
    bool IsLeaf() const { return func != nullptr; };    // ��������� �������� "������" ������, ����� ��������� ������� func
};


namespace SCPI
{
    // ������-��������� ������ �������
    const char SEPARATOR = ':';

    const int SIZE_SEPARATOR = 1;

    void AppendNewData(pchar buffer, uint length);

    void Update();
    // ���������� true, ���� ��������� ��������� �� ����������� ������������������
    bool IsLineEnding(pchar *bufer);
    // ������� �����
    void SendAnswer(pchar message);
    // ���� ������ buffer ���������� � ������������������ �������� word, �� ���������� ��������� �� ������, ��������� �� ��������� �������� ������������������ word.
    // ����� ���������� nullptr.
    pchar BeginWith(pchar buffer, pchar word);
    // ������� ��������� �� ��������� ��������, ���� ������� �������
    void SendBadSymbols();

    void ProcessHint(String *message, cstr names[]); //-V2504

    void ProcessRequestParameterValue(const DParam *param);

    void ProcessRequestParameterValue(const ParameterInteger *param);

    pchar ProcessParameterDouble(pchar buffer, TypeDParam::E value);

    pchar ProcessParameterInteger(pchar buffer, TypeIParam::E value);

    pchar ProcessParameterChoice(pchar buffer, ParameterChoiceType::E choice, cstr *names);

    namespace Handler
    {
        bool Processing(SimpleMessage *message);
    }
};
