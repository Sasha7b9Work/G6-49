#include "stdafx.h"
#ifndef WIN32
#include "Display/InputWindow.h"
#include "Settings/Settings.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/AddPageInput.h"
#include "Menu/Pages/Include/PageSignals.h"
#include "Generator/Signals.h"
#include "Generator/GeneratorPanel.h"
#include "Menu/Menu.h"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pageSignals;
Page *PageSignals::pointer = (Page *)&pageSignals;
extern ChoiceParameterBase cParameters;
/// Номер текущей формы сигнал
static int numForm = 0;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_CHOICE_2( cChannel,                                                                                                                                  //--- НАСТРОЙКИ СИГНАЛОВ - Канал ---
    "КАНАЛ",
    "Выбор канала для настройки."
    ,
    "A", "Управление параметрами сигнала на выходе A.",
    "B", "Управление параметрами сигнала на выходе B.",
    FLAG_1, BIT_CHANNEL, pageSignals, Item::FuncActive, PageSignals::OnPress_Channel, FuncDraw
)


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_8( cFormA,                                                                                                                                    //--- НАСТРОЙКИ СИГНАЛОВ - Форма ---
    "ФОРМА",
    "Выбор формы сигнала."
    ,
    FORM_RU(Form::Sine),         "Синус",
    FORM_RU(Form::RampPlus),     "Нарастающая пила",
    FORM_RU(Form::RampMinus),    "Убывающая пила",
    FORM_RU(Form::Triangle),     "Треугольник",
    FORM_RU(Form::Meander),      "Меандр",
    FORM_RU(Form::Impulse),      "Треугольник",
    FORM_RU(Form::PacketImpuls), "Пакеты",
    FORM_RU(Form::DDS),          "Произвольный",
    numForm, pageSignals, Item::FuncActive, PageSignals::OnPress_Form, FuncDraw
)

DEF_CHOICE_7( cFormB,                                                                                                                                    //--- НАСТРОЙКИ СИГНАЛОВ - Форма ---
    "ФОРМА",
    "Выбор формы сигнала."
    ,
    FORM_RU(Form::Sine),      "Синус",
    FORM_RU(Form::RampPlus),  "Нарастающая пила",
    FORM_RU(Form::RampMinus), "Убывающая пила",
    FORM_RU(Form::Triangle),  "Треугольник",
    FORM_RU(Form::Meander),   "Меандр",
    FORM_RU(Form::Impulse),   "Треугольник",
    FORM_RU(Form::DDS),       "Произвольный",
    numForm, pageSignals, Item::FuncActive, PageSignals::OnPress_Form, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_ChnageParameter()
{
    FORM_CURRENT->ChangeParameter();
}

DEF_BUTTON( bChangeParameter,                                                                                                        //--- НАСТРОЙКИ СИГНАЛОВ - Ввести значение параметра ---
    "Изменить",
    "Открывает окно ввода параметра.",
    pageSignals, Item::FuncActive, OnPress_ChnageParameter, FuncDraw
)

DEF_CHOICE_PARAMETER( cParameters,                                                                                                                    //--- НАСТРОЙКИ СИГНАЛОВ - Параметр ---
    "ПАРАМЕТР",
    "Выбор параметра для настройки.",
    pageSignals, Item::FuncActive, OnPress_ChnageParameter, WAVE(Chan::A).GetForm(0)
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cTypeTune,                                                                                                                               //--- НАСТРОЙКИ СИГНАЛОВ - Засылка ---
    "ЗАСЫЛКА",
    "Определяет периодичность засылки настроек сигналов в устройство управления.",
    "ОДНОКРАТНО", "Засылка настроек сигнала происходит однократно, после нажатия кнопки подтверждения.",
    "НЕПРЕРЫВНО", "Засылка настроек сигнала происходит синхронно с изменением информации на экране.",
    FLAG_1, BIT_TUNE_FULL, pageSignals, Item::FuncActive, FuncChangedChoice, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool OnKey(Control key)
{
    if(key.Is(Control::Esc) && key.action.IsRelease())
    {
        return FORM_CURRENT->CloseOpenedParameter();
    }

    return false;
}

DEF_PAGE_8( pageSignals, //-V641 //-V1027
    "НАСТРОЙКИ СИГНАЛОВ",
    "",
    &cChannel,                                      ///< НАСТРОЙКИ СИГНАЛОВ - Канал
    &cFormA,                                        ///< НАСТРОЙКИ СИГНАЛОВ - Форма
    &cParameters,                                   ///< НАСТРОЙКИ СИГНАЛОВ - Параметр
    &bChangeParameter,                              ///< НАСТРОЙКИ СИГНАЛОВ - Ввести значение параметра
    PageSignals::PageLoad::pointer,                 ///< НАСТРОЙКИ СИГНАЛОВ - ЗАГРУЗКА
    &cTypeTune,                                     ///< НАСТРОЙКИ СИГНАЛОВ - Засылки
    PageSignals::PageCalibration::PageA::pointer,   ///< НАСТРОЙКИ СИГНАЛОВ - КАЛИБРОВКА A
    PageSignals::PageCalibration::PageB::pointer,   ///< НАСТРОЙКИ СИГНАЛОВ - КАЛИБРОВКА B
    Page::Settings, Menu::mainPage, Item::FuncActive, FuncPress, OnKey, FuncBeforeDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageSignals::Init()
{
    OnPress_Channel(true);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageSignals::OnPress_Form(bool)
{
    ChoiceBase *choice = (ChoiceBase *)pageSignals.items[1];    // Указатель на ChoiceBase, хранящий индекс выбранной формы текущего канала                        //-V1027     

    WAVE_CURRENT.SetIndexForm(choice->CurrentIndex());          // Установить для текущего сигнала индекс формы из ChoiceBase

    cParameters.form = FORM_CURRENT;

    Generator::TuneChannel(CURRENT_CHANNEL);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageSignals::OnPress_Channel(bool)
{
    cParameters.form = FORM_CURRENT;
    numForm = FORM_CURRENT->value;
    pageSignals.items[1] = Chan(CURRENT_CHANNEL).IsA() ? (Item *)&cFormA : (Item *)&cFormB; //-V641
}
