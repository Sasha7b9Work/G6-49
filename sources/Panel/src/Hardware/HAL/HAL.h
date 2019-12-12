#pragma once


struct HAL_DAC2
{
	static void Init();
	static void StartDMA(void* points, uint numPoints);
	static void StopDMA();
	static void ConfigTIM7(uint16 prescaler, uint16 period);
};
