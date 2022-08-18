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

    // ��� ������� ������ ���������� ����� ������ ����� ������� - �� � ������� ���������� �������� ����� � ������
    void OnChanged_Form(bool = true);

    void SCPI_SetForm(TypeForm::E form);
};


namespace PageFrequencyCounter
{
    // ������������ ���������� �������� ���������� ������������ � ������������ � �����������
    void WriteRegisterRG9();

    void OnPress_Measure(bool);

    void OnPress_BillingTime(bool);

    void OnPress_Resist(bool);

    void OnPress_TimeStamps(bool);

    void OnPress_AvePeriod(bool);

    void OnPress_Couple(bool);

    void OnPress_Filtr(bool);

    void OnPress_Test(bool);

    void OnChange_FreqLevel();

    extern Page *self;
};


namespace PageService
{
    void OnPress_Reset();

    void OnChange_Volume(bool);

    extern Page *self;
};


namespace PageLoadForm
{
    extern Page *self;

    void LoadForm(const Chan &ch);
};


namespace PageTuneParameter
{
    extern Page *self;

    // ������������� �������� ��� ��������������
    void SetParameter(Param *);

    // ���������� ����� ����� ��������
    void SetModeEntering();

    // �������� ����� ����� ��������
    void ResetModeEntering();

    // ��� ������� ����� �������� Tuner ��� ������� ������ "��������"
    void CallbackOnButtonCancel();

    // ��� ������� ����� �������� Tuner ��� ������� ������ "���������"
    void CallbackOnButtonApply();

    bool IsOpened();

    // ������� �� ������ "���������"
    void OnPress_Apply();
};


namespace PageDebug
{
    extern Page *self;

    // �������� ����������� �������� �������
    void Enable();

    bool SaveScreenToFlashIsEnabled();

    namespace PageRegisters
    {
        void Draw();
    };

    namespace SubRange
    {
        extern Page *self;
    };

    namespace Colors
    {
        void Init();

        extern Page *self;

        namespace ChanA
        {
            extern Page *self;
        };

        namespace ChanB
        {
            extern Page *self;
        };

        namespace Menu
        {
            extern Page *self;
        };
    };

    namespace Calibartion
    {
        extern Page *self;
    };

    namespace _Calibration
    {
        namespace PageA
        {
            extern Page *self;
        };

        namespace PageB
        {
            extern Page *self;
        };

        void OnPress_OffsetAD9952(const Chan &, bool enter, KoeffCal::E koeff);

        void OnPress_DDS(const Chan &, bool enter, KoeffCal::E koeff);

        void OnPress_AmplitudeAD9952(const Chan &, bool enter, KoeffCal::E koeff);

        void WriteKoeffCal(const Chan &, KoeffCal::E koeff);
    };
};
