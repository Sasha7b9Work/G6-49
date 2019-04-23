#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "structs.h"
#include "Command.h"
#include "Message.h"
#include "Hardware/CPU.h"
#include "log.h"
#include <cstring>
#include <cstdlib>
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SimpleMessage::SimpleMessage() : allocated(0), buffer(0), used(0), taken(0)
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
SimpleMessage::~SimpleMessage()
{
    FreeMemory();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
SimpleMessage::SimpleMessage(uint size, uint8 v0) : allocated(0), buffer(0), used(0), taken(0)
{
    Create(size, v0);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
SimpleMessage *SimpleMessage::Clone()
{
    SimpleMessage *result = new SimpleMessage();
    if (result->AllocateMemory(Size()))
    {
        std::memcpy(result->buffer, buffer, allocated);
        result->used = used;
        result->taken = taken;
    }
    
    return result;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SimpleMessage::Create(uint size, uint8 v0)
{
    if (AllocateMemory(size))
    {
        PutByte(v0);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SimpleMessage::Create(uint size, uint8 v0, uint8 v1)
{
    if (AllocateMemory(size))
    {
        PutByte(v0);
        PutByte(v1);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SimpleMessage::Create(uint size, uint8 value0, uint8 value1, uint8 value2, uint16 value3)
{
    if (AllocateMemory(size))
    {
        PutByte(value0);
        PutByte(value1);
        PutByte(value2);
        PutHalfWord(value3);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool SimpleMessage::CreateAllocate(uint8 *_buffer, uint _size)
{
    if (AllocateMemory(_size))
    {
        std::memcpy(buffer, _buffer, _size); //-V595
        used = _size;
    }

    return buffer != 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool SimpleMessage::CreateFromMessage(SimpleMessage *message)
{
    if (AllocateMemory(message->Size()))
    {
        std::memcpy(buffer, message->buffer, message->allocated); //-V595
        used = message->used;
    }

    return buffer != 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SimpleMessage::PutByte(uint8 data)
{
     buffer[used] = data;
     used += sizeof(data);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SimpleMessage::PutDoubleWord(uint64 data)
{
    BitSet64 bs(data);
    PutWord(bs.word0);
    PutWord(bs.word1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SimpleMessage::PutHalfWord(int16 data)
{
    PutHalfWord((uint16)data);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SimpleMessage::PutHalfWord(uint16 data)
{
    BitSet16 bs(data);
    bs.WriteToBuffer(buffer + used);
    used += sizeof(data);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SimpleMessage::PutWord(uint data)
{
     BitSet32 bs(data);
     bs.WriteToBuffer(buffer + used);
     used += sizeof(data);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SimpleMessage::PutData(uint8 *data, uint length)
{
    std::memcpy(buffer + used, data, length);
    used += length;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SimpleMessage::PutFloat(float data)
{
    BitSet32 bs(data);
    bs.WriteToBuffer(buffer + used);
    used += sizeof(data);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint8 SimpleMessage::TakeByte()
{
    uint8 result = buffer[taken];
    taken += sizeof(result);

    return result;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint16 SimpleMessage::TakeHalfWord()
{
    BitSet16 bs(buffer + taken);
    uint16 result = bs.halfWord;
    taken += sizeof(result);

    return result;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint SimpleMessage::TakeWord()
{
    BitSet32 bs(buffer + taken);
    uint result = bs.word;
    taken += sizeof(result);

    return result;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint64 SimpleMessage::TakeDoubleWord()
{
    BitSet64 bs(buffer + taken);
    uint64 result = bs.dword;
    taken += sizeof(result);

    return result;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float SimpleMessage::TakeFloat()
{
    BitSet32 bs(buffer + taken);
    float result = bs.floatValue;
    taken += sizeof(result);

    return result;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SimpleMessage::TakeRemainigData(uint8 *data)
{
    uint size = allocated - taken;
    std::memcpy(data, buffer, size);
    taken = allocated;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint8 *SimpleMessage::RemainingData() const
{
    return &buffer[taken];
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool SimpleMessage::AllocateMemory(uint size)
{
    FreeMemory();

    if (size == 0)
    {
        return false;
    }

    buffer = (uint8 *)std::malloc(size);
    if (buffer)
    {
        allocated = size;
    }
    else
    {
        LOG_ERROR_FINALIZE("Недостаточно памяти для %x байт", size);
    }

    return buffer != 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SimpleMessage::FreeMemory()
{
    allocated = 0;
    used = 0;
    std::free(buffer);
    buffer = 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint8 *SimpleMessage::Data(int pos)
{
    return buffer + pos;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint SimpleMessage::Size() const
{
    return allocated;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool SimpleMessage::IsEmpty() const
{
    return buffer != 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool SimpleMessage::IsEquals(const SimpleMessage *message) const
{
    if (Size() != message->Size())
    {
        return false;
    }

    return std::memcmp(message->buffer, buffer, allocated) == 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
char *SimpleMessage::String(int pos)
{
    return (char *)&buffer[pos];
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
pString SimpleMessage::Trace()
{
    if (Size() == 0)
    {
        return "";
    }

    static Command command(buffer[0]);
    return command.Trace(0);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Message::FreqMeter::Measure::Measure(uint frequency) : SimpleMessage(5, Command::FreqMeasure)
{
    PutWord(frequency);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Message::RequestData::RequestData() : SimpleMessage(1, Command::RequestData)
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Message::EnableChannel::EnableChannel(uint8 ch, uint8 enable) : SimpleMessage(3, Command::EnableChannel)
{
    PutByte(ch);
    PutByte(enable);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Message::Set::FormWave::FormWave(uint8 ch, uint8 form) : SimpleMessage(3, Command::SetFormWave)
{
    PutByte(ch);
    PutByte(form);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Message::Set::Frequency::Frequency(uint8 ch, uint64 frequency) : SimpleMessage(10, Command::SetFrequency)
{
    PutByte(ch);
    PutDoubleWord(frequency);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Message::Set::Amplitude::Amplitude(uint8 ch, uint64 amplitude) : SimpleMessage(10, Command::SetAmplitude)
{
    PutByte(ch);
    PutDoubleWord(amplitude);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Message::Set::Offset::Offset(uint8 ch, uint64 offset) : SimpleMessage(10, Command::SetOffset)
{
    PutByte(ch);
    PutDoubleWord(offset);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Message::FDrive::Mount::Mount(uint8 mount) : SimpleMessage(2, Command::FDrive_Mount)
{
    PutByte(mount);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Message::FDrive::NumDirsAndFiles::NumDirsAndFiles(uint numDirs, uint numFiles) : SimpleMessage(9, Command::FDrive_NumDirsAndFiles)
{
    PutWord(numDirs);
    PutWord(numFiles);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Message::FDrive::NumDirsAndFiles::NumDirsAndFiles(char *directory) : SimpleMessage()
{   //          name | string                   | завершающий ноль
    uint size = 1 +    std::strlen(directory) + 1;
    AllocateMemory(size);
    PutByte(Command::FDrive_NumDirsAndFiles);
    std::strcpy((char *)(buffer + 1), directory);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Message::FDrive::FileName::FileName(uint8 numFile, char *name) : SimpleMessage()
{
    //          v0 | v1 | string |              завершающий_ноль
    uint size = 1 +  1 +  std::strlen(name) + 1;
    AllocateMemory(size);
    PutByte(Command::FDrive_RequestFile);
    PutByte(numFile);
    std::strcpy((char *)(buffer + 2), name);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Message::FDrive::FileString::FileString(uint numString, char *nameFile) : SimpleMessage()
{
    //          commmand  numString  nameFile                завершающий_ноль
    uint size = 1 +       1 +        std::strlen(nameFile) + 1;
    AllocateMemory(size);
    PutByte(Command::FDrive_RequestFileString);
    PutByte((uint8)numString);
    std::strcpy((char *)(buffer + 2), nameFile);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Message::FDrive::FileSize::FileSize(uint8 numFile, uint size) : SimpleMessage(6, Command::FDrive_RequestFileSize)
{
    PutByte(numFile);
    PutWord(size);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Message::Log::Log(char *string) : SimpleMessage()
{
    //          v0 | string              | завершающий_ноль
    uint size = 1 + std::strlen(string) + 1;

    AllocateMemory(size);
    PutByte(Command::Log);

    std::strcpy((char *)(buffer + 1), string);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Message::StartMode::StartMode(uint8 ch, uint8 mode) : SimpleMessage(3, Command::SetStartMode)
{
    PutByte(ch);
    PutByte(mode);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Message::WriteRegister::WriteRegister(uint8 reg, uint64 data) : SimpleMessage(10, Command::WriteRegister)
{
    PutByte(reg);
    PutDoubleWord(data);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Message::DebugMode::DebugMode(uint8 mode) : SimpleMessage(2, Command::ModeDebug)
{
    PutByte(mode);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Message::Reset::Reset() : SimpleMessage(1, Command::RunReset)
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Message::Set::Parameter::Parameter(Command::E param, uint8 ch, uint64 value) : SimpleMessage(10, (uint8)param)
{
    PutByte(ch);
    PutDoubleWord(value);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Message::Set::Parameter::Parameter(Command::E param, uint8 ch, uint8 value) : SimpleMessage(3, (uint8)param)
{
    PutByte(ch);
    PutByte(value);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Message::FDrive::LoadToFPGA::LoadToFPGA(uint8 ch, uint8 numFile, char *directory) : SimpleMessage()
{
    //          com ch  numFile directory                 завершающий_ноль
    uint size = 1 + 1 + 1 +      std::strlen(directory) + 1;
    AllocateMemory(size);
    PutByte(Command::FDrive_LoadToFPGA);
    PutByte(ch);
    PutByte(numFile);
    std::strcpy((char *)&buffer[3], directory);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Message::LoadFromDDS::LoadFromDDS(uint8 ch, uint8 data[8 * 1024]) : SimpleMessage(8 * 1024, Command::LoadFormDDS)
{
    PutByte(ch);
    std::memcpy(buffer + 2, data, 8 * 1024);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Message::FDrive::PictureDDS::PictureDDS(uint8 numFile, uint8 *data) : SimpleMessage(242, Command::FDrive_GetPictureDDS)
{
    PutByte(numFile);
    std::memcpy(buffer + 2, data, 240);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Message::FDrive::PictureDDS::PictureDDS(uint8 numFile) : SimpleMessage(2, Command::FDrive_GetPictureDDS)
{
    PutByte(numFile);
}
