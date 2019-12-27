#pragma once
#include "Menu/MenuItems.h"



class PageSignals
{
public:
    static void Init();

    static void SetCurrentChanenl(Chan::E ch);

    static void OnPress_Channel(bool);

    static void OnPress_Form(bool);

    static void SetForm(Form::E form);

    static Page *pointer;

    
    class PageCalibration
    {
    public:

        class PageA
        {
        public:
            static Page *pointer;
        };

        class PageB
        {
        public:
            static Page *pointer;
        };

        static void OnPress_OffsetAD9952(Chan::E ch, bool enter, KoeffCal::E koeff);

        static void OnPress_DDS(Chan::E ch, bool enter, KoeffCal::E koeff);

        static void OnPress_AmplitudeAD9952(Chan::E ch, bool enter, KoeffCal::E koeff);

        static void WriteKoeffCal(Chan::E ch, KoeffCal::E koeff);
    };
};
