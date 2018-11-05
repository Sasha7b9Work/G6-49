#pragma once
#include <ff.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class FDrive
{
public:

    static void Init();

    static void Update();
    /// Interface вызывает эту функцию для обработки принятой команды
    static void HandlerInterface();
    /// Возвращает количество байт для передачи
    static uint NumBytesForSend();
    /// Получает данные для передачи в панель. Параметром нужно передать указатель на буфер размером не менее возвращаемого функцией NumBytesForSend значения
    static uint8 *GetDataForSend(uint8 *buffer);

    static USBH_HandleTypeDef hUSB_Host;
};
