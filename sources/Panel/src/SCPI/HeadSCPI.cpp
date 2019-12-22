#include "defines.h"
#include "Menu/Pages/Include/PageSignals.h"
#include "SCPI/KeySCPI.h"
#include "SCPI/HeadSCPI.h"
#include "Settings/Settings.h"
#include "Utils/String.h"
#include "Wave.h"


// *IDN?
static const char *FuncIDN(const char *);
static void HintIDN(String *);
// *RST
static const char *FuncReset(const char *);
static void HintReset(String *);
// :HELP
static const char *FuncHelp(const char *);
static void HintHelp(String *);
// :CHANNEL
static const char *FuncChannel(const char *);
static void HintChannel(String *);
// :FORM
static const char *FuncForm(const char *);
static void HintForm(String *);

/// Рекурсивная функция формирования сообщения подсказки
static void ProcessHelp(const StructSCPI strct[], String message); //-V2504


const StructSCPI SCPI::head[] =
{
    SCPI_LEAF("*IDN?",     FuncIDN,     "ID request",                       HintIDN),
    SCPI_LEAF("*RST",      FuncReset,   "Reset settings to default values", HintReset),
    SCPI_LEAF(":HELP",     FuncHelp,    "Output of this help",              HintHelp),

    SCPI_LEAF(":CHANNEL",  FuncChannel, "Set active channel",               HintChannel),
    SCPI_LEAF(":FORM",     FuncForm,    "Set form wave on output",          HintForm),
    SCPI_NODE(":KEY",      SCPI::key),
    SCPI_EMPTY()
};


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
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


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static const char *FuncReset(const char *buffer)
{
    SCPI_PROLOG(buffer)
        
    SCPI_EPILOG(buffer)
}


static void HintReset(String *message) //-V2009 //-V2558
{
    SCPI::SendAnswer(message->c_str());
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
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


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static const char *const channelNames[] =
{
    " A",
    " B",
    ""
};


static void FuncSetChannel(int i)
{
    PageSignals::SetCurrentChanenl(static_cast<Chan::E>(channelNames[i][1] - 'A'));
}


static const char *FuncChannel(const char *buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(channelNames[CURRENT_CHANNEL]));

    SCPI_PROCESS_ARRAY(channelNames, FuncSetChannel(i));
}


static void HintChannel(String *message)
{
    SCPI::ProcessHint(message, channelNames);
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static const char *const formNames[Form::Count + 1] =
{
    " SINE",
    " RAMP+",
    " RAMP-",
    " TRIANGLE",
    " MEANDER",
    " IMPULSE",
    " PACKET",
    " FREE",
    ""
};

static const char *FuncForm(const char *)
{
    return nullptr;
}


static void HintForm(String *message)
{
    SCPI::ProcessHint(message, formNames);
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
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
