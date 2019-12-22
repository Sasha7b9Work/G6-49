#include "defines.h"
#include "SCPI/KeySCPI.h"
#include "SCPI/HeadSCPI.h"
#include "Utils/String.h"


// *IDN?
static const char *FuncIDN(const char *);
static void HintIDN(String *);
// *RST
static const char *FuncReset(const char *);
static void HintReset(String *);
// :HELP
static const char *FuncHelp(const char *);
static void HintHelp(String *);
/// Рекурсивная функция формирования сообщения подсказки
static void ProcessHelp(const StructSCPI strct[], String message); //-V2504


const StructSCPI SCPI::head[] =
{
    SCPI_LEAF("*IDN?",     FuncIDN,    "ID request",                       HintIDN),
    SCPI_LEAF("*RST",      FuncReset,  "Reset settings to default values", HintReset),
    SCPI_LEAF(":HELP",     FuncHelp,   "Output of this help",              HintHelp),
    SCPI_NODE(":KEY",      SCPI::key),
    SCPI_EMPTY()
};


static const char *FuncIDN(const char *buffer)
{
    SCPI_PROLOG(buffer)

    SCPI::SendAnswer("MNIPI, S8-57, v.1.2");

    SCPI_EPILOG(buffer)
}


static void HintIDN(String *message) //-V2009 //-V2558
{
    SCPI::SendAnswer(message->c_str());
}


static const char *FuncReset(const char *buffer)
{
    SCPI_PROLOG(buffer)
        
    SCPI_EPILOG(buffer)
}


static void HintReset(String *message) //-V2009 //-V2558
{
    SCPI::SendAnswer(message->c_str());
}


static const char *FuncHelp(const char *buffer)
{
    SCPI_PROLOG(buffer);
    
    String message;

    ProcessHelp(SCPI::head, message);

    SCPI_EPILOG(buffer);
}


static void HintHelp(String *message) //-V2009 //-V2558
{
    SCPI::SendAnswer(message->c_str());
}


static void ProcessHelp(const StructSCPI strct[], String msg) //-V2504
{
    while(!strct->IsEmpty())
    {
        if(strct->IsNode())
        {
            String message(msg);
            message.Append(strct->key);
            ProcessHelp(strct->strct, message);
        }
        else if(strct->IsLeaf())
        {
            String message(msg); //-V820
            message.Append(strct->key);
            SCPI::SendAnswer(strct->hint);
            strct->funcHint(&message);
            SCPI::SendAnswer("");
        }
        else
        {
            LOG_ERROR("Сюда мы не можем попасть");
        }

        strct++;
    }
}
