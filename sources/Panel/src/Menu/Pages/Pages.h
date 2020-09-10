#pragma once
#include "common/Common.h"
#include "Menu/MenuItems.h"


class Parameter;


struct PageMain
{
    static Page *self;
};


struct PageSignals
{
    static Page *self;

    // Возвращает true, если открыта первая подстраница (с которой производится настройка каналов)
    static bool OnSubPageTuneChannels();

    static void Init();

    static void OnPress_Channel(bool);

    static void SetCurrentChanenl(Chan::E ch);

    // Эта функция должна вызываться после выбора формы сигнала - по её нажатию происходит загрузка формы в прибор
    static void OnChanged_Form(bool = true);

    static void SCPI_SetForm(TypeForm::E form);
};


class PageFrequencyCounter
{
public:
    // Сформировать содержимое регистра управления частотомером в соответствии с настройками
    static void WriteRegisterRG9();

    static void OnPress_Measure(bool);

    static void OnPress_BillingTime(bool);

    static Page *self;
};


class PageService
{
public:

    static void OnPress_Reset();

    static void OnChange_Volume(bool);

    static Page *self;
};


class PageLoadForm
{
public:
    static Page *self;

    static void LoadForm(Chan::E ch);
};


struct PageTuneParameter
{
    static Page *self;

    // Устанавливает параметр для редактирования
    static void SetParameter(Parameter *);

    // Установить режим ввода значения
    static void SetModeEntering();

    // Сбросить режим ввода значения
    static void ResetModeEntering();

    // Эту функцию будет вызывать Tuner при нажатии кнопки "Отменить"
    static void CallbackOnButtonCancel();

    // Эту функцию будет вызывать Tuner при нажатии кнопки "Применить"
    static void CallbackOnButtonApply();

    static bool IsOpened();

    // Проверка на возможность изменения размаха. Возвращает false, если изменение невозможно
    static bool VerifyForPossiblyChangesAmplitude(const Control &control);
};


struct PageDebug
{
    static Page *self;

    // Включить отображение страницы отладки
    static void Enable();

    static bool SaveScreenToFlashIsEnabled();

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
