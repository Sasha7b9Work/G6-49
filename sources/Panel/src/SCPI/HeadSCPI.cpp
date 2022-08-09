// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Signals/Signals.h"
#include "Signals/Wave.h"
#include "Menu/Pages/Pages.h"
#include "SCPI/FreqMeterSCPI.h"
#include "SCPI/KeySCPI.h"
#include "SCPI/HeadSCPI.h"
#include "SCPI/ManipulationSCPI.h"
#include "Settings/Settings.h"
#include "Utils/StringUtils.h"
#include "Settings/CalibrationSettings.h"
#include <cstdio>


// *IDN?
static pchar FuncIDN(pchar);
static void HintIDN(String *);

// *RST
static pchar FuncReset(pchar);
static void HintReset(String *);

// :HELP
static pchar FuncHelp(pchar);
static void HintHelp(String *);

// :AMPLITUDE
static pchar FuncAmplitude(pchar);
static void HintAmplitude(String *);

// :CHANNEL
static pchar FuncChannel(pchar);
static void HintChannel(String *);

// :DURATION
static pchar FuncDuration(pchar);
static void HintDuration(String *);

// :FORM
static pchar FuncForm(pchar);
static void HintForm(String *);

// :FREQUENCY
static pchar FuncFrequency(pchar);
static void HintFrequency(String *);

// :LANGUAGE
static pchar FuncLanguage(pchar);
static void HintLanguage(String *);

// :MODESTART
static pchar FuncModeStart(pchar);
static void HintModeStart(String *);

// :NUMBERIMPULSE
static pchar FuncNumberImpulse(pchar);
static void HintNumberImpulse(String *);

// :OFFSET
static pchar FuncOffset(pchar);
static void HintOffset(String *);

// :PERIOD
static pchar FuncPeriod(pchar);
static void HintPeriod(String *);

// :Phase
static pchar FuncPhase(pchar);
static void HintPhase(String *);

// :PERIODPACKET
static pchar FuncPeriodPacket(pchar);
static void HintPeriodPacket(String *);

// :POLARITY
static pchar FuncPolarity(pchar);
static void HintPolarity(String *);

// :OUTPUT
static pchar FuncOutput(pchar);
static void HintOutput(String *);

// :SOUND
static pchar FuncSound(pchar);
static void HintSound(String *);


// ����������� ������� ������������ ��������� ���������
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
    SCPI_LEAF(":LANGUAGE",      FuncLanguage,      "Change language",                  HintLanguage),
    SCPI_LEAF(":MODESTART",     FuncModeStart,     "Set mode start of wave",           HintModeStart),
    SCPI_LEAF(":NUMBERIMPULSE", FuncNumberImpulse, "Set number of impulses in packet", HintNumberImpulse),
    SCPI_LEAF(":OFFSET",        FuncOffset,        "Set offset of wave",               HintOffset),
    SCPI_LEAF(":OUTPUT",        FuncOutput,        "Enable/disable output",            HintOutput),
    SCPI_LEAF(":PERIODPACKET",  FuncPeriodPacket,  "Set packet following period",      HintPeriodPacket),
    SCPI_LEAF(":PERIOD",        FuncPeriod,        "Set period of wave",               HintPeriod),
    SCPI_LEAF(":PHASE",         FuncPhase,         "Set phase of wave",                HintPhase),
    SCPI_LEAF(":POLARITY",      FuncPolarity,      "Set polarity of wave",             HintPolarity),
    SCPI_LEAF(":REG",           SCPI::FuncReg,     "Control regulator",                SCPI::HintReg),
    SCPI_LEAF(":SOUND",         FuncSound,         "Set volume sound",                 HintSound),
    SCPI_NODE(":FREQMETER",     SCPI::freqmeter),
    SCPI_NODE(":KEY",           SCPI::key),
    //SCPI_NODE(":MANIPULATION",  SCPI::manipulation),
    SCPI_EMPTY()
};


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static pchar FuncIDN(pchar buffer)
{
    SCPI_PROLOG(buffer)

    SCPI::SendAnswer(String("MNIPI, %s, %s, %s", IDENTIFICATOR, VERSION, CRC32).c_str());

    SCPI_EPILOG(buffer)
}


static void HintIDN(String *message) //-V2009 //-V2558
{
    SCPI::SendAnswer(message->c_str());
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static pchar FuncReset(pchar buffer)
{
    SCPI_PROLOG(buffer)

    PGenerator::Reset();
        
    SCPI_EPILOG(buffer)
}


static void HintReset(String *message) //-V2009 //-V2558
{
    SCPI::SendAnswer(message->c_str());
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static pchar FuncHelp(pchar buffer)
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
static pchar const channelNames[] =
{
    " A",
    " B",
    ""
};


static pchar FuncChannel(pchar buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(channelNames[CURRENT_CHANNEL]));

    SCPI_PROCESS_ARRAY(channelNames, PageSignals::SetCurrentChanenl(Chan((Chan::E)(channelNames[i][1] - 'A'))));
}


static void HintChannel(String *message)
{
    SCPI::ProcessHint(message, channelNames);
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static pchar const formNames[TypeForm::Count + 1] =
{
    " SINE",
    " SAW+",
    " SAW-",
    " TRIANGLE",
    " MEANDER",
    " IMPULSE",
    " PACKET",
    " FREE",
    ""
};

static pchar FuncForm(pchar buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(formNames[*CURRENT_FORM]));

    SCPI_PROCESS_ARRAY(formNames, PageSignals::SCPI_SetForm(static_cast<TypeForm::E>(i)));
}


static void HintForm(String *message)
{
    SCPI::ProcessHint(message, formNames);
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static pchar const outputs[] =
{
    " OFF",
    " ON",
    ""
};


static void SetOutput(int i)
{
    PGenerator::EnableChannel(CURRENT_CHANNEL, i != 0);
}


static pchar FuncOutput(pchar buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(outputs[CURRENT_CHANNEL.Enabled()]));

    SCPI_PROCESS_ARRAY(outputs, SetOutput(i));
}


static void HintOutput(String *message)
{
    SCPI::ProcessHint(message, outputs);
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static pchar const modeStartNames[] =
{
    " AUTO",
    " SINGLE",
    " COMPARATORA",
    " FORMB",
    ""
};


static pchar FuncModeStart(pchar buffer)
{
    return SCPI::ProcessParameterChoice(buffer, ParameterChoiceType::ModeStart, modeStartNames);
}


static void HintModeStart(String *message)
{
    SCPI::ProcessHint(message, modeStartNames);
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static pchar const polarityNames[] =
{
    " +",
    " -",
    ""
};


static pchar FuncPolarity(pchar buffer)
{
    return SCPI::ProcessParameterChoice(buffer, ParameterChoiceType::Polarity, polarityNames);
}


static void HintPolarity(String *message)
{
    SCPI::ProcessHint(message, polarityNames);
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static pchar FuncNumberImpulse(pchar buffer)
{
    return SCPI::ProcessParameterInteger(buffer, TypeIParam::PacketNumber);
}


static void HintNumberImpulse(String *)
{

}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static pchar FuncFrequency(pchar buffer)
{
    return SCPI::ProcessParameterDouble(buffer, TypeDParam::Frequency);
}


static void HintFrequency(String *)
{
    SCPI::SendAnswer(":FREQUENCY [300e-6...10e6]");
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static pchar const langNames[] =
{
    " RU",
    " EN",
    ""
};


static pchar FuncLanguage(pchar buffer)
{
    pchar end = SCPI::BeginWith(buffer, "?");

    if (end)
    {
        SCPI_PROLOG(end)

        SCPI::SendAnswer(langNames[LANGUAGE]);

        SCPI_EPILOG(end)
    }

    for (int i = 0; i < 2; i++)
    {
        end = SCPI::BeginWith(buffer, langNames[i]);
        if (end)
        {
            SCPI_PROLOG(end)

            LANGUAGE = static_cast<uint8>(i);

            SCPI_EPILOG(end)
        }
    }

    return nullptr;
}


static void HintLanguage(String *)
{
    SCPI::SendAnswer(":LANGUAGE {RU|EN}");
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static pchar const soundNames[] =
{
    " OFF",
    " LOW",
    " MIDDLE",
    " HIGH",
    ""
};

static pchar FuncSound(pchar buffer)
{
    pchar end = SCPI::BeginWith(buffer, "?");

    if (end)
    {
        SCPI_PROLOG(end)

        SCPI::SendAnswer(soundNames[setCal.soundVolume]);

        SCPI_EPILOG(end)
    }

    for (int i = 0; i < 4; i++)
    {
        end = SCPI::BeginWith(buffer, soundNames[i]);
        if (end)
        {
            SCPI_PROLOG(end)

            setCal.soundVolume = static_cast<uint8>(i);
            PageService::OnChange_Volume(true);

            SCPI_EPILOG(end)
        }
    }

    return nullptr;
}


static void HintSound(String *)
{
    SCPI::SendAnswer(":SOUND {OFF|LOW|MIDDLE|HIGH}");
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static pchar FuncAmplitude(pchar buffer)
{
    return SCPI::ProcessParameterDouble(buffer, TypeDParam::Amplitude);
}


static void HintAmplitude(String *)
{
    SCPI::SendAnswer(":AMPLITUDE [0.000...10.000]");
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static pchar FuncPeriod(pchar buffer)
{
    return SCPI::ProcessParameterDouble(buffer, TypeDParam::Period);
}


static void HintPeriod(String *)
{

}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static pchar FuncPhase(pchar buffer)
{
    return SCPI::ProcessParameterDouble(buffer, TypeDParam::Phase);
}


static void HintPhase(String *)
{

}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static pchar FuncDuration(pchar buffer)
{
    return SCPI::ProcessParameterDouble(buffer, TypeDParam::Duration);
}


static void HintDuration(String *)
{

}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static pchar FuncPeriodPacket(pchar buffer)
{
    return SCPI::ProcessParameterDouble(buffer, TypeDParam::PacketPeriod);
}


static void HintPeriodPacket(String *)
{

}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static pchar FuncOffset(pchar buffer)
{
    return SCPI::ProcessParameterDouble(buffer, TypeDParam::Offset);
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
