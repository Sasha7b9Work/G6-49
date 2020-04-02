#pragma once


struct PHandlers
{
    static bool Processing(SimpleMessage *message);

private:

    static bool Request(SimpleMessage *);
    static bool FreqMeasure(SimpleMessage *);
    static bool Log(SimpleMessage *);
};
