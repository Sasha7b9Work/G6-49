#pragma once


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Interface
{
public:

    void Init();

    void ProcessingCommand();

private:

    void CommandEmpty();

    void ProcessCommand();

    void CommandEnable();

    void CommandFormWave();

    void CommandParameter();

    void CommandReset();

    void CommandModeDebug();
};


extern Interface interface;
