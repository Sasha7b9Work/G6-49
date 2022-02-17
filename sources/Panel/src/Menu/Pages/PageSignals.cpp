#include "Display/Painter.h"
#include "Generator/Signals.h"
#include "Generator/Generator_p.h"
#include "Menu/Menu.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Pages.h"
#include "SCPI/SCPI.h"
#include "Settings/Settings.h"


extern ChoiceParameterBase cParameters;
// ����� ������� ����� ������
static int numForm = 0;



DEF_CHOICE_2( cChannel,                                                                                                                                  //--- ��������� �������� - ����� ---
    "�����", "CHANNEL",
    "����� ������ ��� ���������", "Channel selection for tuning",
    "A",                                          "A",
    "���������� ����������� ������� �� ������ A", "Control the parameters of the signal at output A",
    "B",                                          "B",
    "���������� ����������� ������� �� ������ B", "Control the parameters of the signal at output B",
    set.current, *PageSignals::self, Item::FuncActive, PageSignals::OnPress_Channel, FuncDraw
)



DEF_CHOICE_8( cFormA,                                                                                                                                    //--- ��������� �������� - ����� ---
    "�����", "FORM",
    "����� ����� �������", "Waveform selection",
    FORM_RU(TypeForm::Sine),         FORM_EN(TypeForm::Sine),         "�����",            "Sinus",
    FORM_RU(TypeForm::RampPlus),     FORM_EN(TypeForm::RampPlus),     "����������� ����", "Escalating saw",
    FORM_RU(TypeForm::RampMinus),    FORM_EN(TypeForm::RampMinus),    "��������� ����",   "Waning saw",
    FORM_RU(TypeForm::Triangle),     FORM_EN(TypeForm::Triangle),     "�����������",      "Triangle",
    FORM_RU(TypeForm::Meander),      FORM_EN(TypeForm::Meander),      "������",           "Meander",
    FORM_RU(TypeForm::Impulse),      FORM_EN(TypeForm::Impulse),      "��������",         "Impulse",
    FORM_RU(TypeForm::PacketImpuls), FORM_EN(TypeForm::PacketImpuls), "������",           "Packets",
    FORM_RU(TypeForm::Free),         FORM_EN(TypeForm::Free),         "������������",     "Free",
    numForm, *PageSignals::self, Item::FuncActive, PageSignals::OnChanged_Form, FuncDraw
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


static void OnDraw_TuneParameter(int x, int y);


DEF_BUTTON( bTuneParameter,                                                                                                        //--- ��������� �������� - ������ �������� ��������� ---
    "��������", "Change",
    "��������� ���� ����� ���������", "Opens the parameter input window",
    *PageSignals::self, Item::FuncActive, OnPress_TuneParameter, OnDraw_TuneParameter
)

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

DEF_CHOICE_PARAMETER( cParameters,                                                                                                                    //--- ��������� �������� - �������� ---
    "��������", "PARAMETER",
    "����� ��������� ��� ���������", "Select an option to configure",
    *PageSignals::self, Item::FuncActive, OnPress_TuneParameter, WAVE(Chan::A).GetForm(0)
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


bool PageSignals::OnSubPageTuneChannels()
{
    return (CURRENT_PAGE == PageSignals::self) && (CURRENT_PAGE->CurrentSubPage() == 0);
}


void PageSignals::Init()
{
    OnPress_Channel(true);
}


void PageSignals::OnChanged_Form(bool)
{
    ChoiceBase *choice = reinterpret_cast<ChoiceBase *>(pageSignals.items[1]);      // ��������� �� ChoiceBase, �������� ������ ��������� ����� �������� ������

    CURRENT_WAVE.SetIndexForm(choice->CurrentIndex());                              // ���������� ��� �������� ������� ������ ����� �� ChoiceBase

    cParameters.form = CURRENT_FORM;

    Display::Update();

    PGenerator::TuneChannel(CURRENT_CHANNEL);
}


void PageSignals::SCPI_SetForm(TypeForm::E form)
{
    if(form == TypeForm::PacketImpuls && CURRENT_CHANNEL_IS_B)
    {
        SCPI::SendAnswer("Can not set form \"packet\" on channel B");
        return;
    }
   
    numForm = (form == TypeForm::Free && CURRENT_CHANNEL_IS_B) ? (form - 1) : form;
    OnChanged_Form();
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
    OnPress_Channel(true);
}
