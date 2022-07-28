// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Menu/MenuItems.h"


struct Menu
{
    // ������� ��������� ���� ���������� �� ������� ������������
    static const int NUM_ITEMS_ON_DISPLAY = 4;

    static void Init();
    
    static void Update();

    static void Draw();
    
    // ���������� ���������
    static int DrawTitle();

    static void DrawPagesUGO(int x);
    
    // ���������� true, ���� ����� ��������� �������������� ���������� ����
    static bool RegIsControlSubPages();
    
    // ���������� ����� ��������� �������� ����
    static Item* GetOpenedItem();
    
    // ���������� Item, ������� ����� ��������� ��������
    static void SetOpenedItem(Item *item);
    
    // ���������� ������� Item - ������ � ���� ����� ��������� ������� ����� � ESC
    static void SetCurrentItem(Item *item);
    
    // ����� �������� Item - �������� Item'a ������ ���
    static void ResetCurrentItem();
    
    // ���������� ������� Item
    static Item* GetCurrentItem();
    
    // �������� �������� ���� - ���� ������
    static void ResetOpenedItem();
    
    // ���������� ������� �������� �� ��������. ������ ���� �������� ����������� �������� ����. ����� -1
    static int GetPosition(const Page *page);
    
    // ������������� ��� ��������� ��������������� ��������
    static void SetAdditionPage(Page *page);
    
    // ������� �������������� �������� � ���������
    static void ResetAdditionPage();
  
    // ����� �������� ����� �������� ����, ���������������� �������������� ������� [F1..F4], ���� ��� ��������� � ������ ���������, � 0, ���� �� ���� ������ �� ������.
    static Item *pressedItem;

private:
    
    // ��������� �������� �������, ���� ���������� �������������� ������������� �������������������
    static bool OpenDebugPage(const Control);
    
    // ���������� ����������� ����������� control
    static void ProcessKey(const Control);
    
    // ������������ ��������� �������. ���������� true, ���� ��������� ������ ����������
    static bool ProcessOutputs(const Control);
    static void ProcessOutput(const Chan &);
    
    // ���� �����-�� ���� �������, �� ����� ��� �����
    static Item *openedItem;

    static Item *currentItem;
};
