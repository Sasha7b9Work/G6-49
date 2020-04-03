#pragma once


class SimpleMessage;


class Updater
{
public:
    static void Update();

    static bool Handler(SimpleMessage *);
};
