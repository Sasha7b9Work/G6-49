#include "stdafx.h"
#ifndef WIN32
#include "EEPROM.h"
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#define ADDR_SECTOR_0   0x08000000  // 16k  Загрузчик
#define ADDR_SECTOR_1   0x08004000  // 16k
#define ADDR_SECTOR_2   0x08008000  // 16k
#define ADDR_SECTOR_3   0x0800c000  // 16k
#define ADDR_SECTOR_4   0x08010000  // 64k  SettingsCalibration
#define ADDR_SECTOR_5   0x08020000  // 128k Основная прошивка 1
#define ADDR_SECTOR_6   0x08040000  // 128k Основная прошивка 2
#define ADDR_SECTOR_7   0x08060000  // 128k Основная прошивка 3
#define ADDR_SECTOR_8   0x08080000  // 128k
#define ADDR_SECTOR_9   0x080a0000  // 128k
#define ADDR_SECTOR_10  0x080c0000  // 128k
#define ADDR_SECTOR_11  0x080e0000  // 128k


int tempI = 0;
