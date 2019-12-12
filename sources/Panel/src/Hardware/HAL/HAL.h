#pragma once


struct HAL_DAC2
{
	static void Init();
	static void StartDMA();
	static void StopDMA();
private:
	static void ConfigTIM7();
};
