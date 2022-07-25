// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
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
