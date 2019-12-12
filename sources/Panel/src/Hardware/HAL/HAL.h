#pragma once


struct HAL_DAC2
{
	static void Init();
	static void StartDMA(uint prescaler);
	static void StopDMA();
private:
	static void ConfigTIM7(uint prescaler);
};
