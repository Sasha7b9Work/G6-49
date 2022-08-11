// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "log.h"
#include "structs.h"
#include "common/Command.h"
#include "Display/Painter.h"
#include "Display/Text.h" 
#include "Display/WaveGraphics.h"
#include "FDrive/FDrive_p.h"
#include "FDrive/Items.h"
#include "FDrive/File.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Settings/Settings.h"
#include <cstdlib>
#include <cstdio>


using namespace Primitives;


namespace FDrive
{
    View view;

    SimpleMessage *Handler::msg = nullptr;

    enum Mount
    {
        Disconnect,
        Mounted,
        Failed
    };

    // Текущее состояние флешки
    static Mount mounted = Disconnect;

    // Путь к текущему каталогу
    static char directory[255];

    // Если true - идёт загрузка сигнала с флешки в память
    static bool inStateWaitCompleteLoad = false;
}


static bool E()
{
    return false;
}


void FDrive::Init()
{
    std::strcpy(directory, "\\");
    ListFiles::Init();
}


void FDrive::Draw()
{
    int x = WaveGraphics::X();
    int y = WaveGraphics::Y(ChA) + 1;
    int width = WaveGraphics::Width() - 2;
    int height = WaveGraphics::Height() * 2;

    Primitives::Rectangle(width, height).Fill(x, y, Color::BACK);

    if(mounted == Disconnect)
    {
        BigText("Подключите флешку", 2).Draw(30, 110, Color::FILL);
        return;
    }
    else if (mounted == Failed)
    {
        BigText("Флешка неисправна", 2).Draw(30, 110, Color::FILL);
        return;
    }

    if (ListFiles::NumberFiles() == -1)
    {
        ListFiles::SendRequest();
        return;
    }
    else if (ListFiles::WaitAnswer())
    {
        return;
    }

    ListFiles::Draw(x + 5, y + 5);

    if (inStateWaitCompleteLoad)
    {
        width = 128;
        height = 31;
        x = (Display::WIDTH - width) / 2;
        y = (Display::HEIGHT - height) / 2;

        Primitives::Rectangle(width, height).DrawFilled(x, y, Color::BACK, Color::FILL);

        String(LANG_RU ? "Сигнал загружается" : "Signal loaded").Draw(x + 10, y + 10, Color::FILL);
    }
}


bool FDrive::Handler::Processing(SimpleMessage *message)
{
    msg = message;

    msg->ResetPointer();

    uint8 com = msg->TakeUINT8();

    pFuncBV func = E;

    switch(com)
    {
    case Command::FDrive_NumDirsAndFiles: func = Handler::GetNumDirsAndFiles; break;
    case Command::FDrive_Mount:           func = Handler::IsMount;            break;
    case Command::FDrive_RequestFile:     func = Handler::RequestFile;        break;
    case Command::FDrive_RequestFileSize: func = Handler::RequestFileSize;    break;
    case Command::FDrive_LoadDDSfromFile: func = Handler::LoadDDSfromFile;    break;
    }

    return func();
}


bool FDrive::Handler::IsMount()
{
    mounted = (Mount)msg->TakeUINT8();
    Init();
    return true;
}


bool FDrive::Handler::GetNumDirsAndFiles()
{
    return ListFiles::Handler::Processing(msg);
}


bool FDrive::Handler::RequestFile()
{
    return ListFiles::Handler::Processing(msg);
}


bool FDrive::Handler::RequestFileSize()
{
    return ListFiles::Handler::Processing(msg);
}


bool FDrive::Handler::LoadDDSfromFile()
{
    inStateWaitCompleteLoad = false;

    return true;
}


void FDrive::PressUp()
{
    ListFiles::PressUp();
}


void FDrive::PressDown()
{
    ListFiles::PressDown();
}


void FDrive::PressChoose()
{
    inStateWaitCompleteLoad = true;
    
    Message::FDrive::LoadDDSfromFile((uint8)(CURRENT_CHANNEL), (uint8)ListFiles::NumberCurrentFile(), FDrive::CurrentDirectory()).Transmit();
    
    File::SetDataToWave();
}


char *FDrive::CurrentDirectory()
{
    return directory;
}


static void CreateFileName(char name[256])
{
    static int number = 0;

    std::snprintf(name, 255, "G649_%03d.bmp", number);

    number++;
}


static col_val cols[32] =
{
    MAKE_COLOR(0x00, 0x00, 0x00),   // BLACK
    MAKE_COLOR(0xff, 0xff, 0xff),   // WHITE
    MAKE_COLOR(25, 25, 25),         // GRAY_10
    MAKE_COLOR(0x30, 0x30, 0x30),   // GRAY_25
    MAKE_COLOR(0x80, 0x80, 0x80),   // GRAY_50
    MAKE_COLOR(0xC0, 0xC0, 0xC0),   // GRAY_75
    MAKE_COLOR(0x00, 0x00, 0xff),   // BLUE
    MAKE_COLOR(0x00, 0x00, 25),     // BLUE_10
    MAKE_COLOR(0x00, 0x00, 0x40),   // BLUE_25
    MAKE_COLOR(0x00, 0x00, 0x80),   // BLUE_50
    MAKE_COLOR(0x00, 0x00, 0xc0),   // BLUE_75
    MAKE_COLOR(0x00, 0xff, 0x00),   // GREEN
    MAKE_COLOR(0x00, 13, 0x00),     // GREEN_5
    MAKE_COLOR(0x00, 25, 0x00),     // GREEN_10
    MAKE_COLOR(0x00, 0x50, 0x00),   // GREEN_25
    MAKE_COLOR(0x00, 0x80, 0x00),   // GREEN_50
    MAKE_COLOR(0x00, 0xc0, 0x00),   // GREEN_75
    MAKE_COLOR(0xff, 0x00, 0x00),   // RED
    MAKE_COLOR(0x40, 0x00, 0x00),   // RED_25
    MAKE_COLOR(0x80, 0x00, 0x00),   // RED_50
    MAKE_COLOR(0xc0, 0x00, 0x00),   // RED_75
    MAKE_COLOR(0x40, 0xFF, 0x40),   // CHAN_A
    MAKE_COLOR(0xA0, 0xA0, 0xFF)    // CHAN_B
};

#define COL(x) cols[x]


void FDrive::SaveScreenToFlash()
{
    if (mounted != Mounted)
    {
        Display::Warnings::Show("Флешка не подключена", "Flash drive not connected", true);
        return;
    }

#pragma pack(1)
    struct BITMAPFILEHEADER
    {
        char    type0;      // 0
        char    type1;      // 1
        uint    size;       // 2
        uint16  res1;       // 6
        uint16  res2;       // 8
        uint    offBits;    // 10
    }
    bmFH =
    {
        0x42,
        0x4d,
        14 + 40 + 1024 + 320 * 237,
        0,
        0,
        14 + 40 + 1024
    };

    // 14

    struct BITMAPINFOHEADER
    {
        uint    size;           // 14
        int     width;          // 18
        int     height;         // 22
        uint16  planes;         // 26
        uint16  bitCount;       // 28
        uint    compression;    // 30
        uint    sizeImage;      // 34
        int     xPelsPerMeter;  // 38
        int     yPelsPerMeter;  // 42
        uint    clrUsed;        // 46
        uint    clrImportant;   // 50
        //uint    notUsed[15];
    }
    bmIH =
    {
        40, // size;
        320,// width;
        237,// height;
        1,  // planes;
        8,  // bitCount;
        0,  // compression;
        0,  // sizeImage;
        0,  // xPelsPerMeter;
        0,  // yPelsPerMeter;
        0,  // clrUsed;
        0   // clrImportant;
    };

    // 54
#pragma pack(4)

    char fileName[255];

    CreateFileName(fileName);

    Message::FDrive::CreateFFile(fileName).Transmit();

    Message::FDrive::WriteToFile(&bmFH, 14).Transmit();

    Message::FDrive::WriteToFile(&bmIH, 40).Transmit();

    uint8 buffer[320 * 3] = { 0 };

    struct STM32tagRGBQUAD
    {
        uint8    blue;
        uint8    green;
        uint8    red;
        uint8    rgbReserved;
    };

    STM32tagRGBQUAD colorStruct;

    for (int i = 0; i < 32; i++)
    {
        uint color = COL(i);
        colorStruct.blue = (uint8)((float)B_FROM_COLOR(color));
        colorStruct.green = (uint8)((float)G_FROM_COLOR(color));
        colorStruct.red = (uint8)((float)R_FROM_COLOR(color));
        colorStruct.rgbReserved = 0;
        (reinterpret_cast<STM32tagRGBQUAD *>(buffer))[i] = colorStruct;
    }

    for (int i = 0; i < 4; i++)
    {
        Message::FDrive::WriteToFile(buffer, 256).Transmit();
    }

    for (int row = 238; row >= 2; row--)
    {
        Message::FDrive::WriteToFile(Display::GetRow(row), 320).Transmit();
    }

    Message::FDrive::CloseFile().Transmit();

    Display::Warnings::Show("Снимок сохранён", "Snapshot saved", true);
}
