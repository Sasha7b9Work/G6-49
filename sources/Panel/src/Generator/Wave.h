#pragma once
#include "common/Common.h"
#include "Generator/Parameters.h"
#include "Settings/SettingsTypes.h"
#include <cstring>
#include <limits>


class Wave;


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
    Parameter *GetParameter(int i);
    
    // Установить текущим следующй параметр
    void SetNextParameter();
    
    // Настраивает генератор в соответствии с установленными параметрами
    void TuneGenerator();
    
    // Возвращает true, если тип формы сигнала соответствует e
    bool Is(TypeForm::E e) const { return e == value; };
    
    // Возвращает true, если форма сигнала реализуется с помощью ПЛИС
    bool IsDDS() const;
    
    // Возвращает указатель на родительский Wave
    Wave *GetWave() { return wave; };
  
    // Нарисовать изображение сигнала
    void DrawUGO(Chan::E ch, int y0);
    
    // Установить данные произвольного сигнала для отрисовки
    static void SetFormFlash(Chan::E ch, const uint8 data[300]);
    
    // Возвращает указатель на данные произвольного сигнала для отрисовки
    static uint8 *GetFormFlash(Chan::E ch);
    
    // Получить смещение сигнала
    double GetOffset();
    
    // Получить размах сигнала
    double GetAmplitude();

    operator TypeForm::E() { return value; }
    
    // Находит требуемый параметр. Возвращает 0, если такого параметра нет
    ParameterDouble *FindParameter(ParameterDoubleType::E p);
    ParameterChoice *FindParameter(ParameterChoiceType::E p);
    ParameterInteger *FindParameter(ParameterIntegerType::E p);
    ParameterComposite *FindParameter(ParameterCompositeType::E p);
    
    // Засылает параметр в генератор
    void SendParameterToGenerator(ParameterDoubleType::E p);
    void SendParameterToGenerator(ParameterChoiceType::E p);
    void SendParameterToGenerator(ParameterIntegerType::E p);

    // Раскрывает составной параметр
    void OpenCompositeParameter();
    // Закрывает открытый параметр, если таковой имеется и возвращает true в этом случае
    bool CloseCompositeParameter();

private:
    
    // Wave, к которому относится данный Form
    Wave *wave;
    
    // Здесь хранятся параметры
    Parameter **params;
    
    // Сколько всего параметров
    int numParams;
    
    // Номер текущего параметра в массиве params
    int currentParam;

    static void DrawSine(Chan::E ch, int x, int y, int width, int height);

    static void DrawRampPlus(Chan::E ch, int x, int y, int width, int height);

    static void DrawRampMinus(Chan::E ch, int x, int y, int width, int height);

    static void DrawTriangle(Chan::E ch, int x, int y, int width, int height);

    static void DrawMeander(Chan::E ch, int x, int y, int width, int height);

    static void DrawImpulse(Chan::E ch, int x, int y, int width, int height);

    static void DrawPacketImpulse(Chan::E ch, int x, int y, int width, int height);

    static void DrawFree(Chan::E ch, int x, int y, int width, int height);

    // Здесь будут храниться основные параметры формы при раскрытии составного параметра
    struct Old
    {
        // Здесь сохраняется указатель на основные параметры в случае раскрытия сложного параметра
        Parameter **params;
        int numParams;
        int currentParam;
        void Init(Parameter **parameters, int num, int current) { params = parameters; numParams = num; currentParam = current; }
    } old;
};



class Wave
{
public:

    Wave(Chan::E ch, Form **forms);
    
    // Возвращает установленную форму
    Form *GetCurrentForm();
    
    // Установить индекс текущей формы сигнала.
    void SetIndexForm(int8 num);

    int NumberOfForms() const;

    Form *GetForm(int i);

    Form *GetForm(TypeForm::E form);

    Chan GetChannel() const { return channel; };
    
    // Возвращает true, если установлен ручной режим запуска
    bool StartModeIsSingle();

private:
    
    // Какому каналу принадлежит сигнал
    Chan channel;
    
    // Текущая форма сигнала - указывает на номер сигнала в массиве
    int indexCurrentForm;
    
    // Список форм, которые могут быть назначены
    Form **forms;
    
    // Количество возможных форм
    int numForms;
};
