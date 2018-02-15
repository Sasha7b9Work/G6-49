#include "defines.h"
#include "Generator.h"
#include "Log.h"
#include "Menu/MenuItems.h"
#include "../../Common/Command.h"
#include <string.h>



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static SPI_HandleTypeDef hSPI4 =
{
    SPI4,
    {
        SPI_MODE_MASTER,
        SPI_DIRECTION_2LINES,
        SPI_DATASIZE_8BIT,
        SPI_POLARITY_HIGH,
        SPI_PHASE_2EDGE,
        SPI_NSS_SOFT,
        SPI_BAUDRATEPRESCALER_2,
        SPI_FIRSTBIT_MSB,
        SPI_TIMODE_DISABLED,
        SPI_CRCCALCULATION_DISABLED,
        7
    },
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, HAL_UNLOCKED, HAL_SPI_STATE_RESET, 0
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Возвращает true, если процессор генератора не может принимать команды
static bool InterfaceIsBusy();

static void SendToInterface(uint8 *buffer, int size);
/// Сдвигает буфер на один бит влево
static void ShiftToLeft(uint8 *buffer, int length);


//static void MasterSynchro();


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Generator_Init(void)
{
    __HAL_RCC_SPI4_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();

    GPIO_InitTypeDef isGPIO =
    {   //  CLK         MI           MO
        GPIO_PIN_2 | GPIO_PIN_5 | GPIO_PIN_6,
        GPIO_MODE_AF_PP,
        GPIO_NOPULL,
        GPIO_SPEED_HIGH,
        GPIO_AF5_SPI4
    };

    HAL_GPIO_Init(GPIOE, &isGPIO);

    HAL_SPI_Init(&hSPI4);

    // На этом пине будем читать занятость процессора генератора
    isGPIO.Pin = GPIO_PIN_4;
    isGPIO.Mode = GPIO_MODE_INPUT;
    isGPIO.Alternate = 0;
    HAL_GPIO_Init(GPIOE, &isGPIO);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
/*
static void MasterSynchro(void)
{
    uint8 txByte = SPI_MASTER_SYNBYTE;
    uint8 rxByte = 0x00;

    do
    {
        HAL_SPI_TransmitReceive(&hSPI4, &txByte, &rxByte, 1, HAL_MAX_DELAY);
    } while(rxByte != SPI_SLAVE_SYNBYTE);
}
*/

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator_EnableChannel(Channel ch, bool enable)
{
    uint8 buffer[3] = {ENABLE_CHANNEL, ch, (uint8)(enable ? 1 : 0)};
    SendToInterface(buffer, 3);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator_SetDebugMode(bool enable)
{
    uint8 buffer[2] = {MODE_DEBUG, (uint8)(enable ? 1 : 0)};
    SendToInterface(buffer, 2);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator_Reset(void)
{
    uint8 command = RUN_RESET;
    SendToInterface(&command, 1);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator_SetFormWave(Channel ch, WaveForm form)
{
    uint8 buffer[3] = {SET_FORM_WAVE, ch, form};
    SendToInterface(buffer, 3);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator_SetParameter(Channel ch, WaveParameter param, float value)
{
    static const CommandWrite commands[NumParameters] =
    {
        SET_FREQUENCY,
        COMMAND_NONE,
        SET_AMPLITUDE,
        SET_OFFSET,
        SET_DURATION,
        SET_DUTYRATIO,
        SET_PHASE,
        SET_DELAY
    };

    uint8 buffer[6] = {commands[param], ch};
    memcpy(&buffer[2], &value, 4);
    SendToInterface(buffer, 6);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void SendToInterface(uint8 *data, int size)
{
    while (InterfaceIsBusy())
    {
    };
    
    if (size > LENGTH_SPI_BUFFER)
    {
        LOG_WRITE("слишком маленький буфер");
    }
    else
    {
        static uint8 buffer[LENGTH_SPI_BUFFER];         // Это массив для передаваемых данных
        static uint8 recvBuffer[LENGTH_SPI_BUFFER];     // Это массив для принимаемых данных

        memset(buffer, 0, LENGTH_SPI_BUFFER);
        memcpy(buffer, data, (uint)size);
        
        __IO CommandWrite command = (CommandWrite)buffer[0];
        __IO Channel ch = (Channel)buffer[1];

        HAL_SPI_Transmit(&hSPI4, buffer, LENGTH_SPI_BUFFER, 10);                                // Первая передача

        do
        {
            memset(recvBuffer, 0, LENGTH_SPI_BUFFER);                                           // Очищаем приёмный буфер
            HAL_SPI_TransmitReceive(&hSPI4, buffer, recvBuffer, LENGTH_SPI_BUFFER, 5);
            ShiftToLeft(recvBuffer, LENGTH_SPI_BUFFER);
        }
        while (memcmp(buffer, recvBuffer, LENGTH_SPI_BUFFER) != 0);
        
        command = command;
        ch = ch;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static bool InterfaceIsBusy(void)
{
    return HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4) == GPIO_PIN_RESET;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void ShiftToLeft(uint8 *buffer, int length)
{
    for (int i = 0; i < length; i++)
    {
        buffer[i] = (uint8)(buffer[i] << 1);
        if (i < length - 1)
        {
            if (GetBit(buffer[i + 1], 7))
            {
                SetBit(buffer[i], 0);
            }
        }
    }
}
