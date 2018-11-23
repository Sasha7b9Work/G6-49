#pragma once
#include "Menu/MenuItems.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class PageSignals
{
public:
    static void Init();

    static void OnPress_Channel(bool);

    static void OnPress_Form(bool);

    static Page *pointer;

    //---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    class PageLoad
    {
    public:
        static Page *pointer;
    };

    //---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    class PageCalibration
    {
    public:

        //-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        class PageA
        {
        public:
            static Page *pointer;
        };

        //-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        class PageB
        {
        public:
            static Page *pointer;
        };

        static void OnPress_OffsetAD9952(Chan ch, bool enter, KoeffCal::E koeff);

        static void OnPress_DDS(Chan ch, bool enter, KoeffCal::E koeff);

        static void OnPress_AmplitudeAD9952(Chan ch, bool enter, KoeffCal::E koeff);

        static void WriteKoeffCal(Chan ch, KoeffCal::E koeff);
    };
};
