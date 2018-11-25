#pragma once


class Handlers
{
public:
    static bool Processing(Message *message);

private:
    static bool E(Message *);
    static bool Request(Message *);
    static bool FreqMeasure(Message *);
    static bool Log(Message *);
};
