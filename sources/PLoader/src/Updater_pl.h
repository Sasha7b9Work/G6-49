#pragma once


class SimpleMessage;


class Updater
{
public:

    static const uint MAIN_PROGRAM_START_ADDRESS = 0x8020000;

    static void Update();

    static bool Handler(SimpleMessage *);
};
