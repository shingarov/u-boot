/*
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <common.h>
#include <asm/io.h>
#include <command.h>
#include <pci.h>

int board_init(void)
{
	DECLARE_GLOBAL_DATA_PTR;
	unsigned long gpio;
	unsigned long reg;

	/* AHB Controller */
	*((volatile ulong*) 0x1E600000)  = 0xAEED1A03;	/* unlock AHB controller */
	*((volatile ulong*) 0x1E60008C) |= 0x01;	/* map DRAM to 0x00000000 */

	/* Flash Controller */
	*((volatile ulong*) 0x1e620000) |= 0x800f0000;	/* enable Flash Write */

	/* SCU */
	*((volatile ulong*) 0x1e6e2000) = 0x1688A8A8;	/* unlock SCU */
	reg = *((volatile ulong*) 0x1e6e2008);
	reg &= 0x1c0fffff;
	reg |= 0x61800000;				/* PCLK  = HPLL/8 */

	//check lpc or lpc+ mode
	gpio = *((volatile ulong*) 0x1e780070);		/* mode check */
	if(gpio & 0x2)
		reg |= 0x100000;				/* LHCLK = HPLL/4 */
	else
		reg |= 0x300000;				/* LHCLK = HPLL/8 */

	reg |= 0x80000; 				/* enable LPC Host Clock */

	*((volatile ulong*) 0x1e6e2008) = reg;

	reg = *((volatile ulong*) 0x1e6e200c);		/* enable LPC clock */
	*((volatile ulong*) 0x1e6e200c) &= ~(1 << 28);

	if(gpio & 0x2) {
		//use LPC+ for sys clk
		// set OSCCLK = VPLL1
		*((volatile ulong*) 0x1e6e2010) = 0x18;

		// enable OSCCLK
		reg = *((volatile ulong*) 0x1e6e202c);
		reg |= 0x00000002;
		*((volatile ulong*) 0x1e6e202c) = reg;
	} else {
		// USE LPC use D2 clk
		/*set VPPL1 */
		*((volatile ulong*) 0x1e6e201c) = 0x6420;

		// set d2-pll & enable d2-pll D[21:20], D[4]
		reg = *((volatile ulong*) 0x1e6e202c);
		reg &= 0xffcfffef;
		reg |= 0x00200010;
		*((volatile ulong*) 0x1e6e202c) = reg;

		// set OSCCLK = VPLL1
		*((volatile ulong*) 0x1e6e2010) = 0x8;

		// enable OSCCLK
		reg = *((volatile ulong*) 0x1e6e202c);
		reg &= 0xfffffffd;
		reg |= 0x00000002;
		*((volatile ulong*) 0x1e6e202c) = reg;
	}
	reg = *((volatile ulong*) 0x1e6e200c);		/* enable 2D Clk */
	*((volatile ulong*) 0x1e6e200c) &= 0xFFFFFFFD;
	/* enable wide screen. If your video driver does not support wide screen, don't
	   enable this bit 0x1e6e2040 D[0]*/
	reg = *((volatile ulong*) 0x1e6e2040);
	*((volatile ulong*) 0x1e6e2040) |= 0x01;

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
#define AST_WDT_BASE 0x1e785000
#define AST_WDT_CLK (1*1000*1000) /* 1M clock source */
	u32 reload = AST_WDT_CLK * CONFIG_ASPEED_WATCHDOG_TIMEOUT;
	/* set the reload value */
	__raw_writel(reload, AST_WDT_BASE + 0x04);
	/* magic word to reload */
	__raw_writel(0x4755, AST_WDT_BASE + 0x08);
	/* start the watchdog with 1M clk src and reset whole chip */
	__raw_writel(0x33, AST_WDT_BASE + 0x0c);
	printf("Watchdog: %us\n", CONFIG_ASPEED_WATCHDOG_TIMEOUT);
#endif
}

int misc_init_r(void)
{
    unsigned int reg1, revision, chip_id;

    /* Show H/W Version */
    reg1 = (unsigned int) (*((ulong*) 0x1e6e207c));
    chip_id = (reg1 & 0xff000000) >> 24;
    revision = (reg1 & 0xff0000) >> 16;

    puts ("H/W:   ");
    if (chip_id == 1) {
	if (revision >= 0x80) {
		printf("AST2300 series FPGA Rev. %02x \n", revision);
	}
	else {
		printf("AST2300 series chip Rev. %02x \n", revision);
	}
    }
    else if (chip_id == 2) {
	printf("AST2400 series chip Rev. %02x \n", revision);
    }
    else if (chip_id == 0) {
		printf("AST2050/AST2150 series chip\n");
    }

    if (getenv ("verify") == NULL) {
	setenv ("verify", "n");
    }
    if (getenv ("eeprom") == NULL) {
	setenv ("eeprom", "y");
    }

    watchdog_init();
    return 0;
}

int board_eth_init(bd_t *bis)
{
	int ret = -1;
	ret = aspeednic_initialize(bis);

	return ret;
}
