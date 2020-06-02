#pragma once


#define POLARITY_POS "\x84\x85\x86"
#define POLARITY_NEG "\x94\x95\x96"


struct Ideograph
{
    struct _7
    {
        enum
        {
            FillUp   = '\xa3',
            FillDown = '\xa4'
        };
    };

    struct _8
    {
        struct Governor
        {
            struct Shift
            {
                enum
                {
                    _3 = '\x0e',
                    _2 = '\x8c',
                    _0 = '\xaa',
                    _1 = '\xac'
                };
            };
        };

        enum
        {
            Degree      = '\x7b',
            Up          = '\x7c',
            Down        = '\x7e',
            PolarityPos = '\x84',
            PolarityNeg = '\x94',
            BigPlus     = '\x9c',
            BigMinus    = '\x9d',
            Left        = '\xa0',
            Right       = '\xa2',
            Delete      = '\xa6',
            FillUp      = '\xaa',
            FillDown    = '\xac',
            Save        = '\xae'
        };
    };
};
