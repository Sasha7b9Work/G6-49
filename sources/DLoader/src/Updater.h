#pragma once


class SimpleMessage;


class Updater
{
public:
    static bool Process();

    static void Handler(SimpleMessage *);
};
