#include "bsp_beep.h"

int beep_state = OFF;

void beep_init(void)
{
	IOMUXC_SetPinMux(IOMUXC_SNVS_SNVS_TAMPER1_GPIO5_IO01, 0);
	IOMUXC_SetPinConfig(IOMUXC_SNVS_SNVS_TAMPER1_GPIO5_IO01, 0x000010b0);

	GPIO5->GDIR |= (1<<1);
	GPIO5->DR |= (1<<1);
}

void beep_switch(int status)
{
	if (status == ON) {
		GPIO5->DR &= ~(1<<1);
		beep_state = ON;
	}
	if (status == OFF) {
		GPIO5->DR |= (1<<1);
		beep_state = OFF;
	}
}

