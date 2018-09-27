#include "CPU.h"
#include "Display/Painter.h"
#include "Display/Colors.h"
#include "Display/Text.h"
#include "Hardware/Timer.h"
#include "Utils/StringUtils.h"
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Время последнего нажатия кнопки
static uint timeLastPress = 0;
static int pointer = 0;
static Control commands[10];
static void(*callbackKeyboard)() = 0;
static TIM_HandleTypeDef handleTIM4;
#define TIME_UPDATE 2
static uint8 TS_flag = 0;
TS_StateTypeDef TS_state = { 0 };

#define WIDTH_BUTTON  88
#define HEIGHT_BUTTON 93
#define DELTA_BUTTON  5
static int x0 = 642;
static int y0 = 0;

struct StructButton
{
    const char *title;
    Control control;
    int x;
    int y;
    uint timePress;     // Время нажатия кнопки. Нужно, чтобы определить наступления события Long. Если оно не равно нулю, то кнопка находится в 
                        // нажатом состоянии
};

static StructButton *btnSelected = 0;

#define BUTTON strBtn[i][j]


static StructButton strBtn[6][4] =
{
    { {"F1",  Control::F1},  {"1",        Control::_1},        {"2",    Control::_2},          {"3",         Control::_3} },
    { {"F2",  Control::F2},  {"4",        Control::_4},        {"5",    Control::_5},          {"6",         Control::_6} },
    { {"F3",  Control::F3},  {"7",        Control::_7},        {"8",    Control::_8},          {"9",         Control::_9} },
    { {"F4",  Control::F4},  {".",        Control::Dot},       {"0",    Control::_0},          {"-",         Control::Minus} },
    { {"ON1", Control::On1}, {"REG LEFT", Control::Reg::Left}, {"BTN",  Control::Reg::Button}, {"REG RIGHT", Control::Reg::Right} },
    { {"ON2", Control::On2}, {"ESC",      Control::Esc},       {"LEFT", Control::Left},        {"RIGHT",     Control::Right} }
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CPU::Keyboard::Init()
{
    SetCallback(&Update);

    HAL_NVIC_SetPriority(TIM4_IRQn, 0, 1);

    HAL_NVIC_EnableIRQ(TIM4_IRQn);

    handleTIM4.Instance = TIM4;
    handleTIM4.Init.Period = TIME_UPDATE * 10 - 1;
    handleTIM4.Init.Prescaler = (uint)((SystemCoreClock / 2) / 10000) - 1;
    handleTIM4.Init.ClockDivision = 0;
    handleTIM4.Init.CounterMode = TIM_COUNTERMODE_UP;

    HAL_TIM_Base_Init(&handleTIM4);

    HAL_TIM_Base_Start_IT(&handleTIM4);

    TOUCH::Init();

    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            BUTTON.x = x0 + j * (WIDTH_BUTTON + DELTA_BUTTON);
            BUTTON.y = y0 + i * (HEIGHT_BUTTON + DELTA_BUTTON);
        }
    }

    for (int i = 4; i < 6; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            BUTTON.y += DELTA_BUTTON * 2;
        }
    }

    for (int i = 0; i < 6; i++)
    {
        for (int j = 1; j < 4; j++)
        {
            BUTTON.x += DELTA_BUTTON * 2;
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool CPU::Keyboard::BufferIsEmpty()
{
    return pointer == 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
Control CPU::Keyboard::GetNextControl()
{
    Control retValue;

    if (BufferIsEmpty())
    {
        retValue = Control::None;
    }
    else
    {
        retValue = commands[0];
        for (int i = 1; i < pointer; i++)
        {
            commands[i - 1] = commands[i];
        }
        --pointer;
    }

    return retValue;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawButton(StructButton *button)
{
    if (button->timePress)
    {
        Painter::FillRegion(button->x, button->y, WIDTH_BUTTON, HEIGHT_BUTTON, Color::FILL);
        Painter::SetDoubleSize(true);
        Text::DrawStringInCenterRect(button->x / 2, button->y / 2, WIDTH_BUTTON / 2, HEIGHT_BUTTON / 2, button->title, Color::BACK);
        Painter::SetDoubleSize(false);
    }
    else
    {
        Painter::DrawRectangle(button->x, button->y, WIDTH_BUTTON, HEIGHT_BUTTON, Color::FILL);
        Painter::SetDoubleSize(true);
        Text::DrawStringInCenterRect(button->x / 2, button->y / 2, WIDTH_BUTTON / 2, HEIGHT_BUTTON / 2, button->title);
        Painter::SetDoubleSize(false);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CPU::Keyboard::Draw()
{
    Painter::SetDoubleSize(false);

    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            DrawButton(&BUTTON);
        }
    }

    Painter::SetDoubleSize(true);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static bool InButton(int x, int y, StructButton *button)
{
    return x > button->x && x < (button->x + WIDTH_BUTTON) && y > button->y && y < (button->y + HEIGHT_BUTTON);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CPU::Keyboard::Update()
{
    if (TIME_MS - timeLastPress < 20)
    {
        return;
    }

    if (TS_flag == 1)
    {
        TOUCH::GetState(&TS_state);
        if (TS_state.touchDetected != 0)
        {
            int x = TS_state.touchX[0];
            int y = TS_state.touchY[0];

            for (int i = 0; i < 6; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    if (InButton(x, y, &BUTTON))
                    {
                        if (btnSelected && btnSelected == &BUTTON && (TIME_MS - BUTTON.timePress > 500))
                        {
                            FillCommand(BUTTON.control, Control::Action::Long);
                            BUTTON.timePress = 0;
                            btnSelected = 0;
                        }
                        else
                        {
                            if (btnSelected == &BUTTON)
                            {
                                continue;
                            }
                            else if(btnSelected)
                            {
                                btnSelected->timePress = 0;
                                btnSelected = 0;
                            }

                            btnSelected = &BUTTON;
                            BUTTON.timePress = TIME_MS;
                            FillCommand(BUTTON.control, Control::Action::Down);
                        }
                    }
                }
            }
        }

        TS_flag = 0;
        timeLastPress = TIME_MS;
    }
    else if(btnSelected)
    {
        FillCommand(btnSelected->control, Control::Action::Up);
        btnSelected->timePress = 0;
        btnSelected = 0;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CPU::Keyboard::FillCommand(Control control, Control::Action action)
{
    commands[pointer] = control;
    commands[pointer++].action = action;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CPU::Keyboard::TOUCH::GetState(TS_StateTypeDef *TS_State)
{
    uint8_t RegBuf[34];

    /* Read touch message */
    I2C_ReadReg(GT811_CMD_WR, GT811_READ_XY_REG, RegBuf, sizeof(RegBuf));

    /* get touch massage */
    TS_State->SensorId = (uint)(RegBuf[0x00] >> 6);
    TS_State->touchDetected = (uint8)(RegBuf[0x00] & 0x1F);

    if (TS_State->touchDetected != 0)
    {
        //Touch point 1 coordinates
        TS_State->touchY[0] = (uint16)(GT811_MAX_HEIGHT - (((uint16_t)RegBuf[0x02] << 8) + RegBuf[0x03]));
        TS_State->touchX[0] = (uint16)((((uint16_t)RegBuf[0x04] << 8) + RegBuf[0x05]));
        TS_State->touchWeight[0] = RegBuf[0x06];
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CPU::Keyboard::SetCallback(void(*func)())
{
    callbackKeyboard = func;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CPU::Keyboard::TIM4_::ElapsedCallback(void *htim)
{
    if ((TIM_HandleTypeDef *)htim == &handleTIM4 && callbackKeyboard)
    {
        callbackKeyboard();
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CPU::Keyboard::TOUCH::Init()
{
    // Инициализируем I2C
    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;

    GPIO_InitStruct.Pin = SDA_GPIO_PIN;
    HAL_GPIO_Init(SDA_GPIO_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(SDA_GPIO_PORT, SDA_GPIO_PIN, GPIO_PIN_SET);

    GPIO_InitStruct.Pin = SCL_GPIO_PIN;
    HAL_GPIO_Init(SCL_GPIO_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(SCL_GPIO_PORT, SCL_GPIO_PIN, GPIO_PIN_SET);

    // Инициализируем GT811

    /*Configure GPIO pin : PtPin */
    GPIO_InitStruct.Pin = LCD_INT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* EXTI interrupt init*/
    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

    /* reset GT811 */
    HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_RESET);
    HAL_Delay(200);
    HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET);
    HAL_Delay(200);

    /* if Version is correct, send the configuration parameters */
    if (GT811_ReadID() == GT811_VERSION_VALUE)
    {
        /* touch screen configuration parameter (touch screen manufacturers provide) */
        uint8_t GTP_CFG_DATA[] =
        {
            0x12,0x10,0x0E,0x0C,0x0A,0x08,0x06,0x04,0x02,0x00,0x05,0x55,0x15,0x55,0x25,0x55,0x35,0x55,0x45,0x55,
            0x55,0x55,0x65,0x55,0x75,0x55,0x85,0x55,0x95,0x55,0xA5,0x55,0xB5,0x55,0xC5,0x55,0xD5,0x55,0xE5,0x55,
            0xF5,0x55,0x1B,0x03,0x00,0x00,0x00,0x13,0x13,0x13,0x0F,0x0F,0x0A,0x50,0x30,0x05,0x03,0x64,0x05,0xe0,
            0x01,0x20,0x03,0x00,0x00,0x32,0x2C,0x34,0x2E,0x00,0x00,0x04,0x14,0x22,0x04,0x00,0x00,0x00,0x00,0x00,
            0x20,0x14,0xEC,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0C,0x30,0x25,0x28,0x14,0x00,
            0x00,0x00,0x00,0x00,0x00,0x01,
        };

        /* config  */
        GTP_CFG_DATA[62] = GT811_MAX_WIDTH >> 8;
        GTP_CFG_DATA[61] = GT811_MAX_WIDTH & 0xff;
        GTP_CFG_DATA[60] = GT811_MAX_HEIGHT >> 8;
        GTP_CFG_DATA[59] = GT811_MAX_HEIGHT & 0xff;

        I2C_WriteReg(GT811_CMD_WR, GT811_CONFIG_REG, (uint8_t *)GTP_CFG_DATA, sizeof(GTP_CFG_DATA));
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
uint16 CPU::Keyboard::TOUCH::GT811_ReadID()
{
    uint8_t value[2];
    I2C_ReadReg(GT811_CMD_WR, GT811_VERSION, value, 2);
    return (uint16)(value[0] * 0x100 + value[1]);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
uint8 CPU::Keyboard::TOUCH::I2C_WriteReg(uint8 I2c_Addr, uint16 reg, uint8 *buf, uint8 len)
{
    uint8_t i;
    uint8_t ret = 0;
    I2C_Start();

    I2C_Send_Byte(I2c_Addr);   //Slaver Addr
    I2C_Wait_Ack();

#ifdef I2C_MEMADD_16BIT
    I2C_Send_Byte((uint8)(reg >> 8));   	     //Data Addr high
    I2C_Wait_Ack();
#endif

    I2C_Send_Byte((uint8)(reg & 0xFF));   	   //Data Addr low
    I2C_Wait_Ack();

    for (i = 0; i < len; i++)
    {
        I2C_Send_Byte(buf[i]);
        ret = I2C_Wait_Ack();
        if (ret)break;
    }
    I2C_Stop();
    return ret;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CPU::Keyboard::TOUCH::I2C_ReadReg(uint8 I2c_Addr, uint16 reg, uint8 *buf, uint8 len)
{
    uint8_t i;
    I2C_Start();
    I2C_Send_Byte(I2c_Addr);
    I2C_Wait_Ack();

#ifdef I2C_MEMADD_16BIT
    I2C_Send_Byte((uint8)(reg >> 8));   	     //Data Addr high
    I2C_Wait_Ack();
#endif

    I2C_Send_Byte((uint8)(reg & 0xFF));   	   //Data Addr low
    I2C_Wait_Ack();
    I2C_Stop();

    I2C_Start();
    I2C_Send_Byte((uint8)(I2c_Addr + 1));
    I2C_Wait_Ack();

    for (i = 0; i < len; i++)
    {
        buf[i] = I2C_Read_Byte((uint8)(i == (len - 1) ? 0 : 1));
    }
    I2C_Stop();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void SDA_OUT()
{
    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.Pin = SDA_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(SDA_GPIO_PORT, &GPIO_InitStruct);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void delay_us(uint32_t value)
{
    uint32_t i;
    i = value * 250;
    while (i--);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CPU::Keyboard::TOUCH::I2C_Start()
{
    SDA_OUT();

    SET_SDA();
    delay_us(1);

    SET_CLK();
    delay_us(1);

    CLR_SDA();
    delay_us(1);

    CLR_CLK();
    delay_us(1);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CPU::Keyboard::TOUCH::I2C_Send_Byte(uint8 txd)
{
    uint8_t t;

    SDA_OUT();
    CLR_CLK();       //Put low CLK to send data

    for (t = 0; t < 8; t++)
    {
        if (txd & 0x80)
            SET_SDA();
        else
            CLR_SDA();

        txd <<= 1;
        delay_us(1);

        SET_CLK();
        delay_us(1);
        CLR_CLK();
        delay_us(1);
    }

    SET_SDA();
    delay_us(1);
    CLR_CLK();
    delay_us(1);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void SDA_IN()
{
    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.Pin = SDA_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(SDA_GPIO_PORT, &GPIO_InitStruct);

}

//----------------------------------------------------------------------------------------------------------------------------------------------------
uint8 CPU::Keyboard::TOUCH::I2C_Wait_Ack()
{
    uint16_t ucErrTime = 0;

    SDA_IN();
    SET_SDA();
    delay_us(1);
    SET_CLK();
    delay_us(1);

    while (READ_SDA())
    {
        ucErrTime++;
        if (ucErrTime > 250)
        {
            I2C_Stop();
            return 1;
        }
    }
    CLR_CLK();
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CPU::Keyboard::TOUCH::I2C_Stop()
{
    SDA_OUT();

    CLR_SDA();
    delay_us(1);

    SET_CLK();
    delay_us(1);

    SET_SDA();
    delay_us(1);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void I2C_NAck(void)
{
    SDA_OUT();

    SET_SDA();
    delay_us(1);

    SET_CLK();
    delay_us(1);

    CLR_CLK();
    delay_us(1);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void I2C_Ack(void)
{
    SDA_OUT();
    CLR_CLK();

    CLR_SDA();
    delay_us(1);

    SET_CLK();
    delay_us(1);

    CLR_CLK();
    delay_us(1);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
uint8 CPU::Keyboard::TOUCH::I2C_Read_Byte(uint8 ack)
{
    unsigned char i, receive = 0;
    SDA_IN();         //SDA set in
    for (i = 0; i < 8; i++)
    {
        CLR_CLK();
        delay_us(1);
        SET_CLK();
        receive <<= 1;
        if (READ_SDA())receive++;
        delay_us(1);
    }
    CLR_CLK();
    if (!ack)I2C_NAck();   //sent nACK
    else I2C_Ack();        //sent ACK   

    return receive;
}


#ifdef __cplusplus
extern "C" {
#endif

    //------------------------------------------------------------------------------------------------------------------------------------------------
    void TIM4_IRQHandler()
    {
        if ((TIM4->SR & TIM_SR_UIF) == TIM_SR_UIF)
        {
            if ((TIM4->DIER & TIM_DIER_UIE) == TIM_DIER_UIE)
            {
                TIM4->SR = ~TIM_DIER_UIE;
                CPU::Keyboard::TIM4_::ElapsedCallback(&handleTIM4);
            }
        }
    }

    //------------------------------------------------------------------------------------------------------------------------------------------------
    void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
    {
        if (GPIO_Pin == GPIO_PIN_7)
        {
            TS_flag = 1;
        }
    }

#ifdef __cplusplus
}
#endif
