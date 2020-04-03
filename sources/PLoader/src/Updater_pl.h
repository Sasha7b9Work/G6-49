#pragma once


class SimpleMessage;


class Updater
{
public:
    static void Update();

    static void Handler(SimpleMessage *);
};
