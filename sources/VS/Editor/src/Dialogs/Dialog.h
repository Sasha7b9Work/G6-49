// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Editor/Form.h"

#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)


class SpinControl;


class Dialog : public wxDialog
{
public:
    Dialog(const wxString &title, bool blockingCanvas);
    virtual ~Dialog();

    void OnControlEvent(wxCommandEvent &);

    // ������� ����� ��� ��������������� ���������
    virtual void SendAdditionForm() = 0;

    void OnKeyUp(wxKeyEvent &);

    static const int WIDTH_PANEL = 220;

protected:

    virtual void SaveValues() = 0;

    void SetBoxSizer(wxBoxSizer *sizer, wxSize size);

    void DrawLine(int x1, int y1, int x2, int y2);
    // ������ ��� ������� � TheForm
    uint16 data[Point::AMOUNT];
    // �������������� ����� ��� ������� � TheForm
    std::vector<Point> points;

    // ������ ������ ���������� �������
    wxPanel *CreatePanelPolarity(bool polarityDirect, bool polarityBack);
    wxRadioButton *rbPolarityDirect = nullptr;
    wxRadioButton *rbPolarityBack = nullptr;

    // ������ ������ ������� �������
    wxPanel *CreatePanelLevels(int levelUp, int levelDown);
    SpinControl *scLevelUp = nullptr;
    SpinControl *scLevelDown = nullptr;

    bool isBlockingCanvas = false;      // ���� true, �� ����� ����� �����������

private:
    wxBoxSizer *panelBox = nullptr;

    void OnMove(wxMoveEvent &);

    void OnButtonApply(wxCommandEvent &);

    void OnButtonCancel(wxCommandEvent &);
};
