#include "bsp_delay.h"
#include "bsp_key0.h"

void key0_init(void)
{
        IOMUXC_SetPinMux(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0);
        IOMUXC_SetPinConfig(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0xf080);

        GPIO1->GDIR &= ~(1<<18);
}

const int read_key(void)
{
	return ((GPIO1->DR) >> 18) & 0x1;
}

const int key0_getval(void)
{
	int ret = 0;
	
	if (read_key() == KEY_DOWN) {
		delay(10);

		if (read_key() == KEY_DOWN) {
			ret = KEY_DOWN; 
		}
	} else if (read_key() == KEY_UP) {
		ret = KEY_UP;
	
	}


	return ret;
}


