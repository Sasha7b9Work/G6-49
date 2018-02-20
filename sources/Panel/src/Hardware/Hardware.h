#pragma once


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define ADDR_BANK           0x60000000

#define ADDR_CDISPLAY               ((uint8*)(ADDR_BANK + 0x080000))    ///< Здесь 8-битный низкоскоростной мультиплексный режим
#define ADDR_FPGA                   ((uint8*)(ADDR_BANK + 0x300000))    ///< Здесь 16-битный высокоскоростной немульиплексный режим
#define ADDR_RAM                    ((uint8*)(ADDR_BANK + 0x200000))    ///< Здесь 16-битный высокоскоростной немульиплексный режим

#define PRIORITY_SYS_TICK           0, 0
#define PRIORITY_TIMER_TIM6         2, 0
#define PRIORITY_PANEL_EXTI9_5      3, 0
#define PRIORITY_PANEL_SPI1         4, 0


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Hardware_Init();

void _Error_Handler(const char *, int);

#ifdef ERROR_HANDLER
#undef ERROR_HANDLER
#endif

#define ERROR_HANDLER _Error_Handler(__FILE__, __LINE__)
