#pragma once
#include <ff.h>
#include "Message.h"



struct DDrive
{
    static void Init();

    static void Update();

    static USBH_HandleTypeDef hUSB_Host;

    struct Handler
    {
        static void Processing(SimpleMessage *msg);
    private:
        static SimpleMessage *msg;
        static void GetNumDirsAndFiles();
        static void RequestFile();
        static void RequestFileSize();
        /// Загрузить сигнал с флешки
        static void LoadFromExtStorage();
        static void GetPictureDDS();
        static void E();
    };

private:
    /// Трансформировать точки в пригодный для записи в ПЛИС вид
    static void TransformDataToCode(float d[4096], uint8 code[FPGA::NUM_POINTS * 2]);

    static void Normalize(float d[4096]);

    static void FindMinMax(const float d[4096], float *_min, float *_max);

    static float FindScale(float min, float max);

    static void ToScale(float d[4096], float scale);
    /// Заполнить массив picture данными для отрисовки сигнала на экране
    static void FillPicture(uint8 *picture, uint size, float values[4096]);
};
