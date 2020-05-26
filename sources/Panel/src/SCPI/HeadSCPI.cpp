#include "defines.h"
#include "Generator/Signals.h"
#include "Menu/Pages/PageSignals/PageSignals.h"
#include "SCPI/FreqMeterSCPI.h"
#include "SCPI/KeySCPI.h"
#include "SCPI/HeadSCPI.h"
#include "SCPI/ManipulationSCPI.h"
#include "Settings/Settings.h"
#include "Utils/String.h"
#include "Utils/StringUtils.h"
#include "Generator/Wave.h"
#include <cstdio>


// *IDN?
static pCHAR FuncIDN(pCHAR);
static void HintIDN(String *);
// *RST
static pCHAR FuncReset(pCHAR);
static void HintReset(String *);
// :HELP
static pCHAR FuncHelp(pCHAR);
static void HintHelp(String *);

// :AMPLITUDE
static pCHAR FuncAmplitude(pCHAR);
static void HintAmplitude(String *);
// :CHANNEL
static pCHAR FuncChannel(pCHAR);
static void HintChannel(String *);
// :DURATION
static pCHAR FuncDuration(pCHAR);
static void HintDuration(String *);
// :FORM
static pCHAR FuncForm(pCHAR);
static void HintForm(String *);
// :FREQUENCY
static pCHAR FuncFrequency(pCHAR);
static void HintFrequency(String *);
// :MODESTART
static pCHAR FuncModeStart(pCHAR);
static void HintModeStart(String *);
// :NUMBERIMPULSE
static pCHAR FuncNumberImpulse(pCHAR);
static void HintNumberImpulse(String *);
// :OFFSET
static pCHAR FuncOffset(pCHAR);
static void HintOffset(String *);
// :PERIOD
static pCHAR FuncPeriod(pCHAR);
static void HintPeriod(String *);
// :Phase
static pCHAR FuncPhase(pCHAR);
static void HintPhase(String *);
// :PERIODPACKET
static pCHAR FuncPeriodPacket(pCHAR);
static void HintPeriodPacket(String *);
// :POLARITY
static pCHAR FuncPolarity(pCHAR);
static void HintPolarity(String *);


// Рекурсивная функция формирования сообщения подсказки
static void ProcessHelp(const StructSCPI strct[], String message); //-V2504


const StructSCPI SCPI::head[] =
{
    SCPI_LEAF("*IDN?",          FuncIDN,           "ID request",                       HintIDN),
    SCPI_LEAF("*RST",           FuncReset,         "Reset settings to default values", HintReset),
    SCPI_LEAF(":HELP",          FuncHelp,          "Output of this help",              HintHelp),

    SCPI_LEAF(":AMPLITUDE",     FuncAmplitude,     "Set amplitue of wave",             HintAmplitude),
    SCPI_LEAF(":CHANNEL",       FuncChannel,       "Set active channel",               HintChannel),
    SCPI_LEAF(":DURATION",      FuncDuration,      "Set duraction of impulse",         HintDuration),
    SCPI_LEAF(":FORM",          FuncForm,          "Set form wave on output",          HintForm),
    SCPI_LEAF(":FREQUENCY",     FuncFrequency,     "Set frequency of wave",            HintFrequency),
    SCPI_LEAF(":MODESTART",     FuncModeStart,     "Set mode start of wave",           HintModeStart),
    SCPI_LEAF(":NUMBERIMPULSE", FuncNumberImpulse, "Set number of impulses in packet", HintNumberImpulse),
    SCPI_LEAF(":OFFSET",        FuncOffset,        "Set offset of wave",               HintOffset),
    SCPI_LEAF(":PERIODPACKET",  FuncPeriodPacket,  "Set packet following period",      HintPeriodPacket),
    SCPI_LEAF(":PERIOD",        FuncPeriod,        "Set period of wave",               HintPeriod),
    SCPI_LEAF(":PHASE",         FuncPhase,         "Set phase of wave",                HintPhase),
    SCPI_LEAF(":POLARITY",      FuncPolarity,      "Set polarity of wave",             HintPolarity),
    SCPI_NODE(":FREQMETER",     SCPI::freqmeter),
    SCPI_NODE(":KEY",           SCPI::key),
    SCPI_NODE(":MANIPULATION",  SCPI::manipulation),
    SCPI_EMPTY()
};


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static pCHAR FuncIDN(pCHAR buffer)
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
static pCHAR FuncReset(pCHAR buffer)
{
    SCPI_PROLOG(buffer)
        
    SCPI_EPILOG(buffer)
}


static void HintReset(String *message) //-V2009 //-V2558
{
    SCPI::SendAnswer(message->c_str());
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static pCHAR FuncHelp(pCHAR buffer)
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
static pCHAR const channelNames[] =
{
    " A",
    " B",
    ""
};


static pCHAR FuncChannel(pCHAR buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(channelNames[CURRENT_CHANNEL]));

    SCPI_PROCESS_ARRAY(channelNames, PageSignals::SetCurrentChanenl(static_cast<Chan::E>(channelNames[i][1] - 'A')));
}


static void HintChannel(String *message)
{
    SCPI::ProcessHint(message, channelNames);
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static pCHAR const formNames[TypeForm::Count + 1] =
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

static pCHAR FuncForm(pCHAR buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(formNames[*CURRENT_FORM]));

    SCPI_PROCESS_ARRAY(formNames, PageSignals::SetForm(static_cast<TypeForm::E>(i)));
}


static void HintForm(String *message)
{
    SCPI::ProcessHint(message, formNames);
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static pCHAR const modeStartNames[] =
{
    " AUTO",
    " SINGLE",
    " COMPARATORA",
    " FORMB",
    ""
};


static pCHAR FuncModeStart(pCHAR buffer)
{
    return SCPI::ProcessParameterChoice(buffer, ParameterChoice::ModeStart, modeStartNames);
}


static void HintModeStart(String *message)
{
    SCPI::ProcessHint(message, modeStartNames);
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static pCHAR const polarityNames[] =
{
    " +",
    " -",
    ""
};


static pCHAR FuncPolarity(pCHAR buffer)
{
    return SCPI::ProcessParameterChoice(buffer, ParameterChoice::Polarity, polarityNames);
}


static void HintPolarity(String *message)
{
    SCPI::ProcessHint(message, polarityNames);
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static pCHAR FuncNumberImpulse(pCHAR buffer)
{
    return SCPI::ProcessParameterValue(buffer, ParameterValueType::PacketNumber);
}


static void HintNumberImpulse(String *)
{

}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static pCHAR FuncFrequency(pCHAR buffer)
{
    return SCPI::ProcessParameterValue(buffer, ParameterValueType::Frequency);
}


static void HintFrequency(String *)
{

}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static pCHAR FuncAmplitude(pCHAR buffer)
{
    return SCPI::ProcessParameterValue(buffer, ParameterValueType::Amplitude);
}


static void HintAmplitude(String *)
{

}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static pCHAR FuncPeriod(pCHAR buffer)
{
    return SCPI::ProcessParameterValue(buffer, ParameterValueType::Period);
}


static void HintPeriod(String *)
{

}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static pCHAR FuncPhase(pCHAR buffer)
{
    return SCPI::ProcessParameterValue(buffer, ParameterValueType::Phase);
}


static void HintPhase(String *)
{

}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static pCHAR FuncDuration(pCHAR buffer)
{
    return SCPI::ProcessParameterValue(buffer, ParameterValueType::Duration);
}


static void HintDuration(String *)
{

}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static pCHAR FuncPeriodPacket(pCHAR buffer)
{
    return SCPI::ProcessParameterValue(buffer, ParameterValueType::PacketPeriod);
}


static void HintPeriodPacket(String *)
{

}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static pCHAR FuncOffset(pCHAR buffer)
{
    return SCPI::ProcessParameterValue(buffer, ParameterValueType::Offset);
}


static void HintOffset(String *)
{

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

        strct++;
    }
}
