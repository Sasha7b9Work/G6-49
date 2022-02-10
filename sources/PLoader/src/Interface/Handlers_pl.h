#pragma once


struct PHandlers
{
    static bool Processing(SimpleMessage *message);

    static bool OnStartMainApplication(SimpleMessage *message = nullptr);
};
