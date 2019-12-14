#include "defines.h"
#undef CRC

#include "GUI/Application.h"


#define uint    unsigned int
#define int8    signed char
#define uint8   unsigned char
#define int16   signed short
#define uint16  unsigned short
#define uint    unsigned int
#define uchar   unsigned char
#define pString const char * const

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
#undef pString


#include "defines.h"
#include "Menu/Menu.h"
#include "Utils/Math.h"
#include "Settings/Settings.h"


/// Здесь будем рисовать
static wxBitmap bitmap(320, 240);
/// Контекст рисования
static wxMemoryDC memDC;

static wxButton *buttons[KeyEvent::Count];

/// Цвета
static uint colors[256];

/// Создаёт окно приложения. Возвращает хэндл виджета для отрисовки
static void CreateFrame();
/// Установить оптимальную позицию для окна приложения
static void SetSizeAndPosition(Frame *frame);
/// Получить разрешение максимального имеющегося в системе монитора
static wxRect GetMaxDisplay();
/// Создаёт все кнопки
static void CreateButtons(Frame *frame);
/// Создаёт одну кнопку
static void CreateButton(KeyEvent::E key, Frame *frame, const wxPoint &pos, const wxSize &size);


class Screen : public wxPanel
{
public:
    Screen(wxWindow *parent) : wxPanel(parent, 320)
    {
        SetMinSize({ 320, 240 });
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
    Text::SetUpperCase(true);

    Font::SetType(Font::Type::_8);

    CreateFrame();
}


void Painter::BeginScene(Color color)
{
    memDC.SelectObject(bitmap);
    wxBrush brush({ 0, 0, 0 }, wxTRANSPARENT);
    memDC.SetBrush(brush);
    FillRegion(0, 0, 320, 240, color);
}


void Painter::EndScene()
{
    memDC.SelectObject(wxNullBitmap);
    screen->Refresh();
}


static void SetSizeAndPosition(Frame *frame)
{
    wxSize size = { 680, 307 };
    
    frame->SetSize(size);
    frame->SetMinSize(size);
    frame->SetMaxSize(size);

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
    static const KeyEvent::E keys[5][5] =
    {
        {KeyEvent::F1,   KeyEvent::_1,      KeyEvent::_2,        KeyEvent::_3,       KeyEvent::Esc},
        {KeyEvent::F2,   KeyEvent::_4,      KeyEvent::_5,        KeyEvent::_6,       KeyEvent::Left},
        {KeyEvent::F3,   KeyEvent::_7,      KeyEvent::_8,        KeyEvent::_9,       KeyEvent::Right},
        {KeyEvent::F4,   KeyEvent::Dot,     KeyEvent::_0,        KeyEvent::Minus,    KeyEvent::On1},
        {KeyEvent::None, KeyEvent::RegLeft, KeyEvent::RegButton, KeyEvent::RegRight, KeyEvent::On2}
    };

    int x0 = 340;
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
}


static void CreateButton(KeyEvent::E key, Frame *frame, const wxPoint &pos, const wxSize &size)
{
    if (key == KeyEvent::None)
    {
        return;
    }

    wxButton *button = new wxButton(frame, static_cast<wxWindowID>(key), KeyEvent(key).Name(), pos, size);

    button->Connect(static_cast<wxWindowID>(key), wxEVT_LEFT_DOWN, wxCommandEventHandler(Frame::OnKeyDown));
    button->Connect(static_cast<wxWindowID>(key), wxEVT_LEFT_UP, wxCommandEventHandler(Frame::OnKeyUp));

    buttons[key] = button;
}


void Painter::FillRegion(int x, int y, int width, int height, Color color /* = Color::NUMBER */)
{
    SetColor(color);

    wxBrush brush = memDC.GetBrush();
    wxPen pen = memDC.GetPen();
    memDC.SetBrush(wxBrush(pen.GetColour()));
    memDC.DrawRectangle({ x, y, width + 1, height + 1 });
    memDC.SetBrush(brush);
}


void Painter::SetPoint(int x, int y)
{
    memDC.DrawPoint({ x, y });
}


void Painter::SetColor(Color _color)
{
    if (_color != Color::NUMBER)
    {
        uint color = COLOR(_color.value);
        uint8 b = static_cast<uint8>(color);
        uint8 g = static_cast<uint8>(color >> 8);
        uint8 r = static_cast<uint8>(color >> 16);

        wxColour colorDraw = wxColour(r, g, b);

        memDC.SetPen(wxPen(colorDraw));
    }
}


void Painter::DrawRectangle(int x, int y, int width, int height, Color color /* = Color::NUMBER */)
{
    SetColor(color);
    memDC.DrawRectangle({ x, y, width + 1, height + 1 });
}


void Painter::DrawVLine(int x, int y0, int y1, Color color /* = Color::NUMBER */)
{
    SetColor(color);
    memDC.DrawLine({ x, y0 }, { x, y1 });
}


void Painter::DrawHLine(int y, int x0, int x1, Color color /* = Color::NUMBER */)
{
    SetColor(color);
    memDC.DrawLine({ x0, y }, { x1, y });
}


void Painter::DrawLine(int x0, int y0, int x1, int y1, Color color)
{
    SetColor(color);
    memDC.DrawLine({ x0, y0 }, { x1, y1 });
}


void Painter::DrawFilledRectangle(int x, int y, int width, int height, Color colorFill, Color colorRect)
{
    DrawRectangle(x, y, width, height, colorRect);
    FillRegion(x + 1, y + 1, width - 2, height - 2, colorFill);
}


void Painter::LoadPalette()
{

}

#include "Hardware/Keyboard_win.h"
