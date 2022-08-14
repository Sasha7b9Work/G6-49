// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Common.h"
#include "Signals/Parameters.h"
#include "Settings/SettingsTypes.h"
#include <cstring>
#include <limits>


class Wave;


// �� ����� ���������� ����� ������� ������������ ������
#define POINTS_IN_FORM 300


class Form
{
    friend class Wave;

public:
    TypeForm::E value;

    Form(TypeForm::E v = TypeForm::Count)
    {
        Init(v);
    };

    Form(uint8 v)
    {
        Init((TypeForm::E)v);
    };

    Form(TypeForm::E v, Param **param, Wave *w);

    void Init(TypeForm::E v)
    {
        value = v;
        wave = 0;
        params = nullptr;
        numParams = 0;
        currentParam = 0;
        old.Init(nullptr, 0, 0);
    };

    // ���������� ������������ �������� ����� �������
    cstr Name(uint lang = (uint)(-1)) const;
    
    // ���������� ������ �� ������� ��������
    Param *CurrentParameter() const;
    
    // ���������� ���������� ��������� ����������
    int NumParameters() const;
    
    // ���������� ������ �� i-�� �������� �� ������� params
    Param *GetParameter(int i) const;
    
    // ���������� ������� �������� ��������
    void SetNextParameter();

    // ���������� ������� ���������� ��������
    void SetPrevParameter();
    
    // ����������� ��������� � ������������ � �������������� �����������
    void TuneGenerator();
    
    // ���������� true, ���� ��� ����� ������� ������������� e
    bool Is(TypeForm::E e) const { return e == value; };
    
    // ���������� true, ���� ����� ������� ����������� � ������� ����
    bool IsDDS() const;
    
    // ���������� ��������� �� ������������ Wave
    Wave *GetWave() const { return wave; };
  
    // ���������� ����������� �������
    void DrawUGO(const Chan &, int y0) const;
    
    // ���������� ������ ������������� ������� ��� ���������
    static void SetFormFlash(const Chan &, const uint8 data[POINTS_IN_FORM]);
    
    // ���������� ��������� �� ������ ������������� ������� ��� ���������
    static uint8 *GetFormFlash(const Chan &);
    
    // �������� �������� �������
    double GetOffset() const;
    
    // �������� ������ �������
    double GetAmplitude() const;

    operator TypeForm::E() const { return value; }
    
    // ������� ��������� ��������. ���������� 0, ���� ������ ��������� ���
    DParam *FindParameter(TypeDParam::E p) const;
    CParam *FindParameter(TypeCParam::E p) const;
    IParam *FindParameter(TypeIParam::E p) const;
    CMSParam *FindParameter(TypeCMSParam::E p) const;
    
    // �������� �������� � ���������
    void SendParameterToGenerator(TypeDParam::E p) const;
    void SendParameterToGenerator(TypeCParam::E p) const;
    void SendParameterToGenerator(TypeIParam::E p) const;

    // ���������� ��������� ��������
    void OpenCompositeParameter();
    // ��������� �������� ��������, ���� ������� ������� � ���������� true � ���� ������
    bool CloseCompositeParameter();

    void Reset();

    // ��������� ���������
    void StoreState();

    // ������������ ����� ���������� ���������
    void RestoreState();

private:
    
    // Wave, � �������� ��������� ������ Form
    Wave *wave;
    
    // ����� �������� ���������
    Param **params;
    
    // ������� ����� ����������
    int numParams;
    
    // ����� �������� ��������� � ������� params
    int currentParam;

    static void DrawSine(const Chan &, int x, int y, int width, int height);

    static void DrawRampPlus(const Chan &, int x, int y, int width, int height);

    static void DrawRampMinus(const Chan &, int x, int y, int width, int height);

    static void DrawTriangle(const Chan &, int x, int y, int width, int height);

    static void DrawMeander(const Chan &, int x, int y, int width, int height);

    static void DrawImpulse(const Chan &, int x, int y, int width, int height);

    static void DrawPacketImpulse(const Chan &, int x, int y, int width, int height);

    static void DrawFree(const Chan &, int x, int y, int width, int height);

    // ����� ����� ��������� �������� ��������� ����� ��� ��������� ���������� ���������
    struct Old
    {
        // ����� ����������� ��������� �� �������� ��������� � ������ ��������� �������� ���������
        Param **params;
        int     numParams;
        int     currentParam;
        Old() : params(nullptr), numParams(0), currentParam(0) { }
        void Init(Param **parameters, int num, int current) { params = parameters; numParams = num; currentParam = current; }
    } old;

    struct CheckerParameters
    {
        CheckerParameters(const Form *_form);

        // ��������� ������������ ��������
        bool InvalidDurationImpulse() const;

        const Form *form;
    };
};



class Wave
{
public:

    Wave(const Chan &, Form **forms);
    
    // ���������� ������������� �����
    Form *GetCurrentForm();
    
    // ���������� ������ ������� ����� �������.
    void SetIndexForm(int num) { numberForm = num; };
    int GetIndexForm() const { return numberForm; }
    // ������������� ������� �����. ���� ����� �� ����������� ���� �����, �� ������ �� ����������
    void SetForm(Form *);

    int NumberOfForms() const;

    Form *GetForm(int i);

    Form *GetForm(TypeForm::E form);

    Chan GetChannel() const { return channel; };
    
    // ���������� true, ���� ���������� ������ ����� �������
    bool StartModeIsSingle();

    void Reset();

    // ��������� ������ ������� �����
    void StoreIndexCurrentForm() { stored.index_form = numberForm; };

    // ��������������� ������ ������� �����
    void RestoreIndexCurrentForm() { numberForm = stored.index_form; };

private:
    
    Chan channel;           // ������ ������ ����������� ������
    
    int numberForm;         // ������� ����� ������� - ��������� �� ����� ������� � �������
    
    Form **forms;           // ������ ����, ������� ����� ���� ���������
    
    int numForms;           // ���������� ��������� ����

    struct Stored
    {
        Stored() : index_form(0) { }
        int index_form;

    } stored;
};
