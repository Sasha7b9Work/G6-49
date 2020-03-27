#include "defines.h"
#include "File.h"
#include "common/Command.h"
#include "Interface/Interface_p.h"
#include "Utils/String.h"
#include <cstring>


static struct Picture
{
    uint8 data[300];
    void Clear()
    {
        std::memset(data, 0, 300);
    }
} picture;



/// Порядковый номер файла в текущей папке
//static int num = -1;


File::File()
{
    picture.Clear();
}


File::~File()
{
    Close();
}


//static bool EqualsRequestPicture(Task *request, Task *answer)
//{
//    SimpleMessage *send = request->GetMessage();
//    SimpleMessage *recv = answer->GetMessage();
//
//    uint8 com = Command::FDrive_GetPictureDDS;
//
//    return  (com == send->TakeByte()) &&
//            (com == recv->TakeByte());
//}


void File::Open(int /*n*/)
{
//    Close();
//
//    Message::FDrive::PictureDDS message(static_cast<uint8>(n));
//
//    Task *task = new Task(&message, File::Handler, EqualsRequestPicture);
//
//    PInterface::AddTask(task);
}


void File::Close()
{
//    num = -1;

    picture.Clear();
}
