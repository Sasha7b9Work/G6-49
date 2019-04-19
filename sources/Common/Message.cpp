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
SimpleMessage::SimpleMessage(uint8 v0, char *string) : allocated(0), buffer(0), used(0), taken(0)
{
    //          v0 | string              | завершающий_ноль
    uint size = 1 +  std::strlen(string) + 1;

    Create(size, v0);

    std::strcpy(String(1), string);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
SimpleMessage::SimpleMessage(uint8 v0, uint8 v1, char *string) : allocated(0), buffer(0), used(0), taken(0)
{
    Create(v0, v1, string);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
SimpleMessage::SimpleMessage(uint8 v0, uint8 v1, uint8 v2, char *string) : allocated(0), buffer(0), used(0), taken(0)
{
    Create(v0, v1, v2, string);
}

SimpleMessage::SimpleMessage(uint8 d0, uint d1, char *string) : allocated(0), buffer(0), used(0), taken(0)
{
    Create(d0, d1, string);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
SimpleMessage::SimpleMessage(uint size, uint8 v0) : allocated(0), buffer(0), used(0), taken(0)
{
    Create(size, v0);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
SimpleMessage::SimpleMessage(uint size, uint8 v0, uint v1, uint v2) : allocated(0), buffer(0), used(0), taken(0)
{
    Create(size, v0, v1, v2);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
SimpleMessage::SimpleMessage(uint size, uint8 com, uint8 d0, uint d1) : allocated(0), buffer(0), used(0), taken(0)
{
    Create(size, com, d0, d1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
SimpleMessage::SimpleMessage(uint size, uint8 v0, uint8 v1) : allocated(0), buffer(0), used(0), taken(0)
{   
    Create(size, v0, v1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
SimpleMessage::SimpleMessage(uint size, uint8 v0, uint8 v1, uint8 v2) : allocated(0), buffer(0), used(0), taken(0)
{
    Create(size, v0, v1, v2);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
SimpleMessage::SimpleMessage(uint size, uint8 v0, uint8 v1, float v2) : allocated(0), buffer(0), used(0), taken(0)
{
    Create(size, v0, v1, v2);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
SimpleMessage::SimpleMessage(uint size, uint8 v0, uint8 v1, uint64 v2) : allocated(0), buffer(0), used(0), taken(0)
{
    Create(size, v0, v1);

    BitSet64 bs(v2);

    for (int i = 0; i < 8; i++)
    {
        PutByte(bs.byte[i]);
    }
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
void SimpleMessage::Create(uint size, uint8 v0, uint v1, uint v2)
{
    if (AllocateMemory(size))
    {
        PutByte(v0); //-V525
        PutWord(v1);
        PutWord(v2);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SimpleMessage::Create(uint size, uint8 com, uint8 d0, uint d1)
{
    if (AllocateMemory(size))
    {
        PutByte(com);
        PutByte(d0);
        PutWord(d1);
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
void SimpleMessage::Create(uint8 v0, uint8 v1, char *string)
{
    //          v0 | v1 | string              | завершающий_ноль
    uint size = 1 +  1 +  std::strlen(string) + 1;

    if (AllocateMemory(size))
    {
        PutByte(v0);
        PutByte(v1);
        std::strcpy(String(2), string);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SimpleMessage::Create(uint8 v0, uint8 v1, uint8 v2, char *string)
{
    uint size = 1 + 1 + 1 + std::strlen(string) + 1;
    if (AllocateMemory(size))
    {
        PutByte(v0);
        PutByte(v1);
        PutByte(v2);
        std::strcpy(String(3), string);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SimpleMessage::Create(uint8 v0, uint v1, char *string)
{
    //          v0 | v1 | string              | завершающий_ноль
    uint size = 1 + 1 + std::strlen(string) + 1;

    if (AllocateMemory(size))
    {
        PutByte(v0);
        PutWord(v1);
        std::strcpy(String(2), string);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SimpleMessage::Create(uint size, uint8 v0, uint8 v1, uint8 v2)
{
    if (AllocateMemory(size))
    {
        PutByte(v0);
        PutByte(v1);
        PutByte(v2);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SimpleMessage::Create(uint size, uint8 v0, uint8 v1, float v2)
{
    if (AllocateMemory(size))
    {
        PutByte(v0);
        PutByte(v1);
        PutFloat(v2);
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
MessageFreqMeasure::MessageFreqMeasure(uint frequency) : SimpleMessage(5, Command::FreqMeasure)
{
    PutWord(frequency);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Message::RequestData::RequestData() : SimpleMessage(1, Command::RequestData)
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
MessageEnableChannel::MessageEnableChannel(uint8 ch, uint8 enable) : SimpleMessage(3, Command::EnableChannel)
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
MessageFDriveMount::MessageFDriveMount(uint8 mount) : SimpleMessage(2, Command::FDrive_Mount)
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
MessageFDriveFileSize::MessageFDriveFileSize(uint8 numFile, uint size) : SimpleMessage(6, Command::FDrive_RequestFileSize)
{
    PutByte(numFile);
    PutWord(size);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
MessageLog::MessageLog(char *string) : SimpleMessage()
{
    //          v0 | string              | завершающий_ноль
    uint size = 1 + std::strlen(string) + 1;

    AllocateMemory(size);
    PutByte(Command::Log);

    std::strcpy((char *)(buffer + 1), string);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
MessageStartMode::MessageStartMode(uint8 ch, uint8 mode) : SimpleMessage(3, Command::SetStartMode)
{
    PutByte(ch);
    PutByte(mode);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
MessageWriteRegister::MessageWriteRegister(uint8 reg, uint64 data) : SimpleMessage(10, Command::WriteRegister)
{
    PutByte(reg);
    PutDoubleWord(data);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
MessageDebugMode::MessageDebugMode(uint8 mode) : SimpleMessage(2, Command::ModeDebug)
{
    PutByte(mode);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
MessageReset::MessageReset() : SimpleMessage(1, Command::RunReset)
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
MessageParameter::MessageParameter(Command::E param, uint8 ch, uint64 value) : SimpleMessage(10, (uint8)param)
{
    PutByte(ch);
    PutDoubleWord(value);
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
