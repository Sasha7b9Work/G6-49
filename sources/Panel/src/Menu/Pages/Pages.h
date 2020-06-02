#pragma once
#include "common/Common.h"
#include "Menu/MenuItems.h"


class ParameterDouble;


struct PageSignals
{
    static void Init();

    static void SetCurrentChanenl(Chan::E ch);

    static void OnPress_Channel(bool);

    static void OnPress_Form(bool);

    static void SetForm(TypeForm::E form);

    static Page *self;
};


class PageFrequencyCounter
{
public:
    // Сформировать содержимое регистра управления частотомером в соответствии с настройками
    static void WriteRegisterRG9();

    static Page *self;
};


class PageService
{
public:

    static void OnPress_Reset();

    static Page *self;
};


class PageLoadForm
{
public:
    static Page *pointer;
};


struct PageTuneParameter
{
    static Page *self;

    // Устанавливает параметр для редактирования
    static void SetParameter(ParameterDouble *);

    // Установить режим ввода значения
    static void SetModeEntering();

    // Сбросить режим ввода значения
    static void ResetModeEntering();

    // Эту функцию будет вызывать Tuner при нажатии кнопки "Отменить"
    static void CallbackOnButtonCancel();

    // Эту функцию будет вызывать Tuner при нажатии кнопки "Применить"
    static void CallbackOnButtonApply();

    static bool IsOpened();
};


struct PageDebug
{
    static Page *self;

    // Включить отображение страницы отладки
    static void Enable();

    struct PageRegisters
    {
        static void Draw();

    private:
        // Написать регистры
        static void DrawRegisters(int x, int y);

        static void DrawInputWindow();
    };

    struct SubRange
    {
        static Page *self;
    };

    struct Colors
    {
        static void Init();

        static Page *self;

        struct ChanA
        {
            static Page *self;
        };

        struct ChanB
        {
            static Page *self;
        };

        struct Menu
        {
            static Page *self;
        };
    };

    struct Calibartion
    {
        static Page *self;
    };

    struct _Calibration
    {
        struct PageA
        {
            static Page *self;
        };

        struct PageB
        {
            static Page *self;
        };

        static void OnPress_OffsetAD9952(Chan::E ch, bool enter, KoeffCal::E koeff);

        static void OnPress_DDS(Chan::E ch, bool enter, KoeffCal::E koeff);

        static void OnPress_AmplitudeAD9952(Chan::E ch, bool enter, KoeffCal::E koeff);

        static void WriteKoeffCal(Chan::E ch, KoeffCal::E koeff);
    };
};
