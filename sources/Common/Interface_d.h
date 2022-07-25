// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Command.h"
#include "common/Messages.h"
#include "Utils/Queue.h"


struct DInterface
{
    static void Update();
    
    // ���������� ������ �� ������� ���������, ��������� ��������
    static Queue &GetOutbox();

    static void ResetFreqForSend();
    
    // ��������� ��������� � ������� ��������. ���� ������� ���������, ��������� ��������� � ������� ���������� false
    static bool AddMessageForTransmit(SimpleMessage *message);
private:
    
    // ������� ���������, ��������� ��������
    static Queue outbox;
    
    // ��������� �������� ��������, ��� ��� ������� �������� � ������ ��� ���������� ������������ ��������
    static uint freqForSend;
};
