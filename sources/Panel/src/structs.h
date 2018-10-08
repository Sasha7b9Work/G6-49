#pragma once
#include "defines.h"

#ifndef WIN32
#pragma anon_unions
#endif

union BitSet16
{
    uint16 halfWord;
    uint8  byte[2];
    struct
    {
        uint8 byte0;
        uint8 byte1;
    };
};

#define INIT_BIT_SET_32(name, value)    \
    BitSet32 name;                      \
    name.word = value;

#define INIT_BIT_SET_64(name, value)    \
    BitSet64 name;                      \
    name.dword = value;

union BitSet32
{
    uint    word;
    float   floatValue;
    uint16  halfWord[2];
    struct
    {
        uint16 halfWord0;
        uint16 halfWord1;
    };
    uint8   byte[4];
    struct
    {
        uint8 byte0;
        uint8 byte1;
        uint8 byte2;
        uint8 byte3;
    };
    BitSet32(uint8 buffer[4] = 0)
    {
        if (buffer)
        {
            for (int i = 0; i < 4; i++)
            {
                byte[i] = buffer[i];
            }
        }
    }
};


union BitSet64
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
};
