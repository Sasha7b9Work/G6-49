#pragma once
#include "common/Messages.h"


struct DDrive
{
    static void Init();

    static void Update();

    static void *handle;

    struct Handler
    {
        static void Processing(SimpleMessage *msg);
    };
};
