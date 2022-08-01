// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Common.h"
#include "Signals/Parameters.h"
#include "Settings/SettingsTypes.h"
#include <cstring>
#include <limits>


class Wave;


// Из таого количества точек состоит отрисованный сигнал
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

    // Возвращает человеческое название формы сигнала
    pString Name(uint lang = static_cast<uint>(-1)) const;
    
    // Возвращает ссылку на текущий параметр
    Parameter *CurrentParameter() const;
    
    // Возвращает количество доступных параметров
    int NumParameters() const;
    
    // Возвращает ссылку на i-ый параметр из массива params
    Parameter *GetParameter(int i) const;
    
    // Установить текущим следующй параметр
    void SetNextParameter();

    // Установить текущим предыдущий параметр
    void SetPrevParameter();
    
    // Настраивает генератор в соответствии с установленными параметрами
    void TuneGenerator();
    
    // Возвращает true, если тип формы сигнала соответствует e
    bool Is(TypeForm::E e) const { return e == value; };
    
    // Возвращает true, если форма сигнала реализуется с помощью ПЛИС
    bool IsDDS() const;
    
    // Возвращает указатель на родительский Wave
    Wave *GetWave() const { return wave; };
  
    // Нарисовать изображение сигнала
    void DrawUGO(const Chan &, int y0) const;
    
    // Установить данные произвольного сигнала для отрисовки
    static void SetFormFlash(const Chan &, const uint8 data[POINTS_IN_FORM]);
    
    // Возвращает указатель на данные произвольного сигнала для отрисовки
    static uint8 *GetFormFlash(const Chan &);
    
    // Получить смещение сигнала
    double GetOffset() const;
    
    // Получить размах сигнала
    double GetAmplitude() const;

    operator TypeForm::E() const { return value; }
    
    // Находит требуемый параметр. Возвращает 0, если такого параметра нет
    ParameterDouble *FindParameter(ParameterDoubleType::E p) const;
    ParameterChoice *FindParameter(ParameterChoiceType::E p) const;
    ParameterInteger *FindParameter(ParameterIntegerType::E p) const;
    ParameterComposite *FindParameter(ParameterCompositeType::E p) const;
    
    // Засылает параметр в генератор
    void SendParameterToGenerator(ParameterDoubleType::E p) const;
    void SendParameterToGenerator(ParameterChoiceType::E p) const;
    void SendParameterToGenerator(ParameterIntegerType::E p) const;

    // Раскрывает составной параметр
    void OpenCompositeParameter();
    // Закрывает открытый параметр, если таковой имеется и возвращает true в этом случае
    bool CloseCompositeParameter();

    void Reset();

    // Сохранить состояние
    void StoreState();

    // Восстановить ранее сохранённое состояние
    void RestoreState();

    // Возвращает строку недопустимых параметров, если таковые имеются
    String InvalidParameters() const;

private:
    
    // Wave, к которому относится данный Form
    Wave *wave;
    
    // Здесь хранятся параметры
    Parameter **params;
    
    // Сколько всего параметров
    int numParams;
    
    // Номер текущего параметра в массиве params
    int currentParam;

    static void DrawSine(const Chan &, int x, int y, int width, int height);

    static void DrawRampPlus(const Chan &, int x, int y, int width, int height);

    static void DrawRampMinus(const Chan &, int x, int y, int width, int height);

    static void DrawTriangle(const Chan &, int x, int y, int width, int height);

    static void DrawMeander(const Chan &, int x, int y, int width, int height);

    static void DrawImpulse(const Chan &, int x, int y, int width, int height);

    static void DrawPacketImpulse(const Chan &, int x, int y, int width, int height);

    static void DrawFree(const Chan &, int x, int y, int width, int height);

    // Здесь будут храниться основные параметры формы при раскрытии составного параметра
    struct Old
    {
        // Здесь сохраняется указатель на основные параметры в случае раскрытия сложного параметра
        Parameter **params;
        int numParams;
        int currentParam;
        Old() : params(nullptr), numParams(0), currentParam(0) { }
        void Init(Parameter **parameters, int num, int current) { params = parameters; numParams = num; currentParam = current; }
    } old;

    struct CheckerParameters
    {
        CheckerParameters(const Form *_form);

        // Проверить длительность импульса
        bool CheckDurationImpulse() const;

        const Form *form;
    };
};



class Wave
{
public:

    Wave(const Chan &, Form **forms);
    
    // Возвращает установленную форму
    Form *GetCurrentForm();
    
    // Установить индекс текущей формы сигнала.
    void SetIndexForm(int num) { numberForm = num; };
    int GetIndexForm() const { return numberForm; }
    // Устанавливает текущую форму. Если форма не принадлежит этой волне, то ничего не происходит
    void SetForm(Form *);

    int NumberOfForms() const;

    Form *GetForm(int i);

    Form *GetForm(TypeForm::E form);

    Chan GetChannel() const { return channel; };
    
    // Возвращает true, если установлен ручной режим запуска
    bool StartModeIsSingle();

    void Reset();

    // Сохраняет нидекс текущей формы
    void StoreIndexCurrentForm() { stored.index_form = numberForm; };

    // Восстанавливает индекс текущей формы
    void RestoreIndexCurrentForm() { numberForm = stored.index_form; };

private:
    
    Chan channel;           // Какому каналу принадлежит сигнал
    
    int numberForm;         // Текущая форма сигнала - указывает на номер сигнала в массиве
    
    Form **forms;           // Список форм, которые могут быть назначены
    
    int numForms;           // Количество возможных форм

    struct Stored
    {
        Stored() : index_form(0) { }
        int index_form;

    } stored;
};
