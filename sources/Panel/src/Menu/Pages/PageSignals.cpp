#include "Settings/Settings.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Pages.h"
#include "Generator/Signals.h"
#include "Generator/Generator_p.h"
#include "Menu/Menu.h"


extern const PageBase pageSignals;
Page *PageSignals::self = reinterpret_cast<Page *>(const_cast<PageBase *>(&pageSignals));
extern ChoiceParameterBase cParameters;
// Номер текущей формы сигнал
static int numForm = 0;



DEF_CHOICE_2( cChannel,                                                                                                                                  //--- НАСТРОЙКИ СИГНАЛОВ - Канал ---
    "КАНАЛ", "CHANNEL",
    "Выбор канала для настройки", "Channel selection for tuning",
    "A",                                          "A",
    "Управление параметрами сигнала на выходе A", "Control the parameters of the signal at output A",
    "B",                                          "B",
    "Управление параметрами сигнала на выходе B", "Control the parameters of the signal at output B",
    set.current, pageSignals, Item::FuncActive, PageSignals::OnPress_Channel, FuncDraw
)



DEF_CHOICE_8( cFormA,                                                                                                                                    //--- НАСТРОЙКИ СИГНАЛОВ - Форма ---
    "ФОРМА", "FORM",
    "Выбор формы сигнала", "Waveform selection",
    FORM_RU(TypeForm::Sine),         FORM_EN(TypeForm::Sine),         "Синус",            "Sinus",
    FORM_RU(TypeForm::RampPlus),     FORM_EN(TypeForm::RampPlus),     "Нарастающая пила", "Escalating saw",
    FORM_RU(TypeForm::RampMinus),    FORM_EN(TypeForm::RampMinus),    "Убывающая пила",   "Waning saw",
    FORM_RU(TypeForm::Triangle),     FORM_EN(TypeForm::Triangle),     "Треугольник",      "Triangle",
    FORM_RU(TypeForm::Meander),      FORM_EN(TypeForm::Meander),      "Меандр",           "Meander",
    FORM_RU(TypeForm::Impulse),      FORM_EN(TypeForm::Impulse),      "Импульсы",         "Impulse",
    FORM_RU(TypeForm::PacketImpuls), FORM_EN(TypeForm::PacketImpuls), "Пакеты",           "Packets",
    FORM_RU(TypeForm::Free),         FORM_EN(TypeForm::Free),         "Произвольный",     "Free",
    numForm, pageSignals, Item::FuncActive, PageSignals::OnPress_Form, FuncDraw
)

DEF_CHOICE_7( cFormB,                                                                                                                                    //--- НАСТРОЙКИ СИГНАЛОВ - Форма ---
    "ФОРМА", "FORM",
    "Выбор формы сигнала", "Waveform selection",
    FORM_RU(TypeForm::Sine),      FORM_RU(TypeForm::Sine),      "Синус",            "Sinus",
    FORM_RU(TypeForm::RampPlus),  FORM_RU(TypeForm::RampPlus),  "Нарастающая пила", "Escalating saw",
    FORM_RU(TypeForm::RampMinus), FORM_RU(TypeForm::RampMinus), "Убывающая пила",   "Waning saw",
    FORM_RU(TypeForm::Triangle),  FORM_RU(TypeForm::Triangle),  "Треугольник",      "Triangle",
    FORM_RU(TypeForm::Meander),   FORM_RU(TypeForm::Meander),   "Меандр",           "Meander",
    FORM_RU(TypeForm::Impulse),   FORM_RU(TypeForm::Impulse),   "Импульсы",         "Impulse",
    FORM_RU(TypeForm::Free),      FORM_RU(TypeForm::Free),      "Произвольный",     "Free",
    numForm, pageSignals, Item::FuncActive, PageSignals::OnPress_Form, FuncDraw
)


static void OnPress_TuneParameter()
{
    CURRENT_PARAM->OnPressButtonTune();
}

DEF_BUTTON( bTuneParameter,                                                                                                        //--- НАСТРОЙКИ СИГНАЛОВ - Ввести значение параметра ---
    "Изменить", "Change",
    "Открывает окно ввода параметра", "Opens the parameter input window",
    pageSignals, Item::FuncActive, OnPress_TuneParameter, FuncDraw
)

DEF_CHOICE_PARAMETER( cParameters,                                                                                                                    //--- НАСТРОЙКИ СИГНАЛОВ - Параметр ---
    "ПАРАМЕТР", "PARAMETER",
    "Выбор параметра для настройки", "Select an option to configure",
    pageSignals, Item::FuncActive, OnPress_TuneParameter, WAVE(Chan::A).GetForm(0)
)


static bool OnKey_PageSignals(const Control) //-V801
{
    return false;
//    if(CURRENT_PAGE != PageSignals::self)
//    {
//        return false;
//    }
//
//    return CURRENT_PARAM->ProcessKey(key);
}

DEF_PAGE_4( pageSignals,   //-V641
    "НАСТРОЙКИ СИГНАЛОВ", "SIGNAL SETTINGS",   //-V641 //-V1027
    "", "",
    &cChannel,              // НАСТРОЙКИ СИГНАЛОВ - Канал
    &cFormA,                // НАСТРОЙКИ СИГНАЛОВ - Форма
    &cParameters,           // НАСТРОЙКИ СИГНАЛОВ - Параметр
    &bTuneParameter,        // НАСТРОЙКИ СИГНАЛОВ - Ввести значение параметра
    Page::Settings, Menu::mainPage, Item::FuncActive, Page::FuncEnter, OnKey_PageSignals, FuncBeforeDraw
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
    if(form == TypeForm::Free && (CURRENT_CHANNEL == Chan::B))
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

    PGenerator::TuneChannel(CURRENT_CHANNEL);
}


void PageSignals::SetCurrentChanenl(Chan::E ch)
{
    set.current = ch;
}
