// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Common.h"
#include "Generator/Parameters.h"
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
        Init(static_cast<TypeForm::E>(v));
    };

    Form(TypeForm::E v, Parameter **param, Wave *w);

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
    pString Name(uint lang = static_cast<uint>(-1)) const;
    
    // ���������� ������ �� ������� ��������
    Parameter *CurrentParameter() const;
    
    // ���������� ���������� ��������� ����������
    int NumParameters() const;
    
    // ���������� ������ �� i-�� �������� �� ������� params
    Parameter *GetParameter(int i);
    
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
    Wave *GetWave() { return wave; };
  
    // ���������� ����������� �������
    void DrawUGO(Chan::E ch, int y0);
    
    // ���������� ������ ������������� ������� ��� ���������
    static void SetFormFlash(Chan::E ch, const uint8 data[POINTS_IN_FORM]);
    
    // ���������� ��������� �� ������ ������������� ������� ��� ���������
    static uint8 *GetFormFlash(Chan::E ch);
    
    // �������� �������� �������
    double GetOffset();
    
    // �������� ������ �������
    double GetAmplitude();

    operator TypeForm::E() { return value; }
    
    // ������� ��������� ��������. ���������� 0, ���� ������ ��������� ���
    ParameterDouble *FindParameter(ParameterDoubleType::E p);
    ParameterChoice *FindParameter(ParameterChoiceType::E p);
    ParameterInteger *FindParameter(ParameterIntegerType::E p);
    ParameterComposite *FindParameter(ParameterCompositeType::E p);
    
    // �������� �������� � ���������
    void SendParameterToGenerator(ParameterDoubleType::E p);
    void SendParameterToGenerator(ParameterChoiceType::E p);
    void SendParameterToGenerator(ParameterIntegerType::E p);

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
    Parameter **params;
    
    // ������� ����� ����������
    int numParams;
    
    // ����� �������� ��������� � ������� params
    int currentParam;

    static void DrawSine(Chan::E ch, int x, int y, int width, int height);

    static void DrawRampPlus(Chan::E ch, int x, int y, int width, int height);

    static void DrawRampMinus(Chan::E ch, int x, int y, int width, int height);

    static void DrawTriangle(Chan::E ch, int x, int y, int width, int height);

    static void DrawMeander(Chan::E ch, int x, int y, int width, int height);

    static void DrawImpulse(Chan::E ch, int x, int y, int width, int height);

    static void DrawPacketImpulse(Chan::E ch, int x, int y, int width, int height);

    static void DrawFree(Chan::E ch, int x, int y, int width, int height);

    // ����� ����� ��������� �������� ��������� ����� ��� ��������� ���������� ���������
    struct Old
    {
        // ����� ����������� ��������� �� �������� ��������� � ������ ��������� �������� ���������
        Parameter **params;
        int numParams;
        int currentParam;
        Old() : params(nullptr), numParams(0), currentParam(0) { }
        void Init(Parameter **parameters, int num, int current) { params = parameters; numParams = num; currentParam = current; }
    } old;
};



class Wave
{
public:

    Wave(Chan::E ch, Form **forms);
    
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
