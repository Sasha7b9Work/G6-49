#pragma once
#include "defines.h"
#include "SettingsTypes.h"
#include "Display/InputWindowStruct.h"
#include "Menu/MenuItems.h"
#include "Display/Colors.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pInput;

#define WAVE_FORM               (set.sig_form[CURRENT_CHANNEL])
#define WAVE_FORM_CH(ch)        (set.sig_form[ch])
#define WAVE_FORM_IS_FREE       (WAVE_FORM != Sine && WAVE_FORM != Impulse)

#define COLOR(x)                (set.disp_Colors[x])
#define CURRENT_PAGE            (set.menu_currentPage)
#define ADDITION_PAGE           (set.menu_page)
#define ADDITION_PAGE_IS_INPUT  (ADDITION_PAGE == (Page *)&pInput)
#define ADDITION_PAGE_IS_NONE   (ADDITION_PAGE == 0)

#define MENU_POS_ACT_ITEM(x)    (set.menu_posActItem[x])
#define MENU_CURRENT_SUBPAGE(x) (set.menu_currentSubPage[x])

#define CURRENT_PARAMETER(form) (set.sig_parameter[form.ToValue()])

#define BIT_FL1(numBit)  ((FLAG_1 >> numBit) & 0x01)
#define BIT_FL2(numBit)  ((FLAG_2 >> numBit) & 0x01)
#define BIT_FL3(numBit)  ((FLAG_3 >> numBit) & 0x01)

#define CONSOLE_ENABLED         (BIT_FL1(BIT_CONSOLE))
#define DEBUG_MODE_ENABLED      (BIT_FL1(BIT_DBG_MODE))
#define BACKGROUND_BLACK        (BIT_FL1(BIT_BACK_BLACK))
#define SHOW_STATISTICS         (BIT_FL1(BIT_STATISTICS))
#define TUNE_FULL               (BIT_FL1(BIT_TUNE_FULL))
#define PARITY                  ((Parity)BIT_FL1(BIT_PARITY))

#define CHANNEL_ENABLED(ch)     ((FLAG_2 >> (ch + BIT_CHAN_A)) & 0x01)
#define SWITCH_CHANNEL_A        (FLAG_2 ^= (1 << (BIT_CHAN_A)))
#define SWITCH_CHANNEL_B        (FLAG_2 ^= (1 << (BIT_CHAN_B)))
#define LANG                    ((Language)BIT_FL2(BIT_LANGUAGE))
#define LANG_RU                 (LANG == RU)
#define CURRENT_CHANNEL         ((Channel)BIT_FL2(BIT_CHANNEL))
#define CURRENT_CHANNEL_IS_A    (CURRENT_CHANNEL == A)
#define CURRENT_CHANNEL_IS_B    (CURRENT_CHANNEL == B)
#define SIZE_BYTE               ((SizeByte)BIT_FL2(BIT_SIZE_BYTE))
#define STOP_BIT                ((StopBit)BIT_FL2(BIT_STOP_BIT))

#define FREQ_RESIST             ((FreqResist)BIT_FL2(BIT_FREQ_RESIST))
#define FREQ_COUPLE             ((FreqCouple)BIT_FL2(BIT_FREQ_COUPLE))
#define FREQ_FILTR              ((FreqFiltr)BIT_FL3(BIT_FREQ_FILTR))
#define FREQ_ENABLED            (BIT_FL1(BIT_FREQ_ENABLED))
#define FREQ_INTERVAL           ((FreqInterval)BIT_FL1(BIT_FREQ_INTERVAL))
#define FREQ_MEASURE            ((FreqMeasure)BIT_FL3(BIT_FREQ_MEASURE))
#define FREQ_MEASURE_IS_FREQ    (FREQ_MEASURE == FreqMeasure_Freq)
#define FREQ_AVE_PERIOD         (set.freq_avePeriod)
#define FREQ_BILLING_TIME       (set.freq_billingTime)
#define FREQ_TIME_STAMPS        (set.freq_timeStamps)
#define FREQ_TEST               ((FreqTest)BIT_FL3(BIT_FREQ_TEST))

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
    col_val            disp_Colors[32];                 ///< Цвета
    WaveForm           sig_form[NumChannels];           ///< Текущая выбранная форма сигнала
    Type_WaveParameter sig_parameter[NumForms];         ///< Текущий выбранный параметр сигнала
    int8               menu_currentPage;                ///< Отображаемая страница меню
    Page*              menu_page;                       ///< Если активна страница не из главного меню, то здесь её адрес
    int8               menu_posActItem[NumPages];       ///< Позиция активного пункта меню для каждой страницы
    int8               menu_currentSubPage[NumPages];   ///< Номер текущей подстраницы для каждой страницы
    BillingTime        freq_billingTime;                ///< Время счёта
    FreqAvePeriod      freq_avePeriod;                  ///< Число усредняемых периодов в режиме измерения периода
    FreqTimeStamps     freq_timeStamps;                 ///< Метки времени

#define FLAG_1      set.flag1
    uint8           flag1;
#define BIT_PARITY        0  ///< Parity   - флаг чётности Parity
#define BIT_FREQ_INTERVAL 1  ///< Interval - интервал запуска измерений
#define BIT_CONSOLE       2  ///<          - показ отладочной консоли
#define BIT_BACK_BLACK    3  ///<          - если 1, то цвет фона - чёрный
#define BIT_TUNE_FULL     4  ///<          - если 1, то засылка параметра происходит непрерывно во время настройки
#define BIT_DBG_MODE      5  ///<          - если 1, то включён отладочный режим - непрерывные засылки в альтеру
#define BIT_FREQ_ENABLED  6  ///<          - если 1, то отображаются показания частотомера
#define BIT_STATISTICS    7  ///<          - если 1, то показывать статистику

#define FLAG_2      set.flag2
    uint8           flag2;
#define BIT_LANGUAGE      0  ///< Language - выбранный язык
#define BIT_CHAN_A        1  ///<          - бит, отвечающий за включённый канал A
#define BIT_CHAN_B        2  ///<          - бит, отвечающий за включённый канал B
#define BIT_CHANNEL       3  ///< Channel  - текущий выбранный канал
#define BIT_SIZE_BYTE     4  ///< SizeByte - размер байта для связи по USB
#define BIT_STOP_BIT      5  ///< StopBit  - количество стоп-бит
#define BIT_FREQ_RESIST   6  ///< FreqResist - сопротивление входа частотомера
#define BIT_FREQ_COUPLE   7  ///< FreqCouple - открытый/закрытый вход частотомера

#define FLAG_3      set.flag3
    uint8           flag3;
#define BIT_FREQ_FILTR    0  ///< FreqFiltr   - ФНЧ частотомера
#define BIT_FREQ_MEASURE  1  ///< FreqMeasure - измерение частоты или измерение периода
#define BIT_FREQ_TEST     2  ///< FreqTest    - включение тестового режима


    static void Save();
    static void Load(bool _default = false);
};

#pragma pack(pop)

extern Settings set;

void TuneGenerator(Channel ch);
