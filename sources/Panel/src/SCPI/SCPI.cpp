// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Messages.h"
#include "Display/Symbols.h"
#include "Hardware/VCP_p.h"
#include "Signals/Signals.h"
#include "SCPI/HeadSCPI.h"
#include "SCPI/SCPI.h"
#include "Utils/Buffer.h"
#include "Utils/StringUtils.h"
#include <cstring>


// ����������� ������� ��������� ������� �������� StructSCPI.
// � ������ ��������� ���������� ���������� ����� �������, �������������� �� ��������� ������������ ��������.
// � ������ ����������� ���������� - ���������� nullptr. ��� ������ ��������� � *error
static pchar Process(pchar buffer, const StructSCPI structs[]); //-V2504
// ��������� ���� ������ node
static pchar ProcessNode(pchar begin, const StructSCPI *node);
// ��������� ����� node
static pchar ProcessLeaf(pchar begin, const StructSCPI *node);
// ���������� true, ���� ������ �������� ������� ������� - ������������ ��� '*'
static bool IsBeginCommand(const char &symbol);
// ������� ������������ ������� �� ������ ������
static void RemoveBadSymbolsFromBegin();
// ������� ������������������ ������������ �� ������ ������ �� ���������� ����������
static bool RemoveSeparatorsSequenceFromBegin();
// ������� ��� ������� �� ������� �����������
static bool RemoveSymbolsBeforeSeparator();

static String data;

static String badSymbols;


void SCPI::AppendNewData(pchar buffer, uint size)
{
    data.Append(buffer, size);

    SU::ToUpper(data.c_str());

    RemoveBadSymbolsFromBegin();

    if (data.Size() == 0)
    {
        SendBadSymbols();
    }
}


void SCPI::Update()
{
    RemoveBadSymbolsFromBegin();

    if(data.Size() == 0)
    {
        SendBadSymbols();
        return;
    }

    pchar end = Process(data.c_str(), head);

    if(end)
    {
        data.RemoveFromBegin((uint)(end - data.c_str()));
    }
}


static pchar Process(pchar buffer, const StructSCPI strct[]) //-V2504
{
    while (!strct->IsEmpty())
    {
        pchar end = SCPI::BeginWith(buffer, strct->key);

        if (end)
        {
            if (strct->IsNode())
            {
                return ProcessNode(end, strct);
            }
            else if (strct->IsLeaf())
            {
                return ProcessLeaf(end, strct);
            }
            else
            {
                // ����� ������
            }
        }

        strct++;
    }

    badSymbols.Append(*buffer);         // ��������� ��� ����� � strct � �� ����� �� ������ ������������. ������� �������� ��������� ����������� � ����������� ��������

    return buffer + 1;
}


pchar SCPI::BeginWith(pchar buffer, pchar word)
{
    while (*word)
    {
        if (*buffer == '\0')
        {
            return nullptr;
        }

        if (*word == *buffer)
        {
            ++word;
            ++buffer;
        }
        else
        {
            break;
        }
    }

    return (*word == '\0') ? buffer : nullptr;
}


static pchar ProcessNode(pchar begin, const StructSCPI *node)
{
    return Process(begin, node->strct);
}


static pchar ProcessLeaf(pchar begin, const StructSCPI *node)
{
    if (*begin == '\0')                     // ������������ �� ����, ��� ������ ��������� ������� �� ������
    {
        return nullptr;
    }

    pchar result = node->func(begin);

    if (result)
    {
        return result;
    }

    badSymbols.Append(*begin);

    return begin + 1;
}


bool SCPI::IsLineEnding(pchar *buffer)
{
    bool result = (**buffer == 0x0D);

    if (result)
    {
        *(*buffer)++; //-V532
    }

    return result;
}


void SCPI::SendBadSymbols()
{
    if (badSymbols.Size())
    {
        String message("!!! ERROR !!! Invalid sequency : %s", badSymbols.c_str());
        SCPI::SendAnswer(message.c_str());
        badSymbols.Free();
    }
}


static void RemoveBadSymbolsFromBegin()
{
    while (RemoveSymbolsBeforeSeparator() || RemoveSeparatorsSequenceFromBegin())  { }
}


static bool RemoveSymbolsBeforeSeparator()
{
    bool result = false;

    while (data.Size() != 0 && !IsBeginCommand(data[0]))
    {
        badSymbols.Append(data[0]);
        data.RemoveFromBegin(1);
        result = true;
    }

    return result;
}


static bool RemoveSeparatorsSequenceFromBegin()
{
    bool result = false;

    while (data.Size() > 1 && IsBeginCommand(data[0]) && IsBeginCommand(data[1]))
    {
        badSymbols.Append(data[0]);
        data.RemoveFromBegin(1);
        result = true;
    }

    return result;
}


void SCPI::SendAnswer(pchar message)
{
    if(message[std::strlen(message) - 1] != 0x0D)
    {
        String msg(message);
        msg.Append(0x0D);
        PVCP::SendStringAsynch(msg.c_str());
    }
    else
    {
        PVCP::SendStringAsynch(message);
    }
}


static bool IsBeginCommand(const char &symbol)
{
    return (symbol == SCPI::SEPARATOR) || (symbol == '*');
}


void SCPI::ProcessHint(String *message, cstr names[]) //-V2504
{
    message->Append(" {");
    for(int i = 0; i < names[i][0] != 0; i++)
    {
        message->Append(names[i]);
        message->Append(" |");
    }
    message->RemoveEndSymbol();
    message->Append('}');
    SCPI::SendAnswer(message->c_str());
}


bool SCPI::Handler::Processing(SimpleMessage *message)
{
    uint size = message->TakeUINT();

    SCPI::AppendNewData((pchar)message->TakeData(5), size);

    return true;
}


pchar SCPI::ProcessParameterDouble(pchar buffer, TypeDParam::E value)
{
    DParam *param = CURRENT_FORM->FindParameter(value);

    if (!param)
    {
        return nullptr;
    }

    SCPI_REQUEST(SCPI::ProcessRequestParameterValue(param));

    buffer++;

    char *end_str = nullptr;

    Value paramValue(0);

    if (paramValue.FromString(buffer, &end_str, param->IsNotOrdered() ? 3 : 100))
    {
        if (param->SetAndLoadToGenerator(paramValue))
        {
            return end_str + 1;
        }
    }

    return nullptr;
}


pchar SCPI::ProcessParameterInteger(pchar buffer, TypeIParam::E type)
{
    IParam *param = CURRENT_FORM->FindParameter(type);

    if (!param)
    {
        return nullptr;
    }

    SCPI_REQUEST(SCPI::ProcessRequestParameterValue(param));

    buffer++;

    int paramValue = 0;

    char *end_str = nullptr;

    if (SU::String2Int(buffer, &paramValue, &end_str))
    {
        if (param->SetAndLoadToGenerator(paramValue))
        {
            return end_str + 1;
        }
    }

    return nullptr;
}


pchar SCPI::ProcessParameterChoice(pchar buffer, TypeCParam::E choice, cstr *names)
{
    CParam *param = CURRENT_FORM->FindParameter(choice);

    if(!param)
    {
        String answer("%s parameter not found for the current signal", TypeCParam::Name(choice));
        return nullptr;
    }

    SCPI_REQUEST(SCPI::SendAnswer(names[param->GetChoice()]));

    SCPI_PROCESS_ARRAY(names, param->SetAndLoadChoice(i));
}


void SCPI::ProcessRequestParameterValue(const DParam *param)
{
    if(!param)
    {
        SCPI_SEND_PARAMETER_DOES_NOT_EXIST();
    }
    else
    {
        uint8 lang = LANGUAGE;
        LANGUAGE = 1; //-V519

        String units;
        String answer = param->ToString(units);
        answer.Append(" ");
        answer.Append((units[0] == Ideograph::_8::Degree) ? "degrees" : units.c_str());

        SCPI::SendAnswer(answer.c_str());

        LANGUAGE = lang;
    }
}


void SCPI::ProcessRequestParameterValue(const IParam *param)
{
    if (!param)
    {
        SCPI_SEND_PARAMETER_DOES_NOT_EXIST();
    }
    else
    {
        uint8 lang = LANGUAGE;
        LANGUAGE = 1;

        String units;
        String answer = param->ToString(units);

        SCPI::SendAnswer(answer.c_str());

        LANGUAGE = lang;
    }
}
