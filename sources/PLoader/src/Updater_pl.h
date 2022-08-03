// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct SimpleMessage;


namespace Updater
{
    const int MAIN_PROGRAM_START_ADDRESS = 0x8020000;

    void Update();

    bool Handler(SimpleMessage *);
};
