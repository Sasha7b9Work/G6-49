#pragma once
#include "defines.h"
#include <cstdlib>
#include <string.h>

#define USBH_MAX_NUM_ENDPOINTS                2
#define USBH_MAX_NUM_INTERFACES               2
#define USBH_MAX_NUM_CONFIGURATION            1
#define USBH_MAX_NUM_SUPPORTED_CLASS          1
#define USBH_KEEP_CFG_DESCRIPTOR              0
#define USBH_MAX_SIZE_CONFIGURATION           0x200
#define USBH_MAX_DATA_BUFFER                  0x200
#define USBH_DEBUG_LEVEL                      0
#define USBH_USE_OS                           0

#ifdef STM32F407xx
#include "stm32f4xx.h"
#endif

#ifdef STM32F746xx
#include "stm32f7xx.h"
#endif
    
#if (USBH_USE_OS == 1)
  #include "cmsis_os.h"
  #define   USBH_PROCESS_PRIO    osPriorityNormal
#endif    

#define USBH_malloc               std::malloc
#define USBH_free                 std::free
#define USBH_memset               std::memset
#define USBH_memcpy               std::memcpy


#if (USBH_DEBUG_LEVEL > 0)
#define USBH_UsrLog(...)  Log_Write(__VA_ARGS__)
#define USBH_ErrLog(...)  Log_Write(__VA_ARGS__)
#define USBH_DbgLog(...)  Log_Write(__VA_ARGS__)
#else
#define USBH_UsrLog(...) {}
#define USBH_ErrLog(...) {}
#define USBH_DbgLog(...) {}
#endif
