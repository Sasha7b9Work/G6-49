#pragma once


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class FLASH_
{
public:
    static void SaveSettings();
    static void LoadSettings();
    static void EraseSettings();
private:
    static bool EraseSector(uint startAdddress);
    static void WriteBufferBytes(uint address, void *buffer, int numBytes);
    static uint GetSector(uint startAddress);
    static void ReadBufferBytes(uint addrSrc, void *bufferDest, int size);
};
