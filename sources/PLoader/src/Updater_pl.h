// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class SimpleMessage;


class Updater
{
public:

    static const int MAIN_PROGRAM_START_ADDRESS = 0x8020000;

    static void Update();

    static bool Handler(SimpleMessage *);
};
