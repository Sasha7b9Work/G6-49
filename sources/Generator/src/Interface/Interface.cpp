#include "defines.h"
#include "Log.h"
#include "Interface.h"
#include "Generator/Generator.h"
#include "Hardware/CPU.h"
#include "Command.h"
#include <string.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static SPI_HandleTypeDef hSPI1 =                                   // Для связи с панелью
{
    SPI1,
    {
        SPI_MODE_SLAVE,                 // Init.Mode
        SPI_DIRECTION_2LINES,           // Init.Direction
        SPI_DATASIZE_8BIT,              // Init.DataSize
        SPI_POLARITY_HIGH,              // Init.CLKPolarity
        SPI_PHASE_2EDGE,                // Init.CLKPhase
        SPI_NSS_SOFT,                   // Init.NSS
        SPI_BAUDRATEPRESCALER_2,        // Init.BaudRatePrescaler
        SPI_FIRSTBIT_MSB,               // Init.FirstBit
        SPI_TIMODE_DISABLED,            // Init.TIMode
        SPI_CRCCALCULATION_DISABLED,    // Init.CRCCalculation
        7                               // InitCRCPolynomial
    },
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, HAL_UNLOCKED, HAL_SPI_STATE_RESET, 0
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static uint8 buffer[LENGTH_SPI_BUFFER];     ///< Буфер для принимаемых команд


                                            //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Interface::Init(void)
{
    GPIO_InitTypeDef isGPIOA =
    {   //  SCK         MI           MO
        GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7,
        GPIO_MODE_AF_PP,
        GPIO_NOPULL,
        GPIO_SPEED_HIGH,
        GPIO_AF5_SPI1
    };
    HAL_GPIO_Init(GPIOA, &isGPIOA);

    HAL_SPI_Init(&hSPI1);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::ProcessingCommand()
{
    HAL_StatusTypeDef res = HAL_SPI_Receive(&hSPI1, buffer, LENGTH_SPI_BUFFER, 10);
    
    if (res == HAL_OK)
    {
        if(buffer[0] != 0 || buffer[1] != 0 || buffer[2] != 0)
        {
            buffer[0] = buffer[0];
        }
        
        CPU::SetBusy();

        static uint8 prevBuffer[LENGTH_SPI_BUFFER] = {0};
        bool first = true;

        do
        {
            if (first)
            {
                first = false;
            }
            else
            {
                HAL_SPI_DeInit(&hSPI1);
                HAL_SPI_Init(&hSPI1);
            }
            memcpy(prevBuffer, buffer, LENGTH_SPI_BUFFER);
            res = HAL_SPI_TransmitReceive(&hSPI1, prevBuffer, buffer, LENGTH_SPI_BUFFER, 5);
        } while(memcmp(buffer, prevBuffer, LENGTH_SPI_BUFFER) != 0     // Пока не совпадут пва принятых буфера
                && res != HAL_TIMEOUT);             // Или не истечёт время ожидания ответа, что свидетельствует о том, что передатчик не передаёт
                                                    // данные, т.е. последние принятые данные правильные

        ProcessCommand();
        CPU::SetReady();
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::ProcessCommand()
{
    typedef void(*pFuncInterfaceVV)();

    static const pFuncInterfaceVV commands[NUM_COMMAND_WRITE] =
    {
        &Interface::CommandEmpty,       /// WRITE_SERVICE_COMMAND
        &Interface::CommandEnable,      /// ENABLE_CHANNEL
        &Interface::CommandFormWave,    /// SET_FORM_WAVE
        &Interface::CommandParameter,   /// SET_FREQUENCY
        &Interface::CommandParameter,   /// SET_AMPLITUDE
        &Interface::CommandParameter,   /// SET_OFFSET
        &Interface::CommandParameter,   /// SET_DURATION
        &Interface::CommandParameter,   /// SET_DUTYRATIO
        &Interface::CommandParameter,   /// SET_PHASE
        &Interface::CommandReset,       /// RUN_RESET
        &Interface::CommandModeDebug,   /// MODE_DEBUG
        &Interface::CommandParameter    /// SET_DELAY
    };
  
    if (buffer[0] < NUM_COMMAND_WRITE)
    {       
        pFuncInterfaceVV f = commands[buffer[0]];
        f();
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::CommandEnable(void)
{
    LOG_WRITE("Tест");

    Channel ch = (Channel)buffer[1];
    bool enable = buffer[2] == 1;
    
    __IO uint8 b1 = buffer[1];
    __IO uint8 b2 = buffer[2];

    if((b1 != 0 && b1 != 1) || (b2 != 0 && b2 != 1))
    {
        b1 = b1;
    }
    
    Generator::EnableChannel(ch, enable);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::CommandFormWave(void)
{
    Channel ch = (Channel)buffer[1];
    Type_WaveForm form = (Type_WaveForm)buffer[2];
    Generator::SetFormWave(ch, form);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::CommandParameter(void)
{
    Channel ch = (Channel)buffer[1];
    CommandWrite command = (CommandWrite)buffer[0];
    float value = 0.0f;
    memcpy(&value, &buffer[2], 4);
    Generator::SetParameter(ch, command, value);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::CommandReset()
{
#ifndef MSVC

#define MAIN_PROGRAM_START_ADDRESS  (uint)0x8000000
    typedef void(*pFunction)();
    __disable_irq();
    pFunction JumpToApplication = (pFunction)(*(__IO uint *)(MAIN_PROGRAM_START_ADDRESS + 4));
    __enable_irq();
    JumpToApplication();

#endif
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::CommandModeDebug(void)
{
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::CommandEmpty(void)
{
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
/*
static void SlaveSynchro(void)
{
    uint8 txByte = SPI_SLAVE_SYNBYTE;
    uint8 rxByte = 0x00;

    do
    {
        HAL_SPI_TransmitReceive(&hSPI1, &txByte, &rxByte, 1, HAL_MAX_DELAY);
    } while(rxByte != SPI_MASTER_SYNBYTE);
}
*/
