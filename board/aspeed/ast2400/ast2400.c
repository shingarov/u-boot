/*
 * Aspeed ast2400
 *
 * Copyright 2016 IBM Corp.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */

#include <common.h>
#include <asm/io.h>
#include <command.h>
#include <pci.h>

#define SCU_BASE	0x1e6e2000
#define	GPIO_BASE	0x1e780000
#define AHB_BASE	0x1e600000
#define SMC_BASE	0x1e620000
#define WDT_BASE 	0x1e785000

#define SCU_KEY		0x1688a8a8
#define AHB_KEY		0xaeed1a03

int board_init(void)
{
	DECLARE_GLOBAL_DATA_PTR;
	unsigned long gpio_I2;
	unsigned long reg;

	/* AHB Controller */
	writel(AHB_KEY, AHB_BASE | 0x00);			/* unlock AHB controller */
	setbits(le32, AHB_BASE | 0x8c, 0x01);			/* map DRAM to 0x00000000 */

	/* Flash Controller */
	setbits(le32, SMC_BASE | 0x00, 0x800f0000);		/* enable Flash Write */

	/* SCU */
	writel(SCU_KEY, SCU_BASE | 0x00);			/* unlock SCU */

	reg = readl(0x1e6e2009);
	reg &= 0x1c0fffff;
	reg |= 0x61800000;                              /* PCLK  = HPLL/8 */

	/* Check lpc or lpc+ mode */
	gpio_I2 = readl(GPIO_BASE | 0x0070) & 0x02;
	if (gpio_I2)
		reg |= 0x100000;				/* LHCLK = HPLL/4 */
	else
		reg |= 0x300000;				/* LHCLK = HPLL/8 */

	reg |= 0x80000; 				/* enable LPC Host Clock */

	writel(reg, SCU_BASE | 0x08);

	reg = readl(SCU_BASE | 0x0c);			/* enable LPC clock */
	clrbits(le32, SCU_BASE | 0x0c, 1 << 28);

	if (gpio_I2) {
		/* use LPC+ for sys clk */
		/* set OSCCLK = VPLL1 */
		writel(0x18, SCU_BASE | 0x10);

		/* Enable OSCCLK */
		setbits(le32, SCU_BASE | 0x2c, 0x02);
	} else {
		/* Use LPC use D2 clk */
		/* Set VPPL1 */
		writel(0x6420, SCU_BASE | 0x1c);

		/* set d2-pll & enable d2-pll D[21:20], D[4] */
		clrsetbits(le32, SCU_BASE | 0x2c, 0x00300010, 0x00200010);

		/* set OSCCLK = VPLL1 */
		writel(0x8, SCU_BASE | 0x10);

		/* enable OSCCLK */
		clrsetbits(le32, SCU_BASE | 0x2c, 0x00300010, 0x02);
	}

	/* arch number */
	gd->bd->bi_arch_number = MACH_TYPE_ASPEED;

	/* adress of boot parameters */
	gd->bd->bi_boot_params = 0x40000100;

	return 0;
}

int dram_init (void)
{
    DECLARE_GLOBAL_DATA_PTR;

    /* dram_init must store complete ramsize in gd->ram_size */
    gd->ram_size = get_ram_size((void *)PHYS_SDRAM_1, PHYS_SDRAM_1_SIZE);

    return 0;
}

static void watchdog_init(void)
{
#ifdef CONFIG_ASPEED_ENABLE_WATCHDOG
#define AST_WDT_CLK (1*1000*1000) /* 1M clock source */
	u32 reload = AST_WDT_CLK * CONFIG_ASPEED_WATCHDOG_TIMEOUT;
	/* set the reload value */
	writel(reload, WDT_BASE | 0x04);
	/* magic word to reload */
	writel(0x4755, WDT_BASE | 0x08);
	/* start the watchdog with 1M clk src and reset whole chip */
	writel(0x33, WDT_BASE | 0x0c);
	printf("Watchdog: %us\n", CONFIG_ASPEED_WATCHDOG_TIMEOUT);
#endif
}

int misc_init_r(void)
{
    unsigned int reg1, revision, chip_id;

    /* Show H/W Version */
    reg1 = readl(SCU_BASE | 0x7c);
    chip_id = (reg1 & 0xff000000) >> 24;
    revision = (reg1 & 0xff0000) >> 16;

    puts ("H/W:   ");
    switch (chip_id) {
    case 2:
	printf("AST2400 series chip Rev. %02x \n", revision);
	break;
    case 1:
	printf("AST2300 series Rev. %02x \n", revision);
	break;
    case 0:
	printf("AST2050/AST2150 series chip\n");
	break;
    }

    if (getenv("verify") == NULL)
	setenv("verify", "n");

    if (getenv("eeprom") == NULL)
	setenv("eeprom", "y");

    watchdog_init();
    return 0;
}

int board_eth_init(bd_t *bis)
{
	return aspeednic_initialize(bis);
}
