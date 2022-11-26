#include "bsp_clk.h"

/*
 * @description	: 使能I.MX6U所有外设时钟
 * @param 		: 无
 * @return 		: 无
 */
void clk_enable(void)
{
	CCM->CCGR0 = 0XFFFFFFFF;
	CCM->CCGR1 = 0XFFFFFFFF;
	CCM->CCGR2 = 0XFFFFFFFF;
	CCM->CCGR3 = 0XFFFFFFFF;
	CCM->CCGR4 = 0XFFFFFFFF;
	CCM->CCGR5 = 0XFFFFFFFF;
	CCM->CCGR6 = 0XFFFFFFFF;

}


void imx6u_clkinit(void)
{
	int reg = 0x00000000;
	/*判断当前是否使用的PLL1，如果是先将step_sel设为0选通osc_clk，然后将
	 *pll1_sw_clk_sel设为1,选通step_clk
	 */	
	if (((CCM->CCSR >> 2) & 0x1) == 0) {	//如果正在使用PLL1,将CCSR的step_sel:bit8设为0
		CCM->CCSR &= ~(1 << 8);	//选通osc_clk
		CCM->CCSR |= (1 << 2);	//选通step_clk
	}
	CCM_ANALOG->PLL_ARM = (1 << 13) | ((66 << 0) & 0x7f);//设置主频为792MHz
	CCM->CACRR |= 0;		//divid by 1
	CCM->CCSR &= ~(1 << 2);		//选通pll1

	/*设置PLL2的pfd0-pfd3 output = 528*18/PFD_FRAC*/
	/*
	*pfd0[bit0:5] = 528*18/352 = 27
	*pfd1[bit8:13] = 528*18/594 = 16
	*pfd2[bit16:21] = 528*18/396 = 24
	*pfd3[bit24:29] = 528*18/200 = 32
	*/
	reg = CCM_ANALOG->PFD_528; 
	reg &= ~(0x3f3f3f3f);
	reg |= (27 << 0);
	reg |= (16 << 8);
	reg |= (24 << 16);
	reg |= (32 << 24);
	CCM_ANALOG->PFD_528 = reg;

	/*设置PLL3的pfd0-pfd3*/
	/*
	*pfd0[bit0:5] = 480*18/720 = 12
	*pfd1[bit8:13] = 480*18/540 = 16
	*pfd2[bit16:21] = 480*18/508.2 = 17
	*pfd3[bit24:29] = 480*18/454.7 = 19
	*/
	reg = CCM_ANALOG->PFD_480; 
	reg &= ~(0x3f3f3f3f);
	reg |= (12 << 0);
	reg |= (16 << 8);
	reg |= (17 << 16);
	reg |= (19 << 24);
	CCM_ANALOG->PFD_480 = reg;

	//CBCMR[PRE_PERIPH_CLK_SEL]
	CCM->CBCMR &= ~(0x3 << 18);
	CCM->CBCMR |= (0x1 << 18);

	//CBCDR[PERIPH_CLK_SEL]
	CCM->CBCDR &= ~(0X1 << 25);
	
	//等待握手完成
	while (CCM->CDHIPR & (0X1 << 5));

	//CBCDR[AHB_PODF]
	CCM->CBCDR &= ~(0x7 << 10);	
	CCM->CBCDR |= (0x2 << 10);	
	while (CCM->CDHIPR & (0x1 << 1));

	//CBCDR[IPG_PODF]
	CCM->CBCDR &= ~(0x3 << 8);	
	CCM->CBCDR |= (0x1 << 8);	

	//CCM_CSCMR1[PERCLK_CLK_SEL]
	CCM->CSCMR1 &= ~(0x1 << 6);

	//CCM_CSCMR1[PERCLK_PODF]
	CCM->CSCMR1 &= ~(0x3f << 0);

	/*设置ECSPI根时钟为PLL3——60MHz*/
	CCM->CSCDR2 &= ~(0x1 << 18);
	CCM->CSCDR2 &= ~(0x3f << 19); /*1分频*/
	
}
