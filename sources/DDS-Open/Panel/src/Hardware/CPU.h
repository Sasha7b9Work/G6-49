#pragma once
#include "Hardware/Controls.h"
#include "Hardware/stm746.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define RTC_GET_PACKED_TIME() {}
#define RTC_SET_TIME_AND_DATA(d, m, y, h, min, s)

// Touch
#define SDA_GPIO_PIN        GPIO_PIN_13
#define SDA_GPIO_PORT       GPIOD
#define SCL_GPIO_PIN        GPIO_PIN_12
#define SCL_GPIO_PORT       GPIOD
#define LCD_RST_GPIO_Port   GPIOD
#define LCD_RST_Pin         GPIO_PIN_11
#define GT811_VERSION_VALUE 0x2010   /* GT811 firmware version */
/* Maximum border values of the touchscreen pad */
#define GT811_MAX_WIDTH     ((uint16_t)1024)    /* Touchscreen pad max width   */
#define GT811_MAX_HEIGHT    ((uint16_t)600)     /* Touchscreen pad max height  */
#define GT811_CMD_WR		0XBA	/* GT811 Write address  */
#define GT811_CONFIG_REG    0x6A2	  /* GT811 configuration start address  */
#define GT811_VERSION       0x717    /* GT811 version register addresses */
#define I2C_MEMADD_16BIT
#define SET_SDA()           HAL_GPIO_WritePin(SDA_GPIO_PORT,SDA_GPIO_PIN,GPIO_PIN_SET)  
#define SET_CLK()           HAL_GPIO_WritePin(SCL_GPIO_PORT,SCL_GPIO_PIN,GPIO_PIN_SET)
#define CLR_SDA()           HAL_GPIO_WritePin(SDA_GPIO_PORT,SDA_GPIO_PIN,GPIO_PIN_RESET)
#define CLR_CLK()           HAL_GPIO_WritePin(SCL_GPIO_PORT,SCL_GPIO_PIN,GPIO_PIN_RESET)
#define READ_SDA()          HAL_GPIO_ReadPin(SDA_GPIO_PORT,SDA_GPIO_PIN)
#define LCD_INT_Pin         GPIO_PIN_7
#define GT811_READ_XY_REG   0x721  	/* GT811 touch message start address  */


struct TS_StateTypeDef
{
    uint8  touchDetected;
    uint16 touchX[5];
    uint16 touchY[5];
    uint8  touchWeight[5];
    uint   SensorId;
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CPU : public STM746
{
public:

    static void Init();

    static void Update();

    static float GetFPS();

    //----------------------------------------------------------------------------------------------------------------------------------- Keyboard ---
    class Keyboard
    {
    friend class CPU;

    private:

        static void Init();

        static void FillCommand(Control control, Control::Action typePress);

    public:
        static void Draw();
        /// Установка каллбэка для клавиатуры
        static void SetCallback(void(*func)());
        /// Возвращает true, если буфер пуст
        static bool BufferIsEmpty();
        /// Возвращает следующий орган управления, если таковой имеется
        static Control GetNextControl();

        //----------------------------------------------------------------------------------------------------------------------------------- TIM4 ---
        class TIM4_
        {
        public:
            static void ElapsedCallback(void *htim);
        };

        //--------------------------------------------------------------------------------------------------------------------------------------------
        class TOUCH
        {
        public:
            static void Init();
            static void GetState(TS_StateTypeDef *state);
        private:
            static uint16 GT811_ReadID();
            static uint8 I2C_WriteReg(uint8 I2c_Addr, uint16 reg, uint8 *buf, uint8 len);
            static void I2C_ReadReg(uint8 I2c_Addr, uint16 reg, uint8 *buf, uint8 len);
            static void I2C_Start();
            static void I2C_Send_Byte(uint8 txd);
            static uint8 I2C_Wait_Ack();
            static void I2C_Stop();
            static uint8 I2C_Read_Byte(uint8 ack);
        };

    private:

        static void Update();
    };

    //-------------------------------------------------------------------------------------------------------------------------------------- GPIO ----
    class GPIO_
    {
    public:
        static void WritePin(char port, uint16 maskPin, bool state);

        static bool ReadPin(char port, uint16 maskPin);
    };

    //-------------------------------------------------------------------------------------------------------------------------------------- FLASH ---
    class FLASH_
    {
    public:
        static void SaveSettings();
        static void LoadSettings();
    };
    
    //---------------------------------------------------------------------------------------------------------------------------------------- FMC ---
    class FMC_
    {
    friend class CPU;
    private:
        static void Init();
    };
    
    //-------------------------------------------------------------------------------------------------------------------------------------- SDRAM ---
    class SDRAM_
    {
    friend class CPU;
    private:
        static void Init();
        static void InitializationSequence(uint count);
        static SDRAM_HandleTypeDef sdramHandle;
#define SDRAM_DEVICE_ADDR ((uint)0xD0000000)
    };

private:

    static void SystemClockConfig();
    /// Время начала замера ФПС (время начала очередного 1-секундного отрезка времени)
    static uint timeStartMeasFPS;
    /// Сколько фреймов насчитано с начала очередного 1-секундного отрезка времени
    static int numFrames;

    static float fps;
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif

    /// Прерывание на таймер. Timer.cpp
    void TIM3_IRQHandler();
    /// Прерывание на опрос клавиатуры. CPU_Keyboard.cpp
    void TIM4_IRQHandler();

    void NMI_Handler();
    void HardFault_Handler();
    void MemManage_Handler();
    void BusFault_Handler();
    void UsageFault_Handler();
    void SVC_Handler();
    void DebugMon_Handler();
    void PendSV_Handler();
    void SysTick_Handler();
    
#ifdef __cplusplus
}
#endif
