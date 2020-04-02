#pragma once
#pragma once
#include "common/Messages.h"


namespace Message
{
    class RequestUpdate : public SimpleMessage
    {
    public:
        RequestUpdate();
    };


    class StartMainApplication : public SimpleMessage
    {
    public:
        StartMainApplication();
    };
}
