#include "defines.h"
#include "FatFS.h"



String FatFS::ErrorString(FRESULT result)
{
    static const cstr strings[] =
    {
        "FR_OK",
        "FR_DISK_ERR",
        "FR_INT_ERR",
        "FR_NOT_READY",
        "FR_NO_FILE",
        "FR_NO_PATH",
        "FR_INVALID_NAME",
        "FR_DENIED",
        "FR_EXIST",
        "FR_INVALID_OBJECT",
        "FR_WRITE_PROTECTED",
        "FR_INVALID_DRIVE",
        "FR_NOT_ENABLED",
        "FR_NO_FILESYSTEM",
        "FR_MKFS_ABORTED",
        "FR_TIMEOUT",
        "FR_LOCKED",
        "FR_NOT_ENOUGH_CORE",
        "FR_TOO_MANY_OPEN_FILES",
        "FR_INVALID_PARAMETER"
    };

    return String(strings[result]);
}

