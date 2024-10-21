/*
 * C code entry: dram_init_main();
 */

#include <types.h>
#include <common.h>
#include <config.h>
#include <dwc_dram_param.h>
#include <dwc_ddrphy_phyinit.h>
#include <bootmain.h>
#include <nand_boot/nfdriver.h>
#include <nand_boot/nandop.h>
#include <nand_boot/hal_nand_error.h>
#include <fat/common.h>
#include <fat/fat.h>


#define REG_BASE_AO       0xF8800000
#define RF_GRP_AO(_grp, _reg)           ((((_grp) * 32 + (_reg)) * 4) + REG_BASE_AO)


struct sp_registers_ao {
	unsigned int sp_register[5][32];
};
static volatile struct sp_registers_ao *sp_reg_ptr_AO = (volatile struct sp_registers_ao *)(RF_GRP_AO(0, 0));
#define SP_REG_AO(GROUP, OFFSET)	(sp_reg_ptr_AO->sp_register[GROUP][OFFSET])



struct uart_regs {
	unsigned int dr;  /* data register */
	unsigned int lsr; /* line status register */
	unsigned int msr; /* modem status register */
	unsigned int lcr; /* line control register */
	unsigned int mcr; /* modem control register */
	unsigned int div_l;
	unsigned int div_h;
	unsigned int isc; /* interrupt status/control */
	unsigned int txr; /* tx residue */
	unsigned int rxr; /* rx residue */
	unsigned int thr; /* rx threshold */
	unsigned int clk_src;
	unsigned int smp_rate;
};
#define UART0_REG    ((volatile struct uart_regs *)RF_GRP_AO(50, 0))
#ifdef XBOOT_BUILD
#define CMDLINE "XB> "
#else
#ifdef PLATFORM_SPIBAREMETAL
#define CMDLINE "NOR> "
#else
#define CMDLINE "ROM> "
#endif
#endif
#define DBG_UART_REG       UART0_REG
#define UART_LSR_RX1             (1 << 1)
#define UART_RX_READY1()        ((DBG_UART_REG->lsr) & UART_LSR_RX1)
#define UART_GET_ERROR1()       (((DBG_UART_REG->lsr) << 3) & 0xE0)
#define IS_NEWLINE1(_byte)       (((_byte) == 0x0D) || ((_byte) == 0x0A))
extern void dbg_uart_putc(unsigned char);
static u8 dbg_uart_getc(void)
{
        u8 uart_data;
        while (!(UART_RX_READY1()));
        uart_data = DBG_UART_REG->dr;
        if (UART_GET_ERROR1()) {
                uart_data = 'E';
        }
        return uart_data;
}
static u8 uart_getc_show_char(void)
{
	u8 byte;
	byte = dbg_uart_getc();
	if (IS_NEWLINE1(byte)) {
		dbg_uart_putc('\n');
		dbg_uart_putc('\r');
	} else {
		dbg_uart_putc(byte);
	}
	return byte;
}
static void show_result_reg(u32 group, u32 reg)
{
	prn_string("G"); prn_decimal(group); prn_string("."); prn_decimal(reg);
	prn_string("=");
	prn_dword(*(volatile u32 *)(RF_GRP(group, reg)));
}
static u8 get_word_ex(const char *str, int is_hex, u32 *addr)
{
	u8 count = 0;
	u8 byte;
	u32 result = 0;
	u8 flag = 0;
	if (str) {
		prn_string(str);
	}
	while (1) {
		byte = uart_getc_show_char();
		if (IS_NEWLINE1(byte) || (byte == ' ')) {
			if ((count == 0) && (flag == 0)) {
				continue;
			}
			*addr = result;
			return 1;
		}
		if (('0' <= byte) && (byte <= '9')) {
			if ((count == 0) && (byte == '0')) {
				flag = 1;
				continue;
			}
			byte -= '0';
		} else if (is_hex && ('A' <= byte) && (byte <= 'F')) {
			byte -= '7';
		} else if (is_hex && ('a' <= byte) && (byte <= 'f')) {
			byte -= 'W';
		} else if (byte == 0x08) { /* backspace */
			count--;
			result = is_hex ? (result >> 4) : (result / 10);
			continue;
		} else {
			prn_string("\n?\n");
			return 0;
		}
		result = is_hex ? ((result << 4) + byte) : ((result * 10) + byte);
		count++;
		if (count > 8) {
			prn_string("\n?\n");
			return 0;
		}
	}
	return 0; /* never */
}
static void mon_cmdline(void)
{
	prn_string("\n" CMDLINE "d: dump, r: read, w: write, l: lreg, W: wreg, m: menu, K: reset, q: quit\n");
	prn_string(CMDLINE);
}
void mon_shell2(void)
{
	u8 byte;
	u32 address;
	u32 value;
	u32 group;
	u32 reg;
	while (1) {
		mon_cmdline();
		byte = uart_getc_show_char();
		switch (byte) {
		case 'r':
			if (get_word_ex(" addr=0x", 1, &address)) {
				prn_dword(*(volatile u32 *)address);
			}
			break;
		case 'w':
			if (get_word_ex(" addr=0x", 1, &address)) {
				if (get_word_ex("val=0x", 1, &value)) {
					*(volatile u32 *)address = (u32)value;
					prn_dword(*(volatile u32 *)address);
				}
			}
			break;
		case 'a':
			if (get_word_ex(" addr=0x", 1, &address)) {
				prn_dword(*(volatile u16 *)(0xf9000000 + address*2));
			}
			break;
		case 'b':
			if (get_word_ex(" addr=0x", 1, &address)) {
				if (get_word_ex("val=0x", 1, &value)) {
					*(volatile u16 *)(0xf9000000 + address*2) = (u16)value;
					prn_dword(*(volatile u16 *)(0xf9000000 + address*2));
				}
			}
				break;
#ifdef CONFIG_ARCH_ARM
		case 'S':
#ifdef __thumb__
			asm volatile (".short 0xbf40");
#else
			asm volatile (".word 0xe320f004");
#endif
			break;
#endif
		case 'g':
		case 'l':
			if (get_word_ex(" G=", 0, &group)) {
				int i;
				for (i = 0; i < 32; i++) {
					show_result_reg(group, i);
				}
			}
			break;
		case 'W':
			if (get_word_ex(" G=", 0, &group)) {
				prn_string("G"); prn_decimal(group);
				if (get_word_ex(".", 0, &reg)) {
					if (get_word_ex("val=0x", 1, &value)) {
						*(volatile u32 *)(RF_GRP(group, reg)) = value;
						show_result_reg(group, reg);
					}
				}
			}
			break;
		case 'd':
			if (get_word_ex(" addr=0x", 1, &address)) {
				if (get_word_ex("size=0x", 1, &value)) {
					prn_dump_buffer((u8 *)address, value);
				}
			}
			break;
		case 'q':
			return;
		default:
			prn_string("\n?");
			break;
		}
	}
}
void dwc_ate(void)
{
	prn_string("reset\n");
	SP_REG_AO(0, 14) =0x00200000;
	SP_REG_AO(0, 14) =0x00200000;
	SP_REG_AO(0, 14) =0x00200000;
	SP_REG_AO(0, 14) =0x00200000;
	SP_REG_AO(0, 14) =0x00200000;
	SP_REG_AO(0, 14) =0x00200000;
	SP_REG_AO(0, 14) =0x00200000;
	SP_REG_AO(0, 14) =0x00200000;
	wait_loop(1000);
	wait_loop(1000);
	wait_loop(1000);
	wait_loop(1000);
	wait_loop(1000);
	wait_loop(1000);
	
	SP_REG_AO(0, 14) =0x00200020;
	SP_REG_AO(0, 14) =0x00200020;
	SP_REG_AO(0, 14) =0x00200020;
	SP_REG_AO(0, 14) =0x00200020;
	SP_REG_AO(0, 14) =0x00200020;
	SP_REG_AO(0, 14) =0x00200020;
	SP_REG_AO(0, 14) =0x00200020;
	SP_REG_AO(0, 14) =0x00200020;
	wait_loop(1000);
	wait_loop(1000);
	wait_loop(1000);
	wait_loop(1000);
	wait_loop(1000);
	wait_loop(1000);
	mon_shell2();
	
	prn_string("DDRPHY_Function_ATE_20240822\n");
	#include <SP7350/LPDDR4/DDRPHY_Function_ATE_20240822.txt>	
}

