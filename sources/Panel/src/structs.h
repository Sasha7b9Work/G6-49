#pragma once

#ifndef WIN32

#pragma anon_unions

#else

#pragma warning(push)
#pragma warning(disable:4201)   // nonstandard extension used: nameless struct/union

#endif

union BitSet16 //-V2514
{
    uint16 halfWord;
    uint8  byte[2];
    struct
    {
        uint8 byte0;
        uint8 byte1;
    };
    explicit BitSet16(uint16 value) : halfWord(value) {};
    explicit BitSet16(uint8 *vals) : byte0(*vals), byte1(*(vals + 1)) {};

    void WriteToBuffer(uint8 buffer[2]) const
    {
        buffer[0] = byte0;
        buffer[1] = byte1;
    }
};


union BitSet32 //-V2514
{
    uint    word;
    float   floatValue;
    uint16  halfWord[2];
    struct
    {
        uint16 halfWord0;
        uint16 halfWord1;
    };
    uint8   byte[sizeof(uint)];
    struct
    {
        uint8 byte0;
        uint8 byte1;
        uint8 byte2;
        uint8 byte3;
    };
    explicit BitSet32(uint value) : word(value)  { }
    explicit BitSet32(float value) : floatValue(value) { }

    BitSet32(const uint8 *buffer = 0) : word(0)
    {
        if (buffer)
        {
            for (int i = 0; i < sizeof(uint); i++)
            {
                byte[i] = buffer[i];
            }
        }
    }
    void WriteToBuffer(uint8 buffer[4])
    {
        for(int i = 0; i < sizeof(uint); i++)
        {
            buffer[i] = byte[i];
        }
    }
};


union BitSet64 //-V2514
{
    uint64 dword;
    uint   word[2];
    uint16 halfWord[4];
    uint8  byte[8];
    struct
    {
        int sword0;
        int sword1;
    };
    struct
    {
        uint word0;
        uint word1;
    };
    struct
    {
        uint16 halfWord0;
        uint16 halfWord1;
        uint16 halfWord2;
        uint16 halfWord3;
    };
    struct
    {
        uint8 byte0;
        uint8 byte1;
        uint8 byte2;
        uint8 byte3;
        uint8 byte4;
        uint8 byte5;
        uint8 byte6;
        uint8 byte7;
    };

    explicit BitSet64(uint64 v) : dword(v) {}

    explicit BitSet64(const uint8 *buffer = 0) : dword(0)
    {
        if (buffer)
        {
            for (int i = 0; i < 8; i++)
            {
                byte[i] = buffer[i];
            }
        }
    }
};

#ifdef WIN32
#pragma warning(pop)
#endif
