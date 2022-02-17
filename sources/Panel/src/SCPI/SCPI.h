#pragma once
#include "common/CommonTypes.h"
#include "Generator/Wave.h"
#include "SCPI/MacrosesSCPI.h"

/*
    ������ �������.
    1. ������� ������ ���������� � ������� ':'.
    2. ���� ����������� ��������� ':'.
    3. ������� ����������� ������ ����� ���������� � ���������� 1 ��.
    4. ������� ������ ������������� �������� � ����� 0x0D.
*/

class ParameterDouble;
class String;
class SimpleMessage;

typedef const char *(*FuncSCPI)(pCHAR);
typedef bool (*FuncTestSCPI)();
typedef void (*FuncHint)(String *);


// ���������, �������������� ���� ������.
struct StructSCPI
{
    const char *key;            // �������� ����� ���� (�������)

    const StructSCPI *strct;    // ���� ��������� ����� ��� Node, �� ����� �������� ������ �������� - StructSCPI *structs.

    FuncSCPI  func;             // ���� ��������� ����� ��� Leaf, �� ����� �������� ������� - ���������� ����� ���� FuncSCPI

    const char *hint;

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

    void AppendNewData(pCHAR buffer, uint length);

    void Update();
    // ���������� true, ���� ��������� ��������� �� ����������� ������������������
    bool IsLineEnding(pCHAR *bufer);
    // ������� �����
    void SendAnswer(pCHAR message);
    // ���� ������ buffer ���������� � ������������������ �������� word, �� ���������� ��������� �� ������, ��������� �� ��������� �������� ������������������ word.
    // ����� ���������� nullptr.
    pCHAR BeginWith(pCHAR buffer, pCHAR word);
    // ������� ��������� �� ��������� ��������, ���� ������� �������
    void SendBadSymbols();

    void ProcessHint(String *message, pString names[]); //-V2504

    void ProcessRequestParameterValue(const ParameterDouble *param);

    void ProcessRequestParameterValue(const ParameterInteger *param);

    pCHAR ProcessParameterDouble(pCHAR buffer, ParameterDoubleType::E value);

    pCHAR ProcessParameterInteger(pCHAR buffer, ParameterIntegerType::E value);

    pCHAR ProcessParameterChoice(pCHAR buffer, ParameterChoiceType::E choice, pString *names);

    namespace Handler
    {
        bool Processing(SimpleMessage *message);
    }
};
