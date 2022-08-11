// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "log.h"
#include "File.h"
#include "common/Interface_p.h"
#include "common/Command.h"
#include "Display/Painter.h"
#include "Signals/Signals.h"
#include "Settings/Settings.h"
#include <cstring>


using namespace Primitives;


static struct Picture
{
    uint8 data[300];
    void Clear()
    {
        std::memset(data, 0, 300);
    }
} picture;



// Порядковый номер файла в текущей папке
static int num = -1;


File::File()
{
    picture.Clear();
}


File::~File()
{
    Close();
}


static void DrawSignal(int x, int y, const uint8 *data, float scale)
{
    for (int i = 1; i < 240; i++)
    {
        int x1 = x + i - 1;
        int y1 = (int)((float)y - (float)data[i - 1] * scale);

        int x2 = x + i;
        int y2 = (int)((float)y - (float)data[i] * scale);

        Line().Draw(x1, y1, x2, y2);
    }
}


void File::Draw(int x, int y)
{
    Color::GRAY_10.SetAsCurrent();

    float scale = 100.0F / 255.0F;

    Color::GRAY_25.SetAsCurrent();

    DrawSignal(x, y, &Form::GetFormFlash(CURRENT_CHANNEL)[0], scale);

    if (num == -1)
    {
        return;
    }

    Color::FILL.SetAsCurrent();

    DrawSignal(x, y, &picture.data[0], scale);
}


bool File::Handler(SimpleMessage *msg)
{
    uint8 com = msg->TakeUINT8();

    if (com == Command::FDrive_RequestPictureDDSfromFile)
    {
        num = msg->TakeUINT8();

        msg->TakeRemainigData(picture.data);

        return true;
    }

    return false;
}


void File::SetDataToWave()
{
    Form::SetFormFlash(CURRENT_CHANNEL, picture.data);
}


static bool EqualsRequestPicture(Task *request, Task *answer)
{
    SimpleMessage *send = request->GetMessage();
    SimpleMessage *recv = answer->GetMessage();

    uint8 com = Command::FDrive_RequestPictureDDSfromFile;

    return  (com == send->TakeUINT8()) &&
            (com == recv->TakeUINT8());
}


void File::RequestFromPicture(int numberFile)
{
    Close();

    Message::FDrive::RequestPictureDDSfromFile message((uint8)numberFile);

    Task *task = new Task(&message, File::Handler, EqualsRequestPicture);

    PInterface::AddTask(task);
}


void File::Close()
{
    num = -1;

    picture.Clear();
}
