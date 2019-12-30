#include "defines.h"
#include "Generator/Signals.h"
#include "Menu/Pages/PageSignals/PageSignals.h"
#include "SCPI/KeySCPI.h"
#include "SCPI/HeadSCPI.h"
#include "Settings/Settings.h"
#include "Utils/String.h"
#include "Utils/StringUtils.h"
#include "Wave.h"
#include <cstdio>


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
// :FREQUENCY
static const char *FuncFrequency(const char *);
static void HintFrequency(String *);
// :AMPLITUDE
static const char *FuncAmplitude(const char *);
static void HintAmplitude(String *);
// :OFFSET
static const char *FuncOffset(const char *);
static void HintOffset(String *);
// :MODESTART
static const char *FuncModeStart(const char *);
static void HintModeStart(String *);


/// Рекурсивная функция формирования сообщения подсказки
static void ProcessHelp(const StructSCPI strct[], String message); //-V2504


const StructSCPI SCPI::head[] =
{
    SCPI_LEAF("*IDN?",      FuncIDN,       "ID request",                       HintIDN),
    SCPI_LEAF("*RST",       FuncReset,     "Reset settings to default values", HintReset),
    SCPI_LEAF(":HELP",      FuncHelp,      "Output of this help",              HintHelp),

    SCPI_LEAF(":AMPLITUDE", FuncAmplitude, "Set amplitue of wave",             HintAmplitude),
    SCPI_LEAF(":CHANNEL",   FuncChannel,   "Set active channel",               HintChannel),
    SCPI_LEAF(":FORM",      FuncForm,      "Set form wave on output",          HintForm),
    SCPI_LEAF(":FREQUENCY", FuncFrequency, "Set frequency of wave",            HintFrequency),
    SCPI_LEAF(":MODESTART", FuncModeStart, "Set mode start of wave",           HintModeStart),
    SCPI_LEAF(":OFFSET",    FuncOffset,    "Set offset of wave",               HintOffset),
    SCPI_NODE(":KEY",       SCPI::key),
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


static const char *FuncChannel(const char *buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(channelNames[CURRENT_CHANNEL]));

    SCPI_PROCESS_ARRAY(channelNames, PageSignals::SetCurrentChanenl(static_cast<Chan::E>(channelNames[i][1] - 'A')));
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

static const char *FuncForm(const char *buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(formNames[*CURRENT_FORM]));

    SCPI_PROCESS_ARRAY(formNames, PageSignals::SetForm(static_cast<Form::E>(i)));
}


static void HintForm(String *message)
{
    SCPI::ProcessHint(message, formNames);
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static const char *const modeStartNames[] =
{
    " AUTO",
    " SINGLE",
    " COMPARATORA",
    " FORMB",
    ""
};


static void SetModeStart(ParameterChoice *param, int i)
{
    param->SetChoice(i);
    PGenerator::SetParameter(param);
}


static const char *FuncModeStart(const char *buffer)
{
    ParameterBase *param = CURRENT_FORM->FindParameter(ParameterChoice::ModeStart);

    if(param == nullptr)
    {
        return nullptr;
    }

    ParameterChoice *choice = reinterpret_cast<ParameterChoice *>(param);

    SCPI_REQUEST(SCPI::SendAnswer(modeStartNames[choice->GetChoice()]));

    SCPI_PROCESS_ARRAY(modeStartNames, SetModeStart(choice, i));
}


static void HintModeStart(String *message)
{
    SCPI::ProcessHint(message, modeStartNames);
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static const char *FuncFrequency(const char *buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(CURRENT_FORM->GetParameterValue(ParameterValue::Frequency)->GetStringValue()));

    buffer++;

    float frequency = 0.0F;

    char *end_str = nullptr;

    if(SU::String2Float(buffer, &frequency, &end_str))
    {
        ParameterValue *param = CURRENT_FORM->GetParameterValue(ParameterValue::Frequency);

        param->SetValue(frequency);

        PGenerator::SetParameter(param);

        return end_str + 1;
    }

    return nullptr;
}


static void HintFrequency(String *)
{

}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static const char *FuncAmplitude(const char *buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(CURRENT_FORM->GetParameterValue(ParameterValue::Amplitude)->GetStringValue()));

    buffer++;

    float amplitude = 0.0F;

    char *end_str = nullptr;

    if(SU::String2Float(buffer, &amplitude, &end_str))
    {
        if(amplitude >= 0.0F && amplitude <= 10.0F)
        {
            ParameterValue *param = CURRENT_FORM->GetParameterValue(ParameterValue::Amplitude);

            param->SetValue(amplitude);

            PGenerator::SetParameter(param);

            return end_str + 1;
        }
    }

    return nullptr;
}


static void HintAmplitude(String *)
{

}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static const char *FuncOffset(const char *buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(CURRENT_FORM->GetParameterValue(ParameterValue::Offset)->GetStringValue()));

    buffer++;

    float offset = 0.0F;

    char *end_str = nullptr;

    if(SU::String2Float(buffer, &offset, &end_str))
    {
        if(offset >= -5.0F && offset <= 5.0F)
        {
            ParameterValue *param = CURRENT_FORM->GetParameterValue(ParameterValue::Offset);

            param->SetValue(offset);

            PGenerator::SetParameter(param);

            return end_str + 1;
        }
    }

    return nullptr;
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
        else
        {
            LOG_ERROR("Сюда мы не можем попасть");
        }

        strct++;
    }
}
