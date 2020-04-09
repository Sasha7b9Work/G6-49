#include "defines.h"
#include "structs.h"
#include "common/Messages.h"
#include "Settings/CalibrationSettings.h"
#include "Utils/Debug.h"
#include <cstdlib>
#include <cstdio>
#include <cstring>


int SimpleMessage::created = 0;
int SimpleMessage::destroed = 0;
int SimpleMessage::createdSize = 0;
int SimpleMessage::destroedSize = 0;


SimpleMessage::SimpleMessage() : allocated(0), buffer(0), used(0), taken(0)
{

}


SimpleMessage::~SimpleMessage()
{
    FreeMemory();
}


SimpleMessage::SimpleMessage(int size, uint8 v0) : allocated(0), buffer(0), used(0), taken(0)
{
    Create(size, v0);
}


SimpleMessage *SimpleMessage::Clone()
{
    SimpleMessage *result = new SimpleMessage();
    if (result->AllocateMemory(Size()))
    {
        std::memcpy(result->buffer, buffer, static_cast<uint>(allocated));
        result->used = used;
        result->taken = taken;
    }
    
    return result;
}


void SimpleMessage::Create(int size, uint8 v0)
{
    if (AllocateMemory(size))
    {
        PutByte(v0);
    }
}


bool SimpleMessage::CreateAllocate(const uint8 *_buffer, int _size)
{
    if (AllocateMemory(_size))
    {
        std::memcpy(buffer, _buffer, static_cast<uint>(_size)); //-V595
        used = _size;
    }

    return buffer != 0;
}


bool SimpleMessage::CreateFromMessage(const SimpleMessage *message)
{
    if (AllocateMemory(message->Size()))
    {
        std::memcpy(buffer, message->buffer, static_cast<uint>(message->allocated)); //-V595
        used = message->used;
    }

    return buffer != 0;
}


void SimpleMessage::PutByte(uint8 data)
{
     buffer[used] = data;
     used += sizeof(data);
}


void SimpleMessage::PutDoubleWord(uint64 data)
{
    BitSet64 bs(data);
    PutUINT(bs.word0);
    PutUINT(bs.word1);
}


void SimpleMessage::PutHalfWord(int16 data)
{
    PutHalfWord(static_cast<uint16>(data));
}


void SimpleMessage::PutHalfWord(uint16 data)
{
    BitSet16 bs(data);
    bs.WriteToBuffer(buffer + used);
    used += sizeof(data);
}


void SimpleMessage::PutUINT(uint data)
{
     BitSet32 bs(data);
     bs.WriteToBuffer(buffer + used);
     used += sizeof(data);
}


void SimpleMessage::PutINT(int data)
{
    PutUINT(static_cast<uint>(data));
}


void SimpleMessage::PutData(const uint8 *data, int length)
{
    std::memcpy(buffer + used, data, static_cast<uint>(length));
    used += length;
}


void SimpleMessage::PutFloat(float data)
{
    BitSet32 bs(data);
    bs.WriteToBuffer(buffer + used);
    used += sizeof(data);
}


uint8 SimpleMessage::TakeUINT8()
{
    uint8 result = buffer[taken];
    taken += sizeof(result);

    return result;
}


uint16 SimpleMessage::TakeUINT16()
{
    BitSet16 bs(buffer + taken);
    uint16 result = bs.halfWord;
    taken += sizeof(result);

    return result;
}


int16 SimpleMessage::TakeINT16()
{
    return static_cast<int16>(TakeUINT16());
}


uint SimpleMessage::TakeUINT()
{
    BitSet32 bs(buffer + taken);
    uint result = bs.word;
    taken += sizeof(result);

    return result;
}


int SimpleMessage::TakeINT()
{
    return static_cast<int>(TakeUINT());
}


uint64 SimpleMessage::TakeUINT64()
{
    BitSet64 bs(buffer + taken);
    uint64 result = bs.dword;
    taken += sizeof(result);

    return result;
}


float SimpleMessage::TakeFLOAT()
{
    BitSet32 bs(buffer + taken);
    float result = bs.floatValue;
    taken += sizeof(result);

    return result;
}


void SimpleMessage::TakeRemainigData(uint8 *data)
{
    int size = allocated - taken;
    std::memcpy(data, buffer + taken, static_cast<uint>(size));
    taken = allocated;
}


uint8 *SimpleMessage::RemainingData() const
{
    return &buffer[taken];
}


bool SimpleMessage::AllocateMemory(int size)
{
    DEBUG_POINT_0;

    FreeMemory();

    DEBUG_POINT_0;

    if (size == 0)
    {
        return false;
    }

    DEBUG_POINT_0;

    created++;
    createdSize += size;

    buffer = static_cast<uint8 *>(std::malloc(static_cast<uint>(size)));

    DEBUG_POINT_0;

    if (buffer)
    {
        allocated = size;
    }

    return (buffer != 0);
}


void SimpleMessage::FreeMemory()
{
    if(allocated)
    {
        destroed++;
        destroedSize += allocated;
    }

    allocated = 0;
    used = 0;
    std::free(buffer);
    buffer = 0;
}


uint8 *SimpleMessage::Data(int pos)
{
    return buffer + pos;
}


int SimpleMessage::Size() const
{
    return allocated;
}


bool SimpleMessage::IsEmpty() const
{
    return buffer != 0;
}


bool SimpleMessage::IsEquals(const SimpleMessage *message) const
{
    if (Size() != message->Size())
    {
        return false;
    }

    return std::memcmp(message->buffer, buffer, static_cast<uint>(allocated)) == 0;
}


char *SimpleMessage::String(int pos)
{
    return reinterpret_cast<char *>(&buffer[pos]);
}


Message::FreqMeter::Measure::Measure(uint frequency) : SimpleMessage(5, Command::FreqMeasure)
{
    PutUINT(frequency);
}


Message::RequestData::RequestData() : SimpleMessage(1, Command::RequestData)
{
}


Message::EnableChannel::EnableChannel(uint8 ch, uint8 enable) : SimpleMessage(3, Command::EnableChannel)
{
    PutByte(ch);
    PutByte(enable);
}


Message::RegCPU::RegCPU(uint8 port, uint8 pin, uint8 state) : SimpleMessage(4, Command::PortCPU)
{
    PutByte(port);
    PutByte(pin);
    PutByte(state);
}


Message::Set::FormWave::FormWave(uint8 ch, uint8 form) : SimpleMessage(3, Command::SetFormWave)
{
    PutByte(ch);
    PutByte(form);
}


Message::Set::Amplitude::Amplitude(uint8 ch, uint64 amplitude) : SimpleMessage(10, Command::SetAmplitude)
{
    PutByte(ch);
    PutDoubleWord(amplitude);
}


Message::Set::Offset::Offset(uint8 ch, uint64 offset) : SimpleMessage(10, Command::SetOffset)
{
    PutByte(ch);
    PutDoubleWord(offset);
}


Message::FDrive::Mount::Mount(uint8 mount) : SimpleMessage(2, Command::FDrive_Mount)
{
    PutByte(mount);
}


Message::FDrive::NumDirsAndFiles::NumDirsAndFiles(uint numDirs, uint numFiles) : SimpleMessage(9, Command::FDrive_NumDirsAndFiles)
{
    PutUINT(numDirs);
    PutUINT(numFiles);
}


Message::FDrive::NumDirsAndFiles::NumDirsAndFiles(char *directory) : SimpleMessage()
{   //          name | string                   | завершающий ноль
    int size = 1 +    static_cast<int>(std::strlen(directory)) + 1;
    AllocateMemory(size);
    PutByte(Command::FDrive_NumDirsAndFiles);
    std::strcpy(reinterpret_cast<char *>(buffer + 1), directory);
}


Message::FDrive::FileName::FileName(uint8 numFile, char *name) : SimpleMessage()
{
    //          v0 | v1 | string |              завершающий_ноль
    int size = 1 +  1 +  static_cast<int>(std::strlen(name)) + 1;
    AllocateMemory(size);
    PutByte(Command::FDrive_RequestFile);
    PutByte(numFile);
    std::strcpy(reinterpret_cast<char *>(buffer + 2), name);
}


Message::FDrive::FileString::FileString(uint numString, char *nameFile) : SimpleMessage()
{
    //          commmand  numString  nameFile                завершающий_ноль
    int size = 1 +       1 +        static_cast<int>(std::strlen(nameFile)) + 1;
    AllocateMemory(size);
    PutByte(Command::FDrive_RequestFileString);
    PutByte(static_cast<uint8>(numString));
    std::strcpy(reinterpret_cast<char *>(buffer + 2), nameFile);
}


Message::FDrive::FileSize::FileSize(uint8 numFile, uint size) : SimpleMessage(6, Command::FDrive_RequestFileSize)
{
    PutByte(numFile);
    PutUINT(size);
}


Message::Log::Log(char *string) : SimpleMessage()
{
    //          v0 | string              | завершающий_ноль
    int size = 1 + static_cast<int>(std::strlen(string)) + 1;

    AllocateMemory(size);
    PutByte(Command::Log);

    std::strcpy(reinterpret_cast<char *>(buffer + 1), string);
}


Message::StartMode::StartMode(uint8 ch, uint8 mode) : SimpleMessage(3, Command::SetStartMode)
{
    PutByte(ch);
    PutByte(mode);
}


Message::WriteRegister::WriteRegister(uint8 reg, uint64 data) : SimpleMessage(10, Command::WriteRegister)
{
    PutByte(reg);
    PutDoubleWord(data);
}


Message::DebugMode::DebugMode(uint8 mode) : SimpleMessage(2, Command::ModeDebug)
{
    PutByte(mode);
}


Message::Set::Parameter::Parameter(Command::E param, uint8 ch, uint64 value) : SimpleMessage(10, (uint8)param)
{
    PutByte(ch);
    PutDoubleWord(value);
}


Message::Set::Parameter::Parameter(Command::E param, uint8 ch, uint8 value) : SimpleMessage(3, (uint8)param)
{
    PutByte(ch);
    PutByte(value);
}


Message::FDrive::LoadFromExtStorage::LoadFromExtStorage(uint8 ch, uint8 numFile, char *directory) : SimpleMessage()
{
    //          com ch  numFile directory                 завершающий_ноль
    int size = 1 + 1 + 1 +      static_cast<int>(std::strlen(directory)) + 1;
    AllocateMemory(size);
    PutByte(Command::FDrive_LoadFromExtStorage);
    PutByte(ch);
    PutByte(numFile);
    std::strcpy(reinterpret_cast<char *>(&buffer[3]), directory);
}


Message::FDrive::PictureDDS::PictureDDS(uint8 numFile, uint8 *data) : SimpleMessage(242, Command::FDrive_GetPictureDDS)
{
    PutByte(numFile);
    PutData(data, 240);
}


Message::FDrive::PictureDDS::PictureDDS(uint8 numFile) : SimpleMessage(2, Command::FDrive_GetPictureDDS)
{
    PutByte(numFile);
}

Message::SCPI::Data::Data(uint8 *data, int length) : SimpleMessage()
{
    //          command sizeof(length)   command 
    int size = 1 +     4 +              length;

    AllocateMemory(size);
    PutByte(Command::SCPI_Data);
    PutINT(length);
    PutData(data, length);
}
