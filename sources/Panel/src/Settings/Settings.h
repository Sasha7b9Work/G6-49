#pragma once
#include "defines.h"
#include "SettingsTypes.h"
#include "Display/InputWindowStruct.h"
#include "Menu/MenuItems.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pInput;

#define CURRENT_CHANNEL         (set.sig_channel)
#define CURRENT_CHANNEL_IS_A    (CURRENT_CHANNEL == A)
#define CURRENT_CHANNEL_IS_B    (CURRENT_CHANNEL == B)

#define CHANNEL_ENABLED(ch)     (set.sig_enabled[ch])
#define CHANNEL_ENABLED_A       (CHANNEL_ENABLED(A))
#define CHANNEL_ENABLED_B       (CHANNEL_ENABLED(B))

#define WAVE_FORM               (set.sig_form[CURRENT_CHANNEL])
#define WAVE_FORM_CH(ch)        (set.sig_form[ch])
#define WAVE_FORM_IS_FPGA       (WAVE_FORM != Form_Sine && WAVE_FORM != Form_Impulse)

#define INPUT_WINDOW_STRUCT(ch, form, param)    (set.sig_structParameter[ch][form][param])

#define COLOR(x)                (set.disp_Colors[x])
#define LANGUAGE                (set.serv_language)
#define CURRENT_PAGE            (set.menu_currentPage)
#define OPENED_ITEM             (set.menu_openedItem)
#define OPENED_ITEM_IS_NONE     (OPENED_ITEM == 0)
#define ADDITION_PAGE           (set.menu_page)
#define ADDITION_PAGE_IS_INPUT  (ADDITION_PAGE == (Page *)&pInput)
#define ADDITION_PAGE_IS_NONE   (ADDITION_PAGE == 0)

#define CONSOLE_ENABLED         (set.dbg_console)

#define FREQ_COUNTER_ENABLED    (set.freq_enabled)

#define DEBUG_MODE_ENABLED      (set.dbg_debugModeEnabled)

#define LANG                    (set.serv_language)
#define LANG_RU                 (LANG == Russian)

#define MENU_POS_ACT_ITEM(x)    (set.menu_posActItem[x])
#define MENU_CURRENT_SUBPAGE(x) (set.menu_currentSubPage[x])

#define BACKGROUND_BLACK        (set.serv_bacgroundBlack)

#define CURRENT_PARAMETER(form) (set.sig_parameter[form])

#define SIZE_BYTE               (set.usb_sizeByte)
#define STOP_BIT                (set.usb_stopBit)
#define PARITY                  (set.usb_parity)
#define INTERVAL                (set.freq_interval)
#define BILLING_TIME            (set.freq_billingTime)


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma pack(push, 1)

class Settings
{
public:
    uint16              size;                           ///< Размер структуры Settings
    union
    {
        uint8 empty[2 * 4 * 8];                         ///< Резервируем место для калибровочных коэффициентов исходя из того, что 2 канала,
                                                        ///< четыре параметра на каждый канал и в каждом параметре 8 байт
        struct Cal
        {
        }cal;
    };
    col_val             disp_Colors[16];                ///< Цвета
    Channel             sig_channel;                    ///< Текущий выбранный канал
    Type_WaveForm            sig_form[NumChannels];          ///< Текущая выбранная форма сигнала
    WaveParameter       sig_parameter[NumForms];        ///< Текущий выбранный параметр сигнала
    InputWindowStruct   sig_structParameter[NumChannels][NumForms][NumParameters];
    Language            serv_language;                  ///< Выбранный язык
    int8                menu_currentPage;               ///< Отображаемая страница меню
    Control*            menu_openedItem;                ///< Если какой-то пункт меню раскрыт, то здесь его адрес
    Page*               menu_page;                      ///< Если активна страница не из главного меню, то здесь её адрес
    bool                dbg_console;
    bool                sig_enabled[NumChannels];       ///< Состояние выхода - включен или выключен
    bool                serv_bacgroundBlack;            ///< Если true, то цвет фона - чёрный
    bool                sig_tuneFull;                   ///< Если true, то засылка параметра происходит непрерывно во время настройки
    bool                dbg_debugModeEnabled;           ///< Если true, то включён отладочный режим - непрерывные засылки в альтеру
    int8                menu_posActItem[NumPages];      ///< Позиция активного пункта меню для каждой страницы
    int8                menu_currentSubPage[NumPages];  ///< Номер текущей подстраницы для каждой страницы
    bool                freq_enabled;                   ///< Отображение показаний частотомера
    SizeByte            usb_sizeByte;                   ///< Размер байта для связи по USB
    StopBit             usb_stopBit;                    ///< Количество стоп-бит
    Parity              usb_parity;                     ///< Флаг чётности
    Interval            freq_interval;                  ///< Интервал запуска измерений
    BillingTime         freq_billingTime;               ///< Время счёта

    static void Save();
    static void Load(bool _default = false);
};

#pragma pack(pop)

extern Settings set;

void TuneGenerator(Channel ch);
