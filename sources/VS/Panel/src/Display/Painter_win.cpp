#include "defines.h"
#pragma warning(push)
#pragma warning(disable:4018 4076 4091 4189 4365 4459 4548 4571 4625 4668 5026 5027 5039)
#undef CRC

#include "Application.h"


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

#include <SDL.h>

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


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SDL_Renderer *renderer = nullptr;
static SDL_Window *window = nullptr;
static SDL_Texture *texture = nullptr;

static wxButton *buttons[KeyEvent::Count];

/// Здесь хранятся указатели на кнопки
//static wxButton *buttons[Key::Number] = { nullptr };

/// Цвета
static uint colors[256];


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Создаёт окно приложения. Возвращает хэндл виджета для отрисовки
static HANDLE CreateFrame();
/// Установить оптимальную позицию для окна приложения
static void SetSizeAndPosition(Frame *frame);
/// Получить разрешение максимального имеющегося в системе монитора
static wxRect GetMaxDisplay();
/// Создаёт все кнопки
static void CreateButtons(Frame *frame);
/// Создаёт одну кнопку
static void CreateButton(KeyEvent::E key, Frame *frame, const wxPoint &pos, const wxSize &size);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Display::Init()
{
    Text::SetUpperCase(true);

    Font::SetType(Font::Type::_8);

    HANDLE handle = CreateFrame();

    window = SDL_CreateWindowFrom(handle);

    if (window == nullptr)
    {
        std::cout << "SDL_CreateWindowFrom() Error: " << SDL_GetError() << std::endl;
    }
    else
    {
        std::cout << "Create SDL window is ok" << std::endl;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::BeginScene(Color color)
{
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_RENDERER_ACCELERATED, 320, 240);

    SDL_SetRenderTarget(renderer, texture);
    Painter::SetColor(color);
    SDL_RenderClear(renderer);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::EndScene()
{
    SDL_SetRenderTarget(renderer, NULL);

    SDL_Rect rect = {0, 0, 320, 240};

    SDL_RenderCopy(renderer, texture, NULL, &rect); //-V2001
    SDL_RenderPresent(renderer);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void SetSizeAndPosition(Frame *frame)
{
    wxSize size = { 680, 307 };
    
    frame->SetSize(size);
    frame->SetMinSize(size);
    frame->SetMaxSize(size);

    wxRect rect = GetMaxDisplay();

    frame->SetPosition({ rect.width / 2 - size.GetWidth() / 2, rect.height / 2 - size.GetHeight() / 2 });
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static HANDLE CreateFrame()
{
    Frame *frame = new Frame("");

    SetSizeAndPosition(frame);

    wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);

    wxButton *button = new wxButton(frame, wxID_ANY, "", {10, 10}, {320, 240});
    button->SetMaxSize({320, 240});

    sizer->Add(button);

    frame->SetSizer(sizer);

    CreateButtons(frame);

    frame->Show(true);

    return button->GetHandle();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void CreateButton(KeyEvent::E key, Frame *frame, const wxPoint &pos, const wxSize &size)
{
    if (key == KeyEvent::None)
    {
        return;
    }

    wxButton *button = new wxButton(frame, static_cast<wxWindowID>(key), KeyEvent(key).Name(), pos, size);

    button->Connect(static_cast<wxWindowID>(key), wxEVT_LEFT_DOWN, wxCommandEventHandler(Frame::OnDown));
    button->Connect(static_cast<wxWindowID>(key), wxEVT_LEFT_UP, wxCommandEventHandler(Frame::OnUp));

    buttons[key] = button;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::FillRegion(int x, int y, int width, int height, Color color /* = Color::NUMBER */)
{
    SetColor(color);
    SDL_Rect rect = { x, y, width + 1, height + 1 };
    SDL_RenderFillRect(renderer, &rect);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::SetPoint(int x, int y)
{
    SDL_RenderDrawPoint(renderer, x, y);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::SetColor(Color color)
{
    if (color != Color::NUMBER)
    {
        uint value = COLOR(color.value);
        uint8 blue = static_cast<uint8>(value);
        uint8 green = static_cast<uint8>(value >> 8);
        uint8 red = static_cast<uint8>(value >> 16);
        SDL_SetRenderDrawColor(renderer, red, green, blue, 0x00);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawRectangle(int x, int y, int width, int height, Color color /* = Color::NUMBER */)
{
    SetColor(color);
    SDL_Rect rect = { x, y, width + 1, height + 1 };
    SDL_RenderDrawRect(renderer, &rect);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawVLine(int x, int y0, int y1, Color color /* = Color::NUMBER */)
{
    SetColor(color);
    SDL_RenderDrawLine(renderer, x, y0, x, y1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawHLine(int y, int x0, int x1, Color color /* = Color::NUMBER */)
{
    SetColor(color);
    SDL_RenderDrawLine(renderer, x0, y, x1, y);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawLine(int x0, int y0, int x1, int y1, Color color)
{
    SetColor(color);
    SDL_RenderDrawLine(renderer, x0, y0, x1, y1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawFilledRectangle(int x, int y, int width, int height, Color colorFill, Color colorRect)
{
    DrawRectangle(x, y, width, height, colorRect);
    FillRegion(x + 1, y + 1, width - 2, height - 2, colorFill);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::LoadPalette()
{

}

#include "Hardware/CPU_Keyboard_win.h"
