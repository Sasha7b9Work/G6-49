#include "defines.h"
#include "FDrive_dl.h"
#include "Generator/Generator_d.h"
#include "Hardware/HAL/HAL.h"
#include "Utils/String.h"
#include <usbh_diskio.h>


struct State
{
    enum E
    {
        Disconnected,   // ��������� �������� ����� ������
        NeedMount,      // ���������� ������������ ������, ��������� ������������
        Connected,      // ������ ������������ � ��������������
        NeedUnmount     // ��������� �������������
    } value;
};

struct StructForReadDir
{
    char nameDir[_MAX_LFN + 1];
    FILINFO fno;
    DIR dir;
};


static USBH_HandleTypeDef handle;

void *DLDrive::handleUSBH = &handle;

static FATFS FatFS;

static FIL file;            

static char USBDISKPath[4];

volatile static bool isConnected = false;       // true, ���� ������ ����������

static State::E state = State::Disconnected;


// �������� ���������� ��������� � ������ � ������ ����������
//static void GetNumDirsAndFiles(const char *fullPath, int *numDirs, int *numFiles);

//static void RequestFile(int num, char *fullPath);

//static void RequestFileSize(int num, char *path);

//static int GetFileSize(const char *fullPath);

// �������� ��� numFile-�� ����� �� �������� fullPath
//static bool GetNameFile(const char *fullPath, int numFile, char *nameFileOut);

// ������ �������� �������� ������� �� ����� name
//static bool ReadFloats(float values[4096], char *name);

// �������� ��� numDir-�� �������� �� �������� fullPath
//static bool GetNameDir(const char *fullPath, int numDir, char *nameDirOut, StructForReadDir *s);



static void USBH_UserProcess(USBH_HandleTypeDef *, uint8 id)
{
    switch(id)
    {
        case HOST_USER_SELECT_CONFIGURATION:
            break;

        case HOST_USER_CLASS_ACTIVE:
            state = State::NeedMount;
            break;

        case HOST_USER_CLASS_SELECTED:
            break;

        case HOST_USER_CONNECTION:
            break;

        case HOST_USER_DISCONNECTION:
            state = State::NeedUnmount;
            break;
    }
}


void DLDrive::Init()
{
    HAL_HCD::Init();

    if (FATFS_LinkDriver(&USBH_Driver, USBDISKPath) == FR_OK) //-V2001
    {
        USBH_Init(&handle, USBH_UserProcess, 0);
        USBH_RegisterClass(&handle, USBH_MSC_CLASS);
        USBH_Start(&handle);
    }
}

void DLDrive::DeInit()
{
    USBH_Stop(&handle);
    USBH_DeInit(&handle);
    FATFS_UnLinkDriver(USBDISKPath); //-V2001
    f_mount(0, "", 0);
}


void DLDrive::Update()
{
    USBH_Process(&handle);

    if(state == State::NeedMount)
    {
        f_mount(&FatFS, USBDISKPath, 0);

        state = State::Connected;
    }
    else if(state == State::NeedUnmount)
    {
        f_mount(0, "", 0);

        state = State::Disconnected;
    }
}


bool DLDrive::IsConnected()
{
    return (state == State::Connected);
}


int DLDrive::File::Open(const char *name)
{
    if(f_open(&file, name, FA_READ) == FR_OK)
    {
        return (int)f_size(&file);
    }

    return -1;
}


void DLDrive::File::Seek(int pos)
{
    f_lseek(&file, static_cast<FSIZE_t>(pos));
}


int DLDrive::File::Read(int size, uint8 *buffer)
{
    uint readed = 0;

    if(f_read(&file, buffer, static_cast<UINT>(size), &readed) == FR_OK)
    {
        return static_cast<int>(readed);
    }

    return -1;
}


void DLDrive::File::Close()
{
    f_close(&file);
}
