#pragma once

#define ERROR_HANDLER() STM429::ErrorHandler()


class STM429
{
public:
    static void ErrorHandler();

    static void Init();

protected:
};
