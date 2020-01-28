#pragma once
#include "defines.h"
#include "AD9952.h"
#include "AD5697.h"
#include "FPGA.h"
#include "common/Command.h"


struct DGenerator
{
    static const int DDS_NUM_POINTS = 8 * 1024;

    static void Init();

    static void EnableChannel(Chan::E ch, bool enable);

    static void SetFormWave(Chan::E ch, TypeForm::E form);

    static void SetOffset(Chan::E ch, FloatValue offset);

    static void SetFrequency(Chan::E ch, FloatValue frequency);

    static void SetAmplitude(Chan::E ch, FloatValue amplitude);

    static void SetDuration(Chan::E ch, FloatValue duration);

    static void SetDutyRatio(Chan::E ch, FloatValue dutyRatio);

    static void SetPhase(Chan::E ch, FloatValue phase);

    static void SetDelay(Chan::E ch, FloatValue duration);

    static void SetManipulationDuration(Chan::E ch, FloatValue duration);

    static void SetManipulationPeriod(Chan::E ch, FloatValue period);

    static void SetPacketPeriod(Chan::E ch, FloatValue duration);

    static void SetPacketNumber(Chan::E ch, FloatValue number);

    static void SetPeriod(Chan::E ch, FloatValue period);
    /// Возвращает установленную на канале амплитуду
    static float GetAmplitude(Chan::E ch) { return amplitude[ch]; }
    /// Возвращает установленное на канале смещение
    static float GetOffset(Chan::E ch) { return offset[ch]; }
    /// Возвращает true, если на канале ch установлена синусоидальная форма сигнала
    static bool FormIsSine(Chan::E ch) { return waveIsSine[ch]; }

private:
    /// true, если на канале установлена форма сигнала "синусоида"
    static bool waveIsSine[Chan::Count];
    /// Текущая установленная амплитуда на канале
    static float amplitude[Chan::Count];
    /// Текущая установленное смещение на кнаале
    static float offset[Chan::Count];
};


struct Attenuation
{
    enum E
    {
        _0Db,
        _10Db,
        _20Db,
        _30Db,
        Count
    };
};


struct Attenuator
{
    static void SetAttenuation(Chan::E ch, Attenuation::E attenuation)
    {
        static const HPort::E gpio0[Chan::Count] = { HPort::_E, HPort::_F };
        static const HPort::E gpio1[Chan::Count] = { HPort::_B, HPort::_C };

        static const uint16 pin0[Chan::Count] = { HPin::_15, HPin::_5 };
        static const uint16 pin1[Chan::Count] = { HPin::_5,  HPin::_13 };

        static const HState::E state0[Attenuation::Count] =
        {
            HState::Reset,
            HState::Set,
            HState::Reset,
            HState::Set
        };

        static const HState::E state1[Attenuation::Count] =
        {
            HState::Reset,
            HState::Reset,
            HState::Set,
            HState::Set
        };

        HAL_PIO::Write(gpio0[ch], pin0[ch], state0[attenuation]);
        HAL_PIO::Write(gpio1[ch], pin1[ch], state1[attenuation]);
    }

    static void Init()
    {
        HAL_PIO::Init(HPort::_E, HPin::_15, HMode::Output_PP, HPull::No); //-V525

        HAL_PIO::Init(HPort::_B, HPin::_10, HMode::Output_PP, HPull::No);

        HAL_PIO::Init(HPort::_F, HPin::_0, HMode::Output_PP, HPull::No);
        HAL_PIO::Init(HPort::_F, HPin::_5, HMode::Output_PP, HPull::No);

        HAL_PIO::Init(HPort::_C, HPin::_13, HMode::Output_PP, HPull::No);
        HAL_PIO::Init(HPort::_C, HPin::_14, HMode::Output_PP, HPull::No);
    }
};


struct Amplifier
{
    static void Init()
    {
        Attenuator::Init();
    }


    static void Tune(Chan::E ch)
    {
        float amplitude = DGenerator::GetAmplitude(ch);
        float offset = DGenerator::GetOffset(ch);

        if(amplitude > 3.16F)              // 1 диапазон
        {
            SetState(ch, true);
            Attenuator::SetAttenuation(ch, Attenuation::_0Db);
        }
        else if(amplitude > 1.0F)          // 2 диапазон
        {
            SetState(ch, true);
            Attenuator::SetAttenuation(ch, Attenuation::_10Db);
        }
        else if(amplitude > 0.316F)        // 3,4 диапазоны
        {
            if(offset > 2.5F)
            {
                SetState(ch, true);
                Attenuator::SetAttenuation(ch, Attenuation::_20Db);
            }
            else
            {
                SetState(ch, false);
                Attenuator::SetAttenuation(ch, Attenuation::_0Db);
            }
        }
        else if(amplitude > 0.100F)        // 5 диапазон
        {
            SetState(ch, false);
            Attenuator::SetAttenuation(ch, Attenuation::_10Db);
        }
        else if(amplitude > 0.0316F)        // 6 диапазон
        {
            SetState(ch, false);
            Attenuator::SetAttenuation(ch, Attenuation::_20Db);
        }
        else                                // 7 диапазон
        {
            SetState(ch, false);
            Attenuator::SetAttenuation(ch, Attenuation::_30Db);
        }
    }


    static void SetState(Chan::E ch, bool state)
    {
        static const HPort::E gpio[Chan::Count] = { HPort::_F, HPort::_C };
        static const uint16   pin[Chan::Count] = { HPin::_0,  HPin::_14 };

        HAL_PIO::Write(gpio[ch], pin[ch], state);
    }

};
