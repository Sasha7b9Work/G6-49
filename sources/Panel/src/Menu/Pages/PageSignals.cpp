// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "Display/Painter.h"
#include "Generator/Signals.h"
#include "Generator/Generator_p.h"
#include "Menu/Menu.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Pages.h"
#include "SCPI/SCPI.h"
#include "Settings/Settings.h"


extern ChoiceParameterBase cParameters;
extern const PageBase   pageSignals;
extern const ChoiceBase cFormA;
extern const ChoiceBase cFormB;
extern const ButtonBase bTuneParameter;

// Номер текущей формы сигнал
static int numForm = 0;


static bool IsActive_Channel()
{
    return !(CURRENT_CHANNEL_IS_A && CURRENT_FORM->Is(TypeForm::Packet));
}


void PageSignals::OnPress_Channel(bool)
{
    cParameters.form = CURRENT_FORM;
    numForm = CURRENT_FORM->value;

    pageSignals.items[1] = reinterpret_cast<Item *>(const_cast<ChoiceBase *>(Chan(CURRENT_CHANNEL).IsA() ? &cFormA : &cFormB));

    PGenerator::TuneChannel(CURRENT_CHANNEL);
}


DEF_CHOICE_2( cChannel,                                                                                                                                  //--- НАСТРОЙКИ СИГНАЛОВ - Канал ---
    "КАНАЛ", "CHANNEL",
    "Выбор канала для настройки", "Channel selection for tuning",
    "A",                                          "A",
    "Управление параметрами сигнала на выходе A", "Control the parameters of the signal at output A",
    "B",                                          "B",
    "Управление параметрами сигнала на выходе B", "Control the parameters of the signal at output B",
    set.cur_chan, *PageSignals::self, IsActive_Channel, PageSignals::OnPress_Channel, FuncDraw
)


static void ChangedForm()
{
    ChoiceBase *choice = reinterpret_cast<ChoiceBase *>(pageSignals.items[1]);      // Указатель на ChoiceBase, хранящий индекс выбранной формы текущего канала

    CURRENT_WAVE.SetIndexForm(choice->CurrentIndex());                              // Установить для текущего сигнала индекс формы из ChoiceBase

    cParameters.form = CURRENT_FORM;

    Display::Update();

    PGenerator::TuneChannel(CURRENT_CHANNEL);
}


// Записать параметры импульсного сигнала канала В соответствии с параметрами пакета импульсов из канала А
static void WriteParametersImpulseFromPacket(Form *formImpulse, Form *formPacket)
{
    ParameterDouble *period_packet = formPacket->FindParameter(ParameterDoubleType::PacketPeriod);

    formImpulse->FindParameter(ParameterDoubleType::Period)->SetValue(period_packet->GetValue());

    Value period_impulse = formPacket->FindParameter(ParameterDoubleType::Period)->GetValue();
    Value duration_impulse = formPacket->FindParameter(ParameterDoubleType::Duration)->GetValue();
    Value number_impulse = formPacket->FindParameter(ParameterIntegerType::PacketNumber)->GetValue();

    double duration = (number_impulse.ToDouble() - 1.0) * period_impulse.ToDouble() + duration_impulse.ToDouble();

    formImpulse->FindParameter(ParameterDoubleType::Duration)->SetValue(Value(duration));
}


void PageSignals::OnChanged_Form(bool)
{
    ChangedForm();

    if (CURRENT_CHANNEL_IS_A)
    {
        static int index_form = 0;

        if (CURRENT_FORM->Is(TypeForm::Packet))            // Вошли в пакетный режим
        {
            index_form = WAVE_B.GetIndexForm();

            Signals::B::impulse->SaveState();               // Сохраняем параметры импульсов на втором канале

            WAVE_B.SetForm(Signals::B::impulse);         // Устанавливаем форму импульса на втором канале

            WriteParametersImpulseFromPacket(Signals::B::impulse, Signals::A::packet);

            SetCurrentChanenl(ChB);

            OnChanged_Form(true);

            SetCurrentChanenl(ChA);
        }
        else if (CURRENT_FORM->Is(TypeForm::Free))          // Вышли из пакетного режима
        {
            FORM_B->RestoreState();

            WAVE_B.SetIndexForm(index_form);

            SetCurrentChanenl(ChB);

            OnChanged_Form(true);

            SetCurrentChanenl(ChA);
        }
    }
}


DEF_CHOICE_8( cFormA,                                                                                                                                    //--- НАСТРОЙКИ СИГНАЛОВ - Форма ---
    "ФОРМА", "FORM",
    "Выбор формы сигнала", "Waveform selection",
    FORM_RU(TypeForm::Sine),        FORM_EN(TypeForm::Sine),        "Синус",            "Sinus",
    FORM_RU(TypeForm::RampPlus),    FORM_EN(TypeForm::RampPlus),    "Нарастающая пила", "Escalating saw",
    FORM_RU(TypeForm::RampMinus),   FORM_EN(TypeForm::RampMinus),   "Убывающая пила",   "Waning saw",
    FORM_RU(TypeForm::Triangle),    FORM_EN(TypeForm::Triangle),    "Треугольник",      "Triangle",
    FORM_RU(TypeForm::Meander),     FORM_EN(TypeForm::Meander),     "Меандр",           "Meander",
    FORM_RU(TypeForm::Impulse),     FORM_EN(TypeForm::Impulse),     "Импульсы",         "Impulse",
    FORM_RU(TypeForm::Packet),      FORM_EN(TypeForm::Packet),      "Пакеты",           "Packets",
    FORM_RU(TypeForm::Free),        FORM_EN(TypeForm::Free),        "Произвольный",     "Free",
    numForm, *PageSignals::self, Item::FuncActive, PageSignals::OnChanged_Form, FuncDraw
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
    numForm, *PageSignals::self, Item::FuncActive, PageSignals::OnChanged_Form, FuncDraw
)


static void OnPress_TuneParameter()
{
    if (CURRENT_PARAM->IsDouble())
    {
        ParameterDouble *param = reinterpret_cast<ParameterDouble *>(CURRENT_PARAM);

        if (param->GetType() == ParameterDoubleType::ManipulationDuration || param->GetType() == ParameterDoubleType::ManipulationPeriod)
        {
            return;
        }
    }

    CURRENT_PARAM->OnPressButtonTune();
}


static void OnDraw_TuneParameter(int x, int y)
{
    if (CURRENT_PARAM->IsDouble())
    {
        ParameterDouble *param = reinterpret_cast<ParameterDouble *>(CURRENT_PARAM);

        if (param->GetType() == ParameterDoubleType::ManipulationDuration || param->GetType() == ParameterDoubleType::ManipulationPeriod)
        {
            const Item *item = reinterpret_cast<const Item *>(&bTuneParameter);

            bool isShade = item->IsShade();

            Primitives::Rectangle(Item::WIDTH - 5, Item::HEIGHT - 4).Fill(x + 2, y + 2, isShade ? Color::MENU_ITEM_SHADE : Color::GREEN_10);
        }
    }
}


DEF_BUTTON( bTuneParameter,                                                                                                        //--- НАСТРОЙКИ СИГНАЛОВ - Ввести значение параметра ---
    "Изменить", "Change",
    "Открывает окно ввода параметра", "Opens the parameter input window",
    *PageSignals::self, Item::FuncActive, OnPress_TuneParameter, OnDraw_TuneParameter
)


DEF_CHOICE_PARAMETER( cParameters,                                                                                                                    //--- НАСТРОЙКИ СИГНАЛОВ - Параметр ---
    "ПАРАМЕТР", "PARAMETER",
    "Выбор параметра для настройки", "Select an option to configure",
    *PageSignals::self, Item::FuncActive, OnPress_TuneParameter, WAVE(Chan::A).GetForm(0)
)


DEF_PAGE_4( pageSignals,   //-V641
    "НАСТРОЙКИ СИГНАЛОВ", "SIGNAL SETTINGS",   //-V641 //-V1027
    "", "",
    &cChannel,                  // НАСТРОЙКИ СИГНАЛОВ - Канал
    &cFormA,                    // НАСТРОЙКИ СИГНАЛОВ - Форма
    &cParameters,               // НАСТРОЙКИ СИГНАЛОВ - Параметр
    &bTuneParameter,            // НАСТРОЙКИ СИГНАЛОВ - Ввести значение параметра
    Page::Signals, PageMain::self, Item::FuncActive, Page::FuncEnter, FuncOnKey, FuncBeforeDraw
)

Page *PageSignals::self = reinterpret_cast<Page *>(const_cast<PageBase *>(&pageSignals));


bool PageSignals::OnSubPageTuneChannels()
{
    return (CURRENT_PAGE == PageSignals::self) && (CURRENT_PAGE->CurrentSubPage() == 0);
}


void PageSignals::Init()
{
    OnPress_Channel(true);
}


void PageSignals::SCPI_SetForm(TypeForm::E form)
{
    if(form == TypeForm::Packet && CURRENT_CHANNEL_IS_B)
    {
        SCPI::SendAnswer("Can not set form \"packet\" on channel B");
        return;
    }
   
    numForm = (form == TypeForm::Free && CURRENT_CHANNEL_IS_B) ? (form - 1) : form;
    OnChanged_Form();
}


void PageSignals::SetCurrentChanenl(Chan::E ch)
{
    set.cur_chan = ch;
    OnPress_Channel(true);
}
