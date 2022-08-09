// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
    #pragma once
#include "CommonTypes.h"


// Возможные команды для передачи в основной процессор
struct Command
{
    // Значок "->" означает, что передача ведётся от прибора к панели

    enum E
    {
          // 0
          // RequestData
/* <-> */    RequestData,                // Запрос данных. Панель постоянно опрашивает генератор на предмет наличия данных для передачи.
             EnableChannel,              // Включить/выключить канал
          // 0           1       2
          // SetFomrWave NumChan Form
             SetFormWave,                // Установить форму сигнала
             SetFrequency,               // Установить частоту
             SetAmplitude,               // Установить амплитуду
             SetOffset,                  // Установать смещение
             SetDuration,
             SetDutyRatio,
             SetPhase,
             ModeDebug,                  // Установка отладочного режиме - когда идут непрерывные засылки в FPGA
             SetDelay,                   // Задержка
          // 0             1            2...9
          // WriteRegister NameRegister Value64
             WriteRegister,              // Занести значение в регистр
             SetDurationRise,
             SetDurationFall,
             SetDurationStady,
             SetDutyFactor,
             SetManipulation,            // Установить режим манипуляции
             SetManipulationDuration,    // Установить длительность ипульса в режиме манипуляции
             SetManipulationPeriod,      // Установить период следования импульсов в режиме манипуляции
             SetPacketPeriod,
             SetPacketNumber,
          // 0            1        2
          // SetStartMode NUM_CHAN value
             SetStartMode,               // Установить режим запуска по каналу
             SetPeriod,                  // Установить период
          // 0           1        2
          // SetPolarity NUM_CHAN polarity
             SetPolarity,
          // 0            1       3...
          // LoadFormDDS, NumChan Points
             LoadFormDDS,               // Загружает точки сигнала произовольной формы
          // 0            1...5
          // FreqMeasure, измерение
/*  -> */    FreqMeasure,               // Пересылка показаний частотомера в панель
          //  0
          // Log
/*  -> */    Log,                       // Сообщение для вывода в лог
          // 0 . Далее следуют до 255 байт пути (учитывая завершающий ноль) пути до каталога, количество каталогов и файлов в котором необходимо подсчитать         
          // Ответ :
          // 0                       1...5   6...9
          //                         NumDirs NumFiles
/* <-> */    FDrive_NumDirsAndFiles,    // Запрос количества каталогов и файлов
          // 0             1
          //               data
/*  -> */    FDrive_Mount,              // Сообщение о примонтированной флешке. data == 0 - флешка отмонтирована. 1 - флешка примонтирована
          // 0                  1    ...
          //                    номер  путь_к_родительскому_каталогу
/* <-> */    FDrive_RequestDir,         // Запрос имени каталога
          // 0                  1
          //                    номер путь_к_родительскому_каталогу
/* <-> */   FDrive_RequestFile,         // Запрос имени файла
        //  0      1...5    6...9
        //  Test   size     crc32
/* <-> */   Test,                               // Команда для тестирования SPI
/* <-> */   FDrive_RequestFileSize,             // Запрос размера файла
/* <-> */   FDrive_RequestFileString,           // Считать строку из файла
/* <-> */   FDrive_LoadDDSfromFile,             // Загрузить сигнал с флешки. Получив это сообщение, устройство начинает загрузку сигнала, а по окончании возвращает это же сообщение
/* <-> */   FDrive_RequestPictureDDSfromFile,   // Получить изображение формы сигнала
         // 0           1...5     6.....
         // SCPI_Data   sizeData  data 
/* <-> */   SCPI_Data,                      // От VCP приняты данные либо в VCP нужно передать данные
            PortCPU,
            CalibrationLoad,
            CalibrationSet,

            // byte(com)
            StartApplication,               // Запуск основного приложения. Этой командой Device даёт команду Panel на старт
            // byte(com)
            RequestUpgrade,                 // Запрос на обновление
            // byte(com) | byte(0...100)
            PortionUpgradeDevice,           // Информация о записанной части прошивки Device
            // byte(com) | word(размер)
            AnswerUpgradePanel,             // Этой командой DLoader сообщает о том, что имеется обновление Panel
            // byte(com) | hword(num)
            RequestPortionUpgradePanel,     // Запрос на порцию прошивки Panel
            // byte(com) | hword(num) | word(crc32) | 512 bytes
            AnswerPortionUpgradePanel,      // Ответ на запрос порции прошивки Panel

            Debug,
            SetFreqMeterTrigK,              // Установка калибровочного коээфициента для уровня синхронизации частотомера
            RequestPictureDDSfromStorage,   // Запрос изображения сигнала, сохранённого в ППЗУ
            LoadDDSfromStorage,             // Загрузка сигнала из ППЗУ
            FDrive_CreateFile,
            FDrive_WriteToFile,
            FDrive_CloseFile,
            SingleStart,                    // Даёт однократный запускф
            SetStartStopMode,               // Режим старт/стоп для импульсного сигнала
            SetClockImpulse,                // Установка тактовой частоты формирователя импульсов
            Count
    } value;
    Command(E v) : value(v) {};
    Command(const uint8 v) : value(static_cast<E>(v)) {};
    operator uint8() const { return static_cast<uint8>(value); };
    cstr Name() const;
    operator E() const { return value; };
    operator uint8() { return static_cast<uint8>(value); }
private:
    cstr Value(uint8 *buffer) const;
};


struct Register
{
    enum E
    {
        deleted_Multiplexor1,   // \todo Это надо удалить
        deleted_Multiplexor2,
        OffsetA,
        OffsetB,
        FreqMeterLevel,
        FreqMeterHYS,
        FPGA_RG0_Control,           // Управляющий регистр
        FPGA_RG1_Freq,              // Регистр кода частоты
        FPGA_RG2_Amplitude,         // Регистр кода умножителя
        FPGA_RG3_RectA,             // Регистр кода сигнала прямоугольной формы канала А
        FPGA_RG4_RectB,             // Регистр кода сигнала прямоугольной формы канала B
        FPGA_RG5_PeriodImpulseA,    // Регистр кода периода прямоугольных импульсов канала А
        FPGA_RG6_DurationImpulseA,  // Регистр кода длительности прямоугльных импульсов канала А
        FPGA_RG7_PeriodImpulseB,    // Регистр кода периода прямоугольных импульсов канала B
        FPGA_RG8_DurationImpulseB,  // Регистр кода длительности прямоуольных импульсов канала B
        FPGA_RG9_FreqMeter,         // Регистр параметров частотомера
        FPGA_RG10_Offset,           // Регистр смещения
        FreqMeter_Resist,
        FreqMeter_Couple,
        FreqMeter_Filtr,
        Count
    } value;
    explicit Register(E v) : value(v) {};
    explicit Register(uint8 v) : value(static_cast<E>(v)) {};
    cstr Name() const;
    operator E() { return value; }
};


#define LENGTH_SPI_BUFFER   30

#define SPI_SLAVE_SYNBYTE   0x53

#define SPI_MASTER_SYNBYTE  0xAC

#define NAME_FORM(form, lang)  Form(form).Name(lang)
#define FORM_RU(form) NAME_FORM(form, 0)
#define FORM_EN(form) NAME_FORM(form, 1)
