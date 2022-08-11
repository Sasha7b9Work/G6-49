// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
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


SimpleMessage &SimpleMessage::operator=(const SimpleMessage &rhs)
{
    FreeMemory();

    CreateAllocate(rhs.buffer, rhs.used);

    return *this;
}


SimpleMessage *SimpleMessage::Clone()
{
    SimpleMessage *result = new SimpleMessage();
    if (result->AllocateMemory(Size()))
    {
        std::memcpy(result->buffer, buffer, (uint)allocated);
        result->used = used;
        result->taken = taken;
    }
    
    return result;
}


void SimpleMessage::Create(int size, uint8 v0)
{
    if (AllocateMemory(size))
    {
        PutUINT8(v0);
    }
}


bool SimpleMessage::CreateAllocate(const uint8 *_buffer, int _size)
{
    if (AllocateMemory(_size))
    {
        std::memcpy(buffer, _buffer, (uint)_size); //-V595
        used = _size;
    }

    return buffer != 0;
}


bool SimpleMessage::CreateFromMessage(const SimpleMessage *message)
{
    if (AllocateMemory(message->Size()))
    {
        std::memcpy(buffer, message->buffer, (uint)message->allocated); //-V595
        used = message->used;
    }

    return buffer != 0;
}


void SimpleMessage::PutUINT8(uint8 data)
{
     buffer[used] = data;
     used += sizeof(data);
}


void SimpleMessage::PutUINT64(uint64 data)
{
    BitSet64 bs(data);
    PutUINT(bs.word0);
    PutUINT(bs.word1);
}


void SimpleMessage::PutINT16(int16 data)
{
    PutUINT16((uint16)data);
}


void SimpleMessage::PutUINT16(uint16 data)
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
    PutUINT((uint)data);
}


void SimpleMessage::PutData(const uint8 *data, int length)
{
    std::memcpy(buffer + used, data, (uint)length);
    used += length;
}


void SimpleMessage::PutFLOAT(float data)
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
    return (int16)TakeUINT16();
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
    return (int)TakeUINT();
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
    std::memcpy(data, buffer + taken, (uint)size);
    taken = allocated;
}


uint8 *SimpleMessage::RemainingData() const
{
    return &buffer[taken];
}


bool SimpleMessage::AllocateMemory(int size)
{
    FreeMemory();

    if (size == 0)
    {
        return false;
    }

    created++;
    createdSize += size;

    buffer = (uint8 *)std::malloc((uint)size);

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


uint8 *SimpleMessage::TakeData(int pos)
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

    return std::memcmp(message->buffer, buffer, (uint)allocated) == 0;
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
    PutUINT8(ch);
    PutUINT8(enable);
}


Message::SingleStart::SingleStart() : SimpleMessage(1, Command::SingleStart) { }


Message::RegCPU::RegCPU(uint8 port, uint8 pin, uint8 state) : SimpleMessage(4, Command::PortCPU)
{
    PutUINT8(port);
    PutUINT8(pin);
    PutUINT8(state);
}


Message::Debug::Debug() : SimpleMessage(1, Command::Debug)
{

}


Message::SetFreqMeterTrigK::SetFreqMeterTrigK(int16 k) : SimpleMessage(3, Command::SetFreqMeterTrigK)
{
    PutINT16(k);
}


Message::Set::FormWave::FormWave(uint8 ch, uint8 form) : SimpleMessage(3, Command::SetFormWave)
{
    PutUINT8(ch);
    PutUINT8(form);
}


Message::Set::Amplitude::Amplitude(uint8 ch, uint64 amplitude) : SimpleMessage(10, Command::SetAmplitude)
{
    PutUINT8(ch);
    PutUINT64(amplitude);
}


Message::Set::Offset::Offset(uint8 ch, uint64 offset) : SimpleMessage(10, Command::SetOffset)
{
    PutUINT8(ch);
    PutUINT64(offset);
}


Message::FDrive::Mount::Mount(uint8 mount) : SimpleMessage(2, Command::FDrive_Mount)
{
    PutUINT8(mount);
}


Message::FDrive::NumDirsAndFiles::NumDirsAndFiles(uint numDirs, uint numFiles) : SimpleMessage(9, Command::FDrive_NumDirsAndFiles)
{
    PutUINT(numDirs);
    PutUINT(numFiles);
}


Message::FDrive::NumDirsAndFiles::NumDirsAndFiles(char *directory) : SimpleMessage()
{   //          name | string                   | завершающий ноль
    int size = 1 +    (int)std::strlen(directory) + 1;
    AllocateMemory(size);
    PutUINT8(Command::FDrive_NumDirsAndFiles);
    std::strcpy(reinterpret_cast<char *>(buffer + 1), directory);
}


Message::FDrive::FileName::FileName(uint8 numFile, char *name) : SimpleMessage()
{
    //        v0 | v1 | string |              завершающий_ноль
    int size = 1 +  1 +  (int)std::strlen(name) + 1;
    AllocateMemory(size);
    PutUINT8(Command::FDrive_RequestFile);
    PutUINT8(numFile);
    std::strcpy(reinterpret_cast<char *>(buffer + 2), name);
}


Message::FDrive::CreateFFile::CreateFFile(pchar name) : SimpleMessage()
{
    int size = 1 + (int)std::strlen(name) + 1;
    AllocateMemory(size);
    PutUINT8(Command::FDrive_CreateFile);
    std::strcpy(reinterpret_cast<char *>(buffer + 1), name);
}


Message::FDrive::WriteToFile::WriteToFile(void *data, int size) : SimpleMessage(size + 1 + 4, Command::FDrive_WriteToFile)
{
    PutINT(size);
    std::memcpy(buffer + 5, data, (uint)size);
}


Message::FDrive::CloseFile::CloseFile() : SimpleMessage(1, Command::FDrive_CloseFile)
{

}


Message::FDrive::FileString::FileString(uint numString, char *nameFile) : SimpleMessage()
{
    //          commmand  numString  nameFile                завершающий_ноль
    int size = 1 +       1 +        (int)std::strlen(nameFile) + 1;
    AllocateMemory(size);
    PutUINT8(Command::FDrive_RequestFileString);
    PutUINT8((uint8)numString);
    std::strcpy(reinterpret_cast<char *>(buffer + 2), nameFile);
}


Message::FDrive::FileSize::FileSize(uint8 numFile, uint size) : SimpleMessage(6, Command::FDrive_RequestFileSize)
{
    PutUINT8(numFile);
    PutUINT(size);
}


Message::Log::Log(char *string) : SimpleMessage()
{
    //          v0 | string              | завершающий_ноль
    int size = 1 + (int)std::strlen(string) + 1;

    AllocateMemory(size);
    PutUINT8(Command::Log);

    std::strcpy(reinterpret_cast<char *>(buffer + 1), string);
}


Message::StartMode::StartMode(uint8 ch, uint8 mode) : SimpleMessage(3, Command::SetStartMode)
{
    PutUINT8(ch);
    PutUINT8(mode);
}


Message::SetStartStopMode::SetStartStopMode(int mode) : SimpleMessage(2, Command::SetStartStopMode)
{
    PutUINT8((uint8)mode);
}


Message::SetClockImpulse::SetClockImpulse(int clock) : SimpleMessage(2, Command::SetClockImpulse)
{
    PutUINT8((uint8)clock);
}


Message::WriteRegister::WriteRegister(uint8 reg, uint64 data) : SimpleMessage(10, Command::WriteRegister)
{
    PutUINT8(reg);
    PutUINT64(data);
}


Message::DebugMode::DebugMode(uint8 mode) : SimpleMessage(2, Command::ModeDebug)
{
    PutUINT8(mode);
}


Message::Set::Param::Param(Command::E param, uint8 ch, uint64 value) : SimpleMessage(10, (uint8)param)
{
    PutUINT8(ch);
    PutUINT64(value);
}


Message::Set::Param::Param(Command::E param, uint8 ch, uint8 value) : SimpleMessage(3, (uint8)param)
{
    PutUINT8(ch);
    PutUINT8(value);
}


Message::FDrive::LoadDDSfromFile::LoadDDSfromFile(uint8 ch, uint8 numFile, char *directory) : SimpleMessage()
{
    //          com ch  numFile directory                 завершающий_ноль
    int size = 1 + 1 + 1 +      (int)std::strlen(directory) + 1; //-V522
    AllocateMemory(size);
    PutUINT8(Command::FDrive_LoadDDSfromFile);
    PutUINT8(ch);
    PutUINT8(numFile);
    std::strcpy(reinterpret_cast<char *>(&buffer[3]), directory);
}


Message::Storage::LoadDDS::LoadDDS() : SimpleMessage(1, Command::LoadDDSfromStorage)
{

}


Message::FDrive::RequestPictureDDSfromFile::RequestPictureDDSfromFile(uint8 numFile) : SimpleMessage(2, Command::FDrive_RequestPictureDDSfromFile)
{
    PutUINT8(numFile);
}


Message::FDrive::RequestPictureDDSfromFile::RequestPictureDDSfromFile(uint8 numFile, uint8 data[240]) : SimpleMessage(242, Command::FDrive_RequestPictureDDSfromFile)
{
    PutUINT8(numFile);
    PutData(data, 240);
}


Message::Storage::RequestPictureDDS::RequestPictureDDS(uint8 ch) : SimpleMessage(2, Command::RequestPictureDDSfromStorage)
{
    PutUINT8(ch);
}


Message::Storage::RequestPictureDDS::RequestPictureDDS(uint8 ch, uint8 data[240]) : SimpleMessage(242, Command::RequestPictureDDSfromStorage)
{
    PutUINT8(ch);
    PutData(data, 240);
}


Message::SCPI::Data::Data(uint8 *data, int length) : SimpleMessage()
{
    //          command sizeof(length)   command 
    int size = 1 +     4 +              length;

    AllocateMemory(size);
    PutUINT8(Command::SCPI_Data);
    PutINT(length);
    PutData(data, length);
}
