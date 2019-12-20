#pragma once
#include "Message.h"


namespace Message
{
    class LoadFormDDS : public SimpleMessage
    {
    public:
        LoadFormDDS(uint8 ch, uint16 data[PGenerator::DDS_NUM_POINTS]);
    };
}
