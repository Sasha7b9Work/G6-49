// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Common.h"
#include "Menu/MenuItems.h"


struct Param;


namespace PageMain
{
    extern Page *self;
};


namespace PageSignals
{
    extern Page *self;

    void Init();

    void OnPress_Channel(bool);

    void SetCurrentChanenl(const Chan &);

    // Эта функция должна вызываться после выбора формы сигнала - по её нажатию происходит загрузка формы в прибор
    void OnChanged_Form(bool = true);

    void SCPI_SetForm(TypeForm::E form);
};


class PageFrequencyCounter
{
public:
    // Сформировать содержимое регистра управления частотомером в соответствии с настройками
    static void WriteRegisterRG9();

    static void OnPress_Measure(bool);

    static void OnPress_BillingTime(bool);

    static void OnPress_Resist(bool);

    static void OnPress_TimeStamps(bool);

    static void OnPress_AvePeriod(bool);

    static void OnPress_Couple(bool);

    static void OnPress_Filtr(bool);

    static void OnPress_Test(bool);

    static void OnChange_FreqLevel();

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

    static void LoadForm(const Chan &ch);
};


namespace PageTuneParameter
{
    extern Page *self;

    // Устанавливает параметр для редактирования
    void SetParameter(Param *);

    // Установить режим ввода значения
    void SetModeEntering();

    // Сбросить режим ввода значения
    void ResetModeEntering();

    // Эту функцию будет вызывать Tuner при нажатии кнопки "Отменить"
    void CallbackOnButtonCancel();

    // Эту функцию будет вызывать Tuner при нажатии кнопки "Применить"
    void CallbackOnButtonApply();

    bool IsOpened();

    // Нажатие на кнопку "Применить"
    void OnPress_Apply();
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

        static void OnPress_OffsetAD9952(const Chan &, bool enter, KoeffCal::E koeff);

        static void OnPress_DDS(const Chan &, bool enter, KoeffCal::E koeff);

        static void OnPress_AmplitudeAD9952(const Chan &, bool enter, KoeffCal::E koeff);

        static void WriteKoeffCal(const Chan &, KoeffCal::E koeff);
    };
};
