#include "defines.h"
#include "log.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Pages.h"
#include "Display/Painter.h"
#include "FDrive/FDrive_p.h"


extern const PageBase pageLoad;
Page *PageLoadForm::self = reinterpret_cast<Page *>(const_cast<PageBase *>(&pageLoad));


DEF_BUTTON( bUp,                                                                                                                              //--- ��������� �������� - �������� - ����� ---
    "�����", "Up",
    "", "",
    pageLoad, Item::FuncActive, FDrive::PressUp, FuncDraw
)

volatile const ButtonBase *pbUp = &bUp;


DEF_BUTTON( bDown,                                                                                                                             //--- ��������� �������� - �������� - ���� ---
    "����", "Down",
    "", "",
    pageLoad, Item::FuncActive, FDrive::PressDown, FuncDraw
)

volatile const ButtonBase *pbDown = &bDown;


DEF_BUTTON( bEnter,                                                                                                                           //--- ��������� �������� - �������� - Enter ---
    "�����", "Choice",
    "", "",
    pageLoad, Item::FuncActive, Button::FuncPress, FuncDraw
)

volatile const ButtonBase *pbEnter = &bEnter;


static void OnPress_Choose()
{
    FDrive::PressChoose();
    PageSignals::OnChanged_Form();
}

DEF_BUTTON( bChoose,                                                                                                                     //--- ��������� �������� - �������� - ���������� ---
    "�������", "Select",
    "", "",
    pageLoad, Item::FuncActive, OnPress_Choose, FuncDraw
)



static bool OnControl(const Control control) //-V801
{
    if (control.Is(Key::RotateLeft))
    {
        FDrive::PressDown();
        return true;
    }
    else if (control.Is(Key::RotateRight))
    {
        FDrive::PressUp();
        return true;
    }
    else if (control.Is(Key::Esc, Action::Up))
    {
        Menu::ResetAdditionPage();
        return true;
    }

    return false;
}

DEF_PAGE_4( pageLoad,                                                                                                                         //--- ��������� �������� - �������� --- //-V641
    "�����", "CHOICE",   //-V641
    "�������� ������ ����� ������������� ������� ��� �������� � ������", "The page for choosing the shape of an arbitrary waveform to boot from a USB flash drive",
    &bChoose,   // ��������� �������� - �������� - ����������
    0,
    0,
    0,
    Page::Settings_Load, nullptr, Item::FuncActive, Page::FuncEnter, OnControl, FDrive::Draw
)


void PageLoadForm::LoadForm(Chan::E)
{
    Menu::SetAdditionPage(self);
}
