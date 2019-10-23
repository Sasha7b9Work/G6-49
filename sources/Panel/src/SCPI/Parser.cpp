#include "defines.h"
#include "SCPI/Parser.h"
#include "SCPI/SCPI.h"
#include "SCPI/Errors.h"



List<String> SCPI::Parser::words;



void SCPI::Parser::Parse()
{
    ClearList();

    if (Buffer::GetByte(0) == '*')
    {
        ParseStar();
    }
    else if (Buffer::GetByte(0) == ':')
    {
        ParseColon();
    }
    else
    {
        Error::Syntax::MissingColon();
    }
}


void SCPI::Parser::ClearList()
{
    words.Clear();
}


String *SCPI::Parser::GetWord()
{
    static String string("ERROR!!! List is empty");

    if (words.Size() == 0U)
    {
        return &string;
    }

    return words.First()->Get();
}


void SCPI::Parser::RemoveWord(String *string)
{
    words.Remove(string);
    delete string;
}


void SCPI::Parser::ParseStar()
{
    for (uint i = 0; i < Buffer::Size(); i++)
    {
        if (Buffer::GetByte(i) == 0x0d)
        {
            String *string = CreateString(0, i - 1);
            words.Append(string);
            Buffer::MarkFirstSymbolAsBad();
            Buffer::RemoveBadSymbols();
        }
    }
}


void SCPI::Parser::ParseColon()
{
}


String *SCPI::Parser::CreateString(uint first, uint last)
{
    String *result = new String(last - first + 1);

    int count = 0;

    for (uint i = first; i <= last; i++)
    {
        (*result)[count++] = static_cast<char>(Buffer::GetByte(i));
    }

    (*result)[count] = '\0';

    return result;
}
