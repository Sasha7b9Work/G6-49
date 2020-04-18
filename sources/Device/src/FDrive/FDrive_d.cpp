#include "defines.h"
#include "log.h"
#include "FDrive_d.h"
#include "Generator/Generator_d.h"
#include "Hardware/HAL/HAL.h"
#include "usbh_diskio.h"


static USBH_HandleTypeDef USB_host;
void *DDrive::handle = &USB_host;

static FATFS FatFS;

static char USBDISKPath[4];
// true, если флешка подключена
volatile static bool isConnected = false;

struct State
{
    enum E
    {
        Disconnected,   // Начальное значение после старта
        NeedMount,      // Обнаружена подключенная флешка, требуется монтирование
        Connected,      // Флешка подсоединена и примонтирована
        NeedUnmount     // Требуется отмонтировать
    } value;
};

static State::E state = State::Disconnected;
// Здесь хранится обрабатываемая команда
static Command command = Command::Count;



// В эту функцию попадаем при каждом событии на OTG FS
static void USBH_UserProcess(USBH_HandleTypeDef *, uint8 id);
// Получить имя numDir-го каталога из каталога fullPath
//static bool GetNameDir(const char *fullPath, int numDir, char *nameDirOut, StructForReadDir *s);



static void USBH_UserProcess(USBH_HandleTypeDef *, uint8 id)
{
    switch(id)
    {
        case HOST_USER_SELECT_CONFIGURATION:
            //LOG_WRITE("HOST_USER_SELECT_CONFIGURATION");
            break;

        case HOST_USER_CLASS_ACTIVE:
            //LOG_WRITE("HOST_USER_CLASS_ACTIVE");
            state = State::NeedMount;
            break;

        case HOST_USER_CLASS_SELECTED:
            //LOG_WRITE("HOST_USER_CLASS_SELECTED");
            break;

        case HOST_USER_CONNECTION:
            //LOG_WRITE("HOST_USER_CONNECTION");
            break;

        case HOST_USER_DISCONNECTION:
            //LOG_WRITE("HOST_USER_DISCONNECTION");
            state = State::NeedUnmount;
            break;

        default:
            break;
    }
}


void DDrive::Init()
{
    HAL_HCD::Init();

    if (FATFS_LinkDriver(&USBH_Driver, USBDISKPath) == FR_OK) //-V2001
    {
        USBH_StatusTypeDef res = USBH_Init(&USB_host, USBH_UserProcess, 0);
        res = USBH_RegisterClass(&USB_host, USBH_MSC_CLASS);
        res = USBH_Start(&USB_host);
    }
}


void DDrive::Update()
{
    USBH_Process(&USB_host);

    if(state == State::NeedMount)
    {
        FRESULT result = f_mount(&FatFS, USBDISKPath, 0);

        Message::FDrive::Mount((result == FR_OK) ? static_cast<uint8>(1) : static_cast<uint8>(2)).Transmit();

        state = State::Connected;

        if(result == FR_OK)
        {
            LOG_WRITE("флешка примонтирована");
        }
        else
        {
            LOG_WRITE("не удалось примонтировать флешку");
        }
    }
    else if(state == State::NeedUnmount)
    {
        f_mount(0, "", 0);

        Message::FDrive::Mount(0).Transmit();

        state = State::Disconnected;

        LOG_WRITE("флешка отмонтирована");
    }
}


/*
static bool GetNameDir(const char *fullPath, int numDir, char *nameDirOut, StructForReadDir *s)
{
    memcpy(s->nameDir, (void *)fullPath, strlen(fullPath));
    s->nameDir[strlen(fullPath)] = '\0';

    DIR *pDir = &s->dir;
    if (f_opendir(pDir, s->nameDir) == FR_OK)
    {
        int numDirs = 0;
        FILINFO *pFNO = &s->fno;
        bool alreadyNull = false;
        while (true)
        {
            if (f_readdir(pDir, pFNO) != FR_OK)
            {
                *nameDirOut = '\0';
                f_closedir(pDir);
                return false;
            }
            if (pFNO->fname[0] == 0)
            {
                if (alreadyNull)
                {
                    *nameDirOut = '\0';
                    f_closedir(pDir);
                    return false;
                }
                alreadyNull = true;
            }
            if (numDir == numDirs && (pFNO->fattrib & AM_DIR))
            {
                strcpy(nameDirOut, pFNO->fname);
                return true;
            }
            if ((pFNO->fattrib & AM_DIR) && (pFNO->fname[0] != '.'))
            {
                numDirs++;
            }
        }
    }
    return false;
}
*/
