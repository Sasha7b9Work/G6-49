// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Messages.h"


namespace DDrive
{
    void Init();

    void Update();

    extern void *handle;

    namespace Handler
    {
        void Processing(SimpleMessage *msg);
    };
};
