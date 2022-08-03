// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "Display/Painter.h"
#include "Signals/Signals.h"
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

// ����� ������� ����� ������
static int numForm = 0;


void PageSignals::Init()
{
    OnPress_Channel(true);
}


void PageSignals::SCPI_SetForm(TypeForm::E form)
{
    if (form == TypeForm::Packet && CURRENT_CHANNEL.IsB())
    {
        SCPI::SendAnswer("Can not set form \"packet\" on channel B");
        return;
    }

    numForm = (form == TypeForm::Free && CURRENT_CHANNEL.IsB()) ? (form - 1) : form;
    OnChanged_Form();
}


void PageSignals::SetCurrentChanenl(const Chan &ch)
{
    set.cur_chan = ch;
    OnPress_Channel(true);
}


static bool IsActive_Channel()
{
    if (CURRENT_CHANNEL.IsA() && CURRENT_FORM->Is(TypeForm::Packet))
    {
        return false;
    }

    return true;
}


void PageSignals::OnPress_Channel(bool active)
{
    if (active)
    {
        cParameters.form = CURRENT_FORM;
        numForm = CURRENT_FORM->value;

        pageSignals.items[1] = reinterpret_cast<Item *>(const_cast<ChoiceBase *>(Chan(CURRENT_CHANNEL).IsA() ? &cFormA : &cFormB));

        PGenerator::TuneChannel(CURRENT_CHANNEL);
    }
    else
    {
        if (CURRENT_CHANNEL.IsA() && CURRENT_FORM->Is(TypeForm::Packet))
        {
            Display::Warnings::Show("� ������ ����� ����� ������ � ����������", "Channel B cannot be selected in PACKET mode", true);
        }
    }
}


DEF_CHOICE_2( cChannel,                                                                                                                                  //--- ��������� �������� - ����� ---
    "�����", "CHANNEL",
    "����� ������ ��� ���������", "Channel selection for tuning",
    "A",                                          "A",
    "���������� ����������� ������� �� ������ A", "Control the parameters of the signal at output A",
    "B",                                          "B",
    "���������� ����������� ������� �� ������ B", "Control the parameters of the signal at output B",
    set.cur_chan, *PageSignals::self, IsActive_Channel, PageSignals::OnPress_Channel, FuncDraw
)


static void ChangedForm()
{
    ChoiceBase *choice = reinterpret_cast<ChoiceBase *>(pageSignals.items[1]);      // ��������� �� ChoiceBase, �������� ������ ��������� ����� �������� ������

    CURRENT_WAVE.SetIndexForm(choice->CurrentIndex());                              // ���������� ��� �������� ������� ������ ����� �� ChoiceBase

    cParameters.form = CURRENT_FORM;

    Display::Update();

    PGenerator::TuneChannel(CURRENT_CHANNEL);
}


// �������� ��������� ����������� ������� ������ � ������������ � ����������� ������ ��������� �� ������ �
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


void PageSignals::OnChanged_Form(bool active)
{
    if (active)
    {
        ChangedForm();

        if (CURRENT_CHANNEL.IsA())
        {
            if (CURRENT_FORM->Is(TypeForm::Packet))            // ����� � �������� �����
            {
                S::B::impulse->StoreState();               // ��������� ��������� ��������� �� ������ ������

                WAVE_B.StoreIndexCurrentForm();

                WAVE_B.SetForm(S::B::impulse);         // ������������� ����� �������� �� ������ ������

                WriteParametersImpulseFromPacket(S::B::impulse, S::A::packet);

                SetCurrentChanenl(ChB);

                OnChanged_Form(true);

                SetCurrentChanenl(ChA);
            }
            else if (CURRENT_FORM->Is(TypeForm::Free))          // ����� �� ��������� ������
            {
                FORM_B->RestoreState();

                WAVE_B.RestoreIndexCurrentForm();

                SetCurrentChanenl(ChB);

                OnChanged_Form(true);

                SetCurrentChanenl(ChA);
            }
        }
    }
    else
    {
        if (FORM_A == S::A::Impulse::self && FORM_B == S::B::impulse)
        {
            ParameterChoice *parameter = FORM_A->FindParameter(ParameterChoiceType::ModeStartStop);

            if (parameter->GetChoice() == 1)
            {
                Display::Warnings::Show("��������� ����� �-�����,�-����",
                                        "Disable the A-START,B-STOP mode", true);
            }
        }
    }
}


static bool IsActive_Form()
{
    if (FORM_A->Is(TypeForm::Impulse) && FORM_B->Is(TypeForm::Impulse))
    {
        ParameterChoice *parameter = S::A::Impulse::self->FindParameter(ParameterChoiceType::ModeStartStop);

        if (parameter->GetChoice() == 1)
        {
            return false;
        }
    }

    return true;
}


DEF_CHOICE_8( cFormA,                                                                                                                                    //--- ��������� �������� - ����� ---
    "�����", "FORM",
    "����� ����� �������", "Waveform selection",
    FORM_RU(TypeForm::Sine),        FORM_EN(TypeForm::Sine),        "�����",            "Sinus",
    FORM_RU(TypeForm::RampPlus),    FORM_EN(TypeForm::RampPlus),    "����������� ����", "Escalating saw",
    FORM_RU(TypeForm::RampMinus),   FORM_EN(TypeForm::RampMinus),   "��������� ����",   "Waning saw",
    FORM_RU(TypeForm::Triangle),    FORM_EN(TypeForm::Triangle),    "�����������",      "Triangle",
    FORM_RU(TypeForm::Meander),     FORM_EN(TypeForm::Meander),     "������",           "Meander",
    FORM_RU(TypeForm::Impulse),     FORM_EN(TypeForm::Impulse),     "��������",         "Impulse",
    FORM_RU(TypeForm::Packet),      FORM_EN(TypeForm::Packet),      "������",           "Packets",
    FORM_RU(TypeForm::Free),        FORM_EN(TypeForm::Free),        "������������",     "Free",
    numForm, *PageSignals::self, IsActive_Form, PageSignals::OnChanged_Form, FuncDraw
)

DEF_CHOICE_7( cFormB,                                                                                                                                    //--- ��������� �������� - ����� ---
    "�����", "FORM",
    "����� ����� �������", "Waveform selection",
    FORM_RU(TypeForm::Sine),      FORM_RU(TypeForm::Sine),      "�����",            "Sinus",
    FORM_RU(TypeForm::RampPlus),  FORM_RU(TypeForm::RampPlus),  "����������� ����", "Escalating saw",
    FORM_RU(TypeForm::RampMinus), FORM_RU(TypeForm::RampMinus), "��������� ����",   "Waning saw",
    FORM_RU(TypeForm::Triangle),  FORM_RU(TypeForm::Triangle),  "�����������",      "Triangle",
    FORM_RU(TypeForm::Meander),   FORM_RU(TypeForm::Meander),   "������",           "Meander",
    FORM_RU(TypeForm::Impulse),   FORM_RU(TypeForm::Impulse),   "��������",         "Impulse",
    FORM_RU(TypeForm::Free),      FORM_RU(TypeForm::Free),      "������������",     "Free",
    numForm, *PageSignals::self, IsActive_Form, PageSignals::OnChanged_Form, FuncDraw
)


static bool IsActive_TuneParameter()
{
    return CURRENT_PARAM->funcOfActive();
}


static void OnPress_TuneParameter()
{
    if (IsActive_TuneParameter())
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
    else
    {
        if (CURRENT_CHANNEL.IsA())
        {
            if (FORM_A == S::A::Impulse::self)
            {
                ParameterChoice *start_stop = FORM_A->FindParameter(ParameterChoiceType::ModeStartStop);
                ParameterDouble *delay = FORM_A->FindParameter(ParameterDoubleType::Delay);

                if (CURRENT_PARAM == start_stop || CURRENT_PARAM == delay)
                {
                    Display::Warnings::Show("�������� �� ������ � ����� �������", "Select on the channel To form IMPULSE", true);
                }
            }
        }
        else if(CURRENT_CHANNEL.IsB())
        {
            if (FORM_B == S::B::impulse)
            {
                if (CURRENT_PARAM == FORM_B->FindParameter(ParameterDoubleType::Period))
                {
                    if (FORM_A->FindParameter(ParameterChoiceType::ModeStartStop)->GetChoice() == 0)
                    {
                        if (FORM_B->FindParameter(ParameterChoiceType::ModeStart)->GetChoice() == 1)
                        {
                            Display::Warnings::Show("���������� ��� ����������� �������", "Not available on single launch", true);
                        }
                    }
                }
            }
        }
    }
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


DEF_BUTTON( bTuneParameter,                                                                                                        //--- ��������� �������� - ������ �������� ��������� ---
    "��������", "Change",
    "��������� ���� ����� ���������", "Opens the parameter input window",
    *PageSignals::self, IsActive_TuneParameter, OnPress_TuneParameter, OnDraw_TuneParameter
)


DEF_CHOICE_PARAMETER( cParameters,                                                                                                                    //--- ��������� �������� - �������� ---
    "��������", "PARAMETER",
    "����� ��������� ��� ���������", "Select an option to configure",
    *PageSignals::self, Item::FuncActive, OnPress_TuneParameter, WAVE(ChA).GetForm(0)
)


DEF_PAGE_4( pageSignals,   //-V641
    "��������� ��������", "SIGNAL SETTINGS",   //-V641 //-V1027
    "", "",
    &cChannel,                  // ��������� �������� - �����
    &cFormA,                    // ��������� �������� - �����
    &cParameters,               // ��������� �������� - ��������
    &bTuneParameter,            // ��������� �������� - ������ �������� ���������
    Page::Signals, PageMain::self, Item::FuncActive, Page::FuncEnter, FuncOnKey, FuncBeforeDraw
)

Page *PageSignals::self = reinterpret_cast<Page *>(const_cast<PageBase *>(&pageSignals));
