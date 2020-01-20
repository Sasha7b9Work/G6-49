#include "Display/InputWindow.h"
#include "Settings/Settings.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Addition/PageInput.h"
#include "Menu/Pages/PageSignals/PageSignals.h"
#include "Generator/Signals.h"
#include "Generator/Generator_p.h"
#include "Menu/Menu.h"


extern const PageBase pageSignals;
Page *PageSignals::self = reinterpret_cast<Page *>(const_cast<PageBase *>(&pageSignals));
extern ChoiceParameterBase cParameters;
/// Номер текущей формы сигнал
static int numForm = 0;



DEF_CHOICE_2( cChannel,                                                                                                                                  //--- НАСТРОЙКИ СИГНАЛОВ - Канал ---
    "КАНАЛ",
    "Выбор канала для настройки."
    ,
    "A", "Управление параметрами сигнала на выходе A.",
    "B", "Управление параметрами сигнала на выходе B.",
    FLAG, BIT_CHANNEL, pageSignals, Item::FuncActive, PageSignals::OnPress_Channel, FuncDraw
)



DEF_CHOICE_8( cFormA,                                                                                                                                    //--- НАСТРОЙКИ СИГНАЛОВ - Форма ---
    "ФОРМА",
    "Выбор формы сигнала."
    ,
    FORM_RU(TypeForm::Sine),         "Синус",
    FORM_RU(TypeForm::RampPlus),     "Нарастающая пила",
    FORM_RU(TypeForm::RampMinus),    "Убывающая пила",
    FORM_RU(TypeForm::Triangle),     "Треугольник",
    FORM_RU(TypeForm::Meander),      "Меандр",
    FORM_RU(TypeForm::Impulse),      "Треугольник",
    FORM_RU(TypeForm::PacketImpuls), "Пакеты",
    FORM_RU(TypeForm::Free),         "Произвольный",
    numForm, pageSignals, Item::FuncActive, PageSignals::OnPress_Form, FuncDraw
)

DEF_CHOICE_7( cFormB,                                                                                                                                    //--- НАСТРОЙКИ СИГНАЛОВ - Форма ---
    "ФОРМА",
    "Выбор формы сигнала."
    ,
    FORM_RU(TypeForm::Sine),      "Синус",
    FORM_RU(TypeForm::RampPlus),  "Нарастающая пила",
    FORM_RU(TypeForm::RampMinus), "Убывающая пила",
    FORM_RU(TypeForm::Triangle),  "Треугольник",
    FORM_RU(TypeForm::Meander),   "Меандр",
    FORM_RU(TypeForm::Impulse),   "Треугольник",
    FORM_RU(TypeForm::Free),      "Произвольный",
    numForm, pageSignals, Item::FuncActive, PageSignals::OnPress_Form, FuncDraw
)


static void OnPress_ChnageParameter()
{
    CURRENT_FORM->ChangeParameter();
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


DEF_CHOICE_2( cTypeTune,                                                                                                                               //--- НАСТРОЙКИ СИГНАЛОВ - Засылка ---
    "ЗАСЫЛКА",
    "Определяет периодичность засылки настроек сигналов в устройство управления.",
    "ОДНОКРАТНО", "Засылка настроек сигнала происходит однократно, после нажатия кнопки подтверждения.",
    "НЕПРЕРЫВНО", "Засылка настроек сигнала происходит синхронно с изменением информации на экране.",
    FLAG, BIT_TUNE_FULL, pageSignals, Item::FuncActive, FuncChangedChoice, FuncDraw
)


static bool OnKey_PageSignals(Key &key)
{
    if(key.Is(Key::Esc) && key.action.IsRelease())
    {
        return CURRENT_FORM->CloseOpenedParameter();
    }

    return false;
}

DEF_PAGE_5( pageSignals,   //-V641
    "НАСТРОЙКИ СИГНАЛОВ",  
    "",
    &cChannel,                                      ///< НАСТРОЙКИ СИГНАЛОВ - Канал
    &cFormA,                                        ///< НАСТРОЙКИ СИГНАЛОВ - Форма
    &cParameters,                                   ///< НАСТРОЙКИ СИГНАЛОВ - Параметр
    &bChangeParameter,                              ///< НАСТРОЙКИ СИГНАЛОВ - Ввести значение параметра
    &cTypeTune,                                     ///< НАСТРОЙКИ СИГНАЛОВ - Засылки
    Page::Settings, Menu::mainPage, Item::FuncActive, FuncPress, OnKey_PageSignals, FuncBeforeDraw
)


void PageSignals::Init()
{
    OnPress_Channel(true);
}


void PageSignals::OnPress_Form(bool)
{
    ChoiceBase *choice = reinterpret_cast<ChoiceBase *>(pageSignals.items[1]);    // Указатель на ChoiceBase, хранящий индекс выбранной формы текущего канала

    CURRENT_WAVE.SetIndexForm(choice->CurrentIndex());          // Установить для текущего сигнала индекс формы из ChoiceBase

    cParameters.form = CURRENT_FORM;

    PGenerator::TuneChannel(CURRENT_CHANNEL);
}


void PageSignals::SetForm(TypeForm::E form)
{
    if(form == TypeForm::Free && CURRENT_CHANNEL_IS_B)
    {
        return;
    }

    numForm = form;
    OnPress_Form(true);
}


void PageSignals::OnPress_Channel(bool)
{
    cParameters.form = CURRENT_FORM;
    numForm = CURRENT_FORM->value;

    pageSignals.items[1] = reinterpret_cast<Item *>(const_cast<ChoiceBase *>(Chan(CURRENT_CHANNEL).IsA() ? &cFormA : &cFormB));
}


void PageSignals::SetCurrentChanenl(Chan::E ch)
{
    if(ch == Chan::A)
    {
        CLEAR_FL(BIT_CHANNEL);
    }
    else
    {
        SET_FL(BIT_CHANNEL);
    }
}
