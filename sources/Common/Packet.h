#pragma once


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
    +------------------------------------------------+-------------+
    |             Служебное поле                     | Поле данных |
    +--------------+-----------------+---------------+-------------+
    | Номер пакета | Номер субпакета | Размер данных |   Данные    |
    +--------------+-----------------+---------------+-------------+
    |    0...3     |      4...7      |   8...11      | 12...63     |
    +--------------+-----------------+---------------+-------------+

    Данные могут занимать не всё поле для данных. В поле "Размер данных" рельное количество данных
*/

class Packet
{
public:
    Packet();
    /// Создаёт пакет из передаваемых данных
    void Create(uint8 *data, uint size);
    /// Указатель на начало пакета
    uint8 *Begin();
    /// Размер пакета
    uint Size() const;
    /// Возвращает true, если пакеты идентичны (равны с точностью до бита)
    bool IsEquals(Packet *rhs) const;

private:
    /// Размер пакета
    static const uint SIZE = 64;
    /// Размер поля контрольной суммы
    static const uint SIZE_FIELD_CRC = 4;
    /// Размер поля номера пакета
    static const uint SIZE_FIELD_NUMBER_PACKET = 4;
    /// Размер поля номера субпакета
    static const uint SIZE_FIELD_NUMBER_SUBPACKET = 3;
    /// Размер поля размера данных
    static const uint SIZE_FIELD_SIZE_DATA = 1;
    /// Размер служебного поля
    static const uint SIZE_SERVICE_FIELD = SIZE_FIELD_CRC + SIZE_FIELD_NUMBER_PACKET + SIZE_FIELD_NUMBER_SUBPACKET + SIZE_FIELD_SIZE_DATA;
    /// Размер поля данных
    static const uint MAX_SIZE_DATA_FIELD = SIZE - SIZE_SERVICE_FIELD;
    /// Возвращает указатель на начало поля данных
    uint8 *DataField();
    /// Возвращает указатель на байт, смещённый на offset от начала пакета
    uint8 *FromBegin(uint offset);
    /// Заполнить служебное поле значениями
    void FillServiceField(uint sizeData);
    /// Записать информацию в поле номера пакета
    void WriteNumberPacket();
    /// Записать информацию в поле номера субпакета
    void WriteNumberSubPacket(uint number);
    /// Записать информацию в поле размера данных
    void WriteSizeData(uint size);
    /// Записать контрольную сумму
    void WriteCRC();
    /// Количество переданных пакетов. Все пакеты нумеруются по порядку.
    static uint transcievedPackets;

    struct Data
    {
        uint numberPacket;
        uint numberSubPacket;
        uint size;
        uint8 data[SIZE - 3 * 4];
    } data;
};
