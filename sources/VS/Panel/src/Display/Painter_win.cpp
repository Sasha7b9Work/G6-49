// 2022/8/1 11:34:42 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#undef CRC

#include "GUI/Application.h"
#include "GUI/GovernorGUI.h"
#include "GUI/Dimensions.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"


#define uint    unsigned int
#define int8    signed char
#define uint8   unsigned char
#define int16   signed short
#define uint16  unsigned short
#define uint    unsigned int
#define uchar   unsigned char
#define cstr pchar  const

#include "Display/Painter.h"
#include "Display/Text.h"

#pragma warning(push, 0)
#include <wx/display.h>
#pragma warning(pop)

#include "Hardware/CPU.h"

#undef uint   
#undef int8   
#undef uint8  
#undef int16  
#undef uint16 
#undef uint   
#undef uchar  
#undef cstr


#include "defines.h"
#include "Menu/Menu.h"
#include "Utils/Math.h"
#include "Settings/Settings.h"


// ����� ����� ��������
static wxBitmap bitmap(320, 240);
// �������� ���������
static wxMemoryDC memDC;

static wxButton *buttons[Key::Count];

static GovernorGUI *governor = nullptr;

// �����
static uint colors[256];

// ������ ���� ����������. ���������� ����� ������� ��� ���������
static void CreateFrame();
// ���������� ����������� ������� ��� ���� ����������
static void SetSizeAndPosition(Frame *frame);
// �������� ���������� ������������� ���������� � ������� ��������
static wxRect GetMaxDisplay();
// ������ ��� ������
static void CreateButtons(Frame *frame);
// ������ ���� ������
static void CreateButton(Key::E key, Frame *frame, const wxPoint &pos, const wxSize &size);


class Screen : public wxPanel
{
public:
    Screen(wxWindow *parent) : wxPanel(parent, 320)
    {
        SetMinSize({ Size::Screen::Width(), Size::Screen::Height() });
        SetDoubleBuffered(true);
        Bind(wxEVT_PAINT, &Screen::OnPaint, this);
    }

    void OnPaint(wxPaintEvent &)
    {
        wxPaintDC dc(this);
        dc.DrawBitmap(bitmap, 0, 0);
    }
};


static Screen *screen = nullptr;



void Display::Init()
{
    Font::ForceUpperCase(true);

    Font::Set(TypeFont::_8);

    CreateFrame();
}


void Painter::BeginScene(const Color &color)
{
    memDC.SelectObject(bitmap);
    wxBrush brush({ 0, 0, 0 }, wxTRANSPARENT);
    memDC.SetBrush(brush);
    Primitives::Rectangle(320, 240).Fill(0, 0, color);
}


void Painter::EndScene()
{
    memDC.SelectObject(wxNullBitmap);
    screen->Refresh();
}


static void SetSizeAndPosition(Frame *frame)
{
    wxSize size = { Size::Client::Width(), Size::Client::Height() };

    frame->SetMinClientSize(size);
    frame->SetMaxClientSize(size);

    wxRect rect = GetMaxDisplay();

    frame->SetPosition({ rect.width / 2 - size.GetWidth() / 2, rect.height / 2 - size.GetHeight() / 2 });
}


static wxRect GetMaxDisplay()
{
    wxRect result = {0, 0, 0, 0};

    for (uint i = 0; i < wxDisplay::GetCount(); i++)
    {
        wxDisplay display(i);

        wxRect rect = display.GetClientArea();
        if (rect.width > result.width)
        {
            result.width = rect.width;
            result.height = rect.height;
        }
    }

    return result;
}


static void CreateFrame()
{
    Frame *frame = new Frame("");

    SetSizeAndPosition(frame);

    wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);

    screen = new Screen(frame);

    sizer->Add(screen);

    frame->SetSizer(sizer);

    CreateButtons(frame);

    frame->Show(true);

}


static void CreateButtons(Frame *frame)
{
    static const Key::E keys[5][5] =
    {
        {Key::F1,   Key::_1,         Key::_2,        Key::_3,          Key::Esc},
        {Key::F2,   Key::_4,         Key::_5,        Key::_6,          Key::Left},
        {Key::F3,   Key::_7,         Key::_8,        Key::_9,          Key::Right},
        {Key::F4,   Key::Comma,      Key::_0,        Key::Minus,       Key::None},
        {Key::None, Key::RotateLeft, Key::RegButton, Key::RotateRight, Key::None}
    };

    int x0 = Size::Buttons::X0();
    int y0 = 10;

    int dX = 5;
    int dY = 5;

    int width = 60;
    int height = 25;

    wxSize size = { width, height };

    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            CreateButton(keys[i][j], frame, { x0 + j * (width + dX), y0 + i * (height + dY) }, size);
        }
    }

    governor = new GovernorGUI(frame, { 477, 170 });

    CreateButton(Key::On1, frame, { x0, 180 }, size);
    CreateButton(Key::On2, frame, { x0 + width + dX, 180 }, size);
}


static void CreateButton(Key::E key, Frame *frame, const wxPoint &pos, const wxSize &size)
{
    if (key == Key::None)
    {
        return;
    }

    wxButton *button = new wxButton(frame, (wxWindowID)key, Key(key).Name(), pos, size);

    button->Connect((wxWindowID)key, wxEVT_LEFT_DOWN, wxCommandEventHandler(Frame::OnKeyDown));
    button->Connect((wxWindowID)key, wxEVT_LEFT_UP, wxCommandEventHandler(Frame::OnKeyUp));

    buttons[key] = button;
}


void Color::SetAsCurrent() const
{
    current.value = value;

    if(value != COLOR_NUMBER)
    {
        uint color = COLOR(value);

        if (value == COLOR_FLASH_01 || value == COLOR_FLASH_10)
        {
            uint time = _TIME_MS + ((value == COLOR_FLASH_01) ? 0 : 500);

            color = ((time / 500) % 2) ? COLOR(COLOR_WHITE) : COLOR(COLOR_BLACK);
        }

        uint8 b = (uint8)(color);
        uint8 g = (uint8)(color >> 8);
        uint8 r = (uint8)(color >> 16);

        wxColour colorDraw = wxColour(r, g, b);

        memDC.SetPen(wxPen(colorDraw));
    }
}


void Primitives::VLine::Draw(int x, int y0, int y1, const Color &color)
{
    color.SetAsCurrent();
    memDC.DrawLine({ x, y0 }, { x, y1 });
}


void Primitives::HLine::Draw(int y, int x0, int x1, const Color &color)
{
    color.SetAsCurrent();
    memDC.DrawLine({ x0, y }, { x1, y });
}


void Primitives::Line::Draw(int x0, int y0, int x1, int y1, const Color &color)
{
    color.SetAsCurrent();
    memDC.DrawLine({ x0, y0 }, { x1, y1 });
}


void Painter::LoadPalette()
{

}


void Primitives::Rectangle::Fill(int x, int y, const Color &color)
{
    color.SetAsCurrent();

    wxBrush brush = memDC.GetBrush();
    wxPen pen = memDC.GetPen();
    memDC.SetBrush(wxBrush(pen.GetColour()));
    memDC.DrawRectangle({ x, y, width + 1, height + 1 });
    memDC.SetBrush(brush);
}


void Primitives::Rectangle::Draw(int x, int y, const Color &color)
{
    color.SetAsCurrent();

    memDC.DrawRectangle({ x, y, width + 1, height + 1 });
}


void Primitives::Rectangle::DrawFilled(int x, int y, Color colorFill, Color colorRect)
{
    Rectangle(width, height).Draw(x, y, colorRect);
    Rectangle(width - 2, height - 2).Fill(x + 1, y + 1, colorFill);
}


void Primitives::Point::Draw(int x, int y)
{
    memDC.DrawPoint({ x, y });
}



#include "Keyboard/Keyboard_win.h"
