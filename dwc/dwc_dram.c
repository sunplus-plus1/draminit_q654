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

#define SPI_FLASH_BASE      0xF0000000
#define SPI_XBOOT_OFFSET    (96 * 1024)

#define EMMC_BOOT               0x1F
#define SPINAND_BOOT            0x1D
#define USB_ISP                 0x1B
#define SDCARD_ISP              0x19
#define SPI_NOR_BOOT            0x17
#define UART_ISP                0x15
#ifdef PLATFORM_SP7350
#define PARA_NAND_BOOT          0x11
#else
#define AUTO_SCAN               0x11
#endif
#define USB_BOOT                0xfd
#define SDCARD_BOOT             0xfe
#define NAND_LARGE_BOOT         0xff

#define rsize 512
#define IM1D_HDR_MAGIC   0x64316d69
#define DM1D_HDR_MAGIC   0x64316d64
#define IM2D_HDR_MAGIC   0x64326d69
#define DM2D_HDR_MAGIC   0x64326d64
#define IMDA_HDR_MAGIC   0x6a646d69
#define DMDA_HDR_MAGIC   0x6a646d64

#define USB3_PORT		0
#define USB2_PORT		1

typedef unsigned int        u32;

#define MO3_REG ((volatile struct int_regs *)RF_GRP(3, 0))
struct int_regs {
    UINT32  mo3_reserved[32];
};

#define CSTAMP(value)      { *(volatile unsigned int *)RF_GRP(0, 0) = (unsigned int)ADDRESS_CONVERT(value); }

// #define DRAM_INIT_DEBUG 0 // defined in Makefile: please use "make debug"

#ifdef	CSIM_ASIC
#define	ASIC_CSIM
#elif	defined(CSIM_FPGA)
#define	SDRAM_FPGA
#else
#error Please define => simulation type
#endif

struct sp_registers {
	unsigned int sp_register[1024][32];
};
struct sp_registers_ao {
	unsigned int sp_register[5][32];
};
static volatile struct sp_registers_ao *sp_reg_ptr_AO = (volatile struct sp_registers_ao *)(RF_GRP_AO(0, 0));
#define SP_REG_AO(GROUP, OFFSET)	(sp_reg_ptr_AO->sp_register[GROUP][OFFSET])

struct dwc_phy_regs {
	UINT16 dwc_phy_reg[0x90200];	/* change the size here, (area >> 2) */
};
static volatile struct dwc_phy_regs *dwc_phy_reg_ptr = (volatile struct dwc_phy_regs *)(DWC_PHY_REG_Base);
#define DWC_PHY_REG(OFFSET)		(dwc_phy_reg_ptr->dwc_phy_reg[OFFSET ])

static unsigned int ckobd_training_flag = 0;
#if defined(CONFIG_HAVE_SPI_NAND) || defined(CONFIG_HAVE_PARA_NAND)
static unsigned int pg_off_prev = 0;
static unsigned int chk_multi_img_in_same_page = 0;
#endif
static unsigned int bootdevice;
static unsigned int XBOOT_len  = 0;
static unsigned int IMEM1d_len = 0;
static unsigned int DMEM1d_len = 0;
static unsigned int IMEM2d_len = 0;
static unsigned int DMEM2d_len = 0;
static unsigned int IMDA_len = 0;
static unsigned int DMDA_len = 0;

int mp;
#ifdef PLATFORM_PENTAGRAM
#define CHIP_WARM_RESET
#endif

void wait_loop(unsigned int wait_counter)
{
	unsigned int i;

	for (i = 0; i < wait_counter; i++) {
		__asm__("nop");
	}
}

//////////////////////////////////////////////////////
///////////////LPDDR4////////////////////////////////
//////////////////////////////////////////////////////
void dwc_ddrphy_apb_wr(UINT32 adr, UINT32 dat)
{
	//dwc_ddrphy_phyinit_print ("dwc_ddrphy_apb_wr(12'h%x, 32'h%x);\n", adr, dat);
	#if 0
	if(mp == 1)
	{
		prn_string("APB W  PUB  ");
		prn_dword0(adr);
		prn_string("  ");
		prn_dword0(dat);
		prn_string("\n");
	}
	#endif
	DWC_PHY_REG(adr)=dat;
}

UINT16 dwc_ddrphy_apb_rd(UINT32 adr)
{
	UINT16 value;
	//dwc_ddrphy_phyinit_print ("dwc_ddrphy_apb_rd(12'h%x, rd_data);\n", adr);
	value= DWC_PHY_REG(adr);

#if 0
	if(mp == 1)
	{
		prn_string("APB R  PUB  ");
		prn_dword0(adr);
		prn_string("  ");
		prn_dword0(value);
		prn_string("\n");
	}
#endif
	return value;
}

#define IMEM_ADDR 0x50000
#define DMEM_ADDR 0x54000
#define DIA_DMEM_ADDR 0x54200
#define mem_size 128
unsigned int sum = 0;
unsigned int mem[mem_size];
void tcpsum(unsigned int StartNo, unsigned int EndNo, unsigned int *buf, unsigned char flag)
{
	unsigned short word16_h,word16_l;
	int i;

	/* Accumulate checksum */
	for (i = StartNo; i < EndNo; i++) {
		word16_h = (buf[i]>>16)&0xFFFF;
		word16_l = buf[i]&0xFFFF;
		sum += word16_l;
		sum += word16_h;
	}

	if (flag == 0)
		return;
	/* Fold to get the ones-complement result */
	while (sum >> 16) sum = (sum & 0xFFFF)+(sum >> 16);

	/* Invert to get the negative in ones-complement arithmetic */
	sum = ~sum;

}

void DwcCheckSum(unsigned int magic, unsigned int checksum)
{
	if ((sum&0x0000FFFF) != checksum) {
		if (magic == IM1D_HDR_MAGIC)
			prn_string("1D IMEM ");
		else if (magic == DM1D_HDR_MAGIC)
			prn_string("1D DMEM ");
		else if (magic == IM2D_HDR_MAGIC)
			prn_string("2D IMEM ");
		else if (magic == DM2D_HDR_MAGIC)
			prn_string("2D DMEM ");
		else if (magic == IMDA_HDR_MAGIC)
			prn_string("IMDA ");
		else if (magic == DMDA_HDR_MAGIC)
			prn_string("DMDA ");

		prn_string("checksum error!\n");
		prn_string("sum="); prn_dword(sum);
		prn_string("checksum="); prn_dword((checksum));
	} else {
		if (magic == IM1D_HDR_MAGIC)
			prn_string("1D IMEM checksum ok\n");
		else if (magic == DM1D_HDR_MAGIC)
			prn_string("1D DMEM checksum ok\n");
		else if (magic == IM2D_HDR_MAGIC)
			prn_string("2D IMEM checksum ok\n");
		else if (magic == DM2D_HDR_MAGIC)
			prn_string("2D DMEM checksum ok\n");
		else if (magic == IMDA_HDR_MAGIC)
			prn_string("IMDA checksum ok\n");
		else if (magic == DMDA_HDR_MAGIC)
			prn_string("DMDA checksum ok\n");
	}
	sum = 0;    // Reset DWC checksum
}

void printf_offset_value(unsigned short offset, unsigned short value)
{
	prn_string("mem_offset="); prn_dword(offset);
	prn_string("value="); prn_dword(value);
}

void printf_sectorNo(unsigned short sectorNo0, unsigned short sectorNo1)
{
	prn_string("sectorNo0="); prn_dword(sectorNo0);
	prn_string("sectorNo1="); prn_dword(sectorNo1);
}

void LoadBinCode(unsigned char Train2D, unsigned int offset, unsigned int MEM_ADDR)
{
	struct xboot_hdr *xhdr = (struct xboot_hdr*)(SPI_FLASH_BASE + SPI_XBOOT_OFFSET+offset);//xboot start addr
	unsigned short i, j, addr, num0, num1,word16;

	prn_string("mg=");
	prn_dword(xhdr->magic);
	prn_string("len=");
	prn_dword(xhdr->length);
	prn_string("chk=");
	prn_dword(xhdr->checksum);
	// checksum verify
	num0 = (xhdr->length/rsize);
	num1 = (xhdr->length%rsize);

	for (i=0; i<num0; i++) {
		for (addr=0; addr<mem_size; addr++)
			mem[addr]=0;

		unsigned int *src = (unsigned int*)(SPI_FLASH_BASE + SPI_XBOOT_OFFSET+offset+32+(rsize*i));
		memcpy32(mem, src, rsize/4); //copy data 512 bytest
		tcpsum(0, 128, mem, 0);//checksum
		for (j=0; j<128; j++) {
			/*****write register *********/
			word16 = mem[j]&0xFFFF;
			dwc_ddrphy_apb_wr(MEM_ADDR+ (256*i)+(j*2), word16);
			//printf_offset_value((256*i)+(j*2),word16);
			word16 = (mem[j]>>16)&0xFFFF;
			dwc_ddrphy_apb_wr(MEM_ADDR+ (256*i)+(j*2)+1, word16);
			//printf_offset_value((256*i)+(j*2)+1,word16);
		}
	}

	for (addr=0; addr<mem_size; addr++) {
		mem[addr]=0;
	}

	unsigned int *src =  (unsigned int*)(SPI_FLASH_BASE + SPI_XBOOT_OFFSET+offset+32+(rsize*num0));
	memcpy32(mem, src, num1/4);//copy data
	tcpsum(0,num1/4, mem, 1);//checksum
	for (j=0; j<(num1/4); j++) {
		/*****write register *********/
		word16 = mem[j]&0xFFFF;
		dwc_ddrphy_apb_wr(MEM_ADDR+ (256*num0)+(j*2), word16);
		//printf_offset_value((256*num0)+(j*2),word16);
		word16 = (mem[j]>>16)&0xFFFF;
		dwc_ddrphy_apb_wr(MEM_ADDR+ (256*num0)+(j*2)+1, word16);
		//printf_offset_value((256*num0)+(j*2)+1,word16);
	}

	DwcCheckSum(xhdr->magic, xhdr->checksum);
	sum = 0;
}

int ReadSector(unsigned int sectorNo, unsigned int pageCount, unsigned int *ptrPyldData)
{
	if (bootdevice == EMMC_BOOT)
		return ReadSDSector(sectorNo+xboot_start_secotr, pageCount, ptrPyldData);
	else if (bootdevice == SDCARD_ISP)
		return ReadSDSector(lba2sec_for_training_fw(sectorNo), pageCount, ptrPyldData);
#ifdef CONFIG_HAVE_SNPS_USB3_DISK
	else if (g_bootinfo.bootdev_port == USB3_PORT)
		return usb_readSector(lba2sec_for_training_fw(sectorNo), pageCount, ptrPyldData);
#endif
#ifdef CONFIG_HAVE_USB_DISK
	else if (g_bootinfo.bootdev_port == USB2_PORT)
		return usb2_readSector(lba2sec_for_training_fw(sectorNo), pageCount, ptrPyldData);
#endif
	else
		return -1;
}

void LoadBinCodeForSectorMode(unsigned char Train2D, unsigned int offset, unsigned int MEM_ADDR)
{
	unsigned short i, j, addr, word16;
	unsigned short mem_offset, img_length, cnt;
	unsigned int img_name, img_sum;
	unsigned short last_img_name_array_cnt, last_img_length_array_cnt;

	i = 0;
	mem_offset = 0;
	img_name = 0;
	img_length = 0;
	img_sum = 0;
	last_img_name_array_cnt = 0;
	last_img_length_array_cnt = 0;

	if ((bootdevice == SDCARD_ISP) || (bootdevice == USB_ISP)) {
		fat_read_fat_for_training_fw(&g_finfo, g_io_buf.usb.sect_buf, offset);
		offset = 0;
	}

	//Read first block
	for (addr = 0; addr < mem_size; addr++)
		mem[addr] = 0;

	ReadSector(offset, 1, mem);

	for (j = 0; j < 128; j++) {
		if ((mem[j] == IM1D_HDR_MAGIC) || (mem[j] == DM1D_HDR_MAGIC)
			|| (mem[j] == IM2D_HDR_MAGIC) || (mem[j] == DM2D_HDR_MAGIC)
			|| (mem[j] == IMDA_HDR_MAGIC) || (mem[j] == DMDA_HDR_MAGIC)) {

			//prn_string("j="); prn_dword(j);
			//prn_string("mem[j]="); prn_dword(mem[j]);
			img_name = mem[j];
			if ((j == 126) || (j == 127)) {
				last_img_name_array_cnt = j;
				break;
			}

			img_length = mem[j + 2];
			//prn_string("img_length="); prn_dword(img_length);

			if (img_name == IM1D_HDR_MAGIC)
				IMEM1d_len = img_length;
			else if(img_name == DM1D_HDR_MAGIC)
				DMEM1d_len = img_length;
			else if(img_name == IM2D_HDR_MAGIC)
				IMEM2d_len = img_length;
			else if(img_name == DM2D_HDR_MAGIC)
				DMEM2d_len = img_length;
			else if(img_name == IMDA_HDR_MAGIC)
				IMDA_len = img_length;
			else if(img_name == DMDA_HDR_MAGIC)
				DMDA_len = img_length;

			if (j == 125) {
				last_img_length_array_cnt = 127;
				break;
			}

			img_sum = mem[j + 3];
			//prn_string("img_sum="); prn_dword(img_sum);

			if (j >= 120) {
				// Header locates at the lastest 8 words of a sector.
				for (addr = 0; addr < mem_size; addr++)
					mem[addr] = 0;

				offset++;
				ReadSector(offset, 1, mem);

				i = j - 120;
			} else {
				i = j + 8;
			}
			tcpsum(i, 128, mem, 0); //checksum

			for (; i < 128; i++) {
				/*****write register *********/
				word16 = mem[i] & 0xFFFF;
				dwc_ddrphy_apb_wr(MEM_ADDR+mem_offset, word16);
				//printf_offset_value(mem_offset, word16);
				mem_offset++;

				word16 = (mem[i] >> 16) & 0xFFFF;
				dwc_ddrphy_apb_wr(MEM_ADDR+mem_offset, word16);
				//printf_offset_value(mem_offset, word16);
				mem_offset++;
				img_length -= 4;
			}
			break;
		}
	}
	if (j == 128) {
		prn_string("Missing f/w header!\n");
		while (1);
	}

	if ((last_img_name_array_cnt == 126) || (last_img_name_array_cnt == 127)
		|| (last_img_length_array_cnt == 127)) {

		for (addr = 0; addr < mem_size; addr++)
			mem[addr] = 0;

		offset++;
		ReadSector(offset, 1, mem);

		if (last_img_name_array_cnt == 126) {
			img_length = mem[0];
			img_sum = mem[1];
			i = 6;
		} else if (last_img_name_array_cnt == 127) {
			img_length = mem[1];
			img_sum = mem[2];
			i = 7;
		} else if (last_img_length_array_cnt == 127) {
			img_sum = mem[0];
			i = 5;
		}

		if ((last_img_name_array_cnt == 126) || (last_img_name_array_cnt == 127)) {
			if (img_name == IM1D_HDR_MAGIC)
				IMEM1d_len = img_length;
			else if (img_name == DM1D_HDR_MAGIC)
				DMEM1d_len = img_length;
			else if (img_name == IM2D_HDR_MAGIC)
				IMEM2d_len = img_length;
			else if (img_name == DM2D_HDR_MAGIC)
				DMEM2d_len = img_length;
			else if (img_name == IMDA_HDR_MAGIC)
				IMDA_len = img_length;
			else if (img_name == DMDA_HDR_MAGIC)
				DMDA_len = img_length;
		}

		tcpsum(i, 128, mem, 0); //checksum

		for (; i < 128; i++) {
			/*****write register *********/
			word16 = mem[i] & 0xFFFF;
			dwc_ddrphy_apb_wr(MEM_ADDR+mem_offset, word16);
			//printf_offset_value(mem_offset, word16);
			mem_offset++;

			word16 = (mem[i] >> 16) & 0xFFFF;
			dwc_ddrphy_apb_wr(MEM_ADDR+mem_offset, word16);
			//printf_offset_value(mem_offset, word16);
			mem_offset++;
			img_length -= 4;
		}
	}

	//Read middle block
	while (img_length > 512) {
		for (addr = 0; addr < mem_size; addr++)
			mem[addr] = 0;

		offset++;
		ReadSector(offset, 1, mem);

		tcpsum(0, 128, mem, 0); //checksum

		for (j = 0; j < 128; j++) {
			/*****write register *********/
			word16 = mem[j] & 0xFFFF;
			dwc_ddrphy_apb_wr(MEM_ADDR+mem_offset, word16);
			//printf_offset_value(mem_offset, word16);
			mem_offset++;

			word16 = (mem[j] >> 16) & 0xFFFF;
			dwc_ddrphy_apb_wr(MEM_ADDR+mem_offset, word16);
			//printf_offset_value(mem_offset, word16);
			mem_offset++;
		}
		img_length -= 512;
		//prn_string("img_length"); prn_dword(img_length);
	}

	//Read last block
	for (addr = 0; addr < mem_size; addr++)
		mem[addr] = 0;

	offset++;
	ReadSector(offset, 1, mem);

	cnt = img_length / 4;
	tcpsum(0, cnt, mem, 1); //checksum

	//prn_string("cnt="); prn_dword(cnt);
	for (j = 0; j < cnt; j++){
		/*****write register *********/
		word16 = mem[j] & 0xFFFF;
		dwc_ddrphy_apb_wr(MEM_ADDR+mem_offset, word16);
		//printf_offset_value(mem_offset, word16);
		mem_offset++;

		word16 = (mem[j] >> 16) & 0xFFFF;
		dwc_ddrphy_apb_wr(MEM_ADDR+mem_offset, word16);
		//printf_offset_value(mem_offset, word16);
		mem_offset++;
	}

	DwcCheckSum(img_name, img_sum);
	sum = 0;
}

#if defined(CONFIG_HAVE_SPI_NAND) || defined(CONFIG_HAVE_PARA_NAND)
void LoadBinForNAND(int Train2D, int mem_type, u32 pg_off, UINT32 *buf)
{
	u32 read_bytes = 0;
	int sz_sect = GetNANDPageCount_1K60(g_bootinfo.sys_nand.u16PyldLen) * 1024;
	int res = 2;
	u32 mem_addr = 0, mem_offset = 0;
	u32 img_name = 0, img_length = 0, img_sum = 0;
	u32 i, j, cnt, word16;

	//prn_string("LoadBinForNAND\n");

	//Consider the case that multiple image in one page
	if(pg_off == pg_off_prev)
		chk_multi_img_in_same_page = 1;
	else
		chk_multi_img_in_same_page = 0;
	pg_off_prev = pg_off;

	sum = 0;    // Reset DWC checksum
	cnt = sz_sect / 4;
	mem_addr = (mem_type == 0)? IMEM_ADDR : DMEM_ADDR;

	/* Read first page of BIN file */
	//prn_string("Read first page\n");
	prn_string("1st pg_off="); prn_decimal(pg_off); prn_string("\n");
	if (bootdevice == SPINAND_BOOT) {
#ifdef CONFIG_HAVE_SPI_NAND
		res = SPINANDReadNANDPage_1K60(NAND_CS0, pg_off, (u32 *)buf, &read_bytes);
#endif
	} else {// PARA NAND
#ifdef CONFIG_HAVE_PARA_NAND
		res = PNANDReadNANDPage_1K60(pg_off, (u32 *)buf, &read_bytes);
#endif
	}
	if (res != ROM_SUCCESS) {
		prn_string("failed at pg="); prn_dword(pg_off);
		//break;
	}

	// Search BIN header
	//prn_string("Search BIN header\n");
	for (i = 0; i < cnt; i++) {
		if ((buf[i] == IM1D_HDR_MAGIC) || (buf[i] == DM1D_HDR_MAGIC) ||
			(buf[i] == IM2D_HDR_MAGIC) || (buf[i] == DM2D_HDR_MAGIC)) {
			prn_string("Found BIN at i="); prn_dword(i);

			img_name   = buf[i];
			img_length = buf[i+2];
			img_sum    = buf[i+3];
			prn_string("img_name="); prn_dword(img_name);

			// Check if BIN image is correct
			if (((Train2D == 0) && (mem_type == 0) && (img_name != IM1D_HDR_MAGIC)) ||
				((Train2D == 0) && (mem_type == 1) && (img_name != DM1D_HDR_MAGIC)) ||
				((Train2D == 1) && (mem_type == 0) && (img_name != IM2D_HDR_MAGIC)) ||
				((Train2D == 1) && (mem_type == 1) && (img_name != DM2D_HDR_MAGIC))) {
				//Consider the case that multiple image in one page
				if(chk_multi_img_in_same_page == 1) {
					continue;
				} else {
					prn_string("Wrong image! img_name="); prn_dword(img_name);
				}
			}

			// Check first data position
			j = i + 8;
			//prn_string("j="); prn_dword(j);
			if (j >= cnt) {
				// The first page of BIN files spans two NAND pages
				j = j - cnt;
				pg_off++;
				//prn_string("pg_off="); prn_decimal(pg_off); prn_string("\n");
				if (bootdevice == SPINAND_BOOT) {
#ifdef CONFIG_HAVE_SPI_NAND
					res = SPINANDReadNANDPage_1K60(NAND_CS0, pg_off, (u32 *)buf, &read_bytes);
#endif
				} else {// PARA NAND
#ifdef CONFIG_HAVE_PARA_NAND
					res = PNANDReadNANDPage_1K60(pg_off, (u32 *)buf, &read_bytes);
#endif
				}
				if (res != ROM_SUCCESS) {
					prn_string("failed at pg="); prn_dword(pg_off);
					//break;
				}

				// Get BIN checksum and length again
				if (j >= 5) {
					img_sum = buf[j-5];
				}
				if (j >= 6) {
					img_length = buf[j-6];
				}
			}
			prn_string("img_length="); prn_dword(img_length);
			prn_string("img_sum="); prn_dword(img_sum);

			//Consider the case that (img_start_pos + img_length) < page_size
			if(j + img_length / 4 < cnt)
				i = j + img_length / 4;
			else
				i = cnt;
			tcpsum(j, i, buf, 0);

			// Store BIN file length. Add it to header length, 32 bytes.
			if (img_name == IM1D_HDR_MAGIC)
				IMEM1d_len = img_length + 32;
			else if (img_name == DM1D_HDR_MAGIC)
				DMEM1d_len = img_length + 32;
			else if (img_name == IM2D_HDR_MAGIC)
				IMEM2d_len = img_length + 32;
			else if (img_name == DM2D_HDR_MAGIC)
				DMEM2d_len = img_length + 32;

			// Write DWC PHY register
			//prn_string("Write DWC PHY register\n");
			//prn_string("mem_addr="); prn_dword(mem_addr);
			for (; j < cnt; j++) {
				//prn_string("j="); prn_dword(j);
				word16 = buf[j]&0xFFFF;
				dwc_ddrphy_apb_wr(mem_addr+mem_offset, word16);
				//printf_offset_value(mem_offset,word16);
				mem_offset++;
				word16 = (buf[j]>>16)&0xFFFF;
				dwc_ddrphy_apb_wr(mem_addr+mem_offset, word16);
				//printf_offset_value(mem_offset,word16);
				mem_offset++;
				img_length -= 4;
				if(img_length == 0)
					break;
			}
			//prn_string("img_length="); prn_dword(img_length);
			break;
		}
	}

	/* Read middle pages of BIN file */
	//prn_string("Read middle pages\n");
	while(img_length > sz_sect) {
		pg_off++;
		//prn_string("pg_off="); prn_decimal(pg_off); prn_string("\n");
		if (bootdevice == SPINAND_BOOT) {
#ifdef CONFIG_HAVE_SPI_NAND
			res = SPINANDReadNANDPage_1K60(NAND_CS0, pg_off, (u32 *)buf, &read_bytes);
#endif
		} else {// PARA NAND
#ifdef CONFIG_HAVE_PARA_NAND
			res = PNANDReadNANDPage_1K60(pg_off, (u32 *)buf, &read_bytes);
#endif
		}
		if (res != ROM_SUCCESS) {
			prn_string("failed at pg="); prn_dword(pg_off);
			//break;
		}
		tcpsum(0, cnt, buf, 0);

		// Write DWC PHY register
		//prn_string("Write DWC PHY register\n");
		for (j = 0; j < cnt; j++) {
			//prn_string("j="); prn_dword(j);
			word16 = buf[j]&0xFFFF;
			dwc_ddrphy_apb_wr(mem_addr+mem_offset, word16);
			//printf_offset_value(mem_offset,word16);
			mem_offset++;
			word16 = (buf[j]>>16)&0xFFFF;
			dwc_ddrphy_apb_wr(mem_addr+mem_offset, word16);
			//printf_offset_value(mem_offset,word16);
			mem_offset++;
			img_length -= 4;
		}
		//prn_string("img_length ="); prn_dword(img_length);
	}

	/* Read last page of BIN file */
	//prn_string("Read last page\n");
	pg_off++;
	//prn_string("pg_off="); prn_decimal(pg_off); prn_string("\n");
	if (bootdevice == SPINAND_BOOT) {
#ifdef CONFIG_HAVE_SPI_NAND
		res = SPINANDReadNANDPage_1K60(NAND_CS0, pg_off, (u32 *)buf, &read_bytes);
#endif
	} else {// PARA NAND
#ifdef CONFIG_HAVE_PARA_NAND
		res = PNANDReadNANDPage_1K60(pg_off, (u32 *)buf, &read_bytes);
#endif
	}
	if (res != ROM_SUCCESS) {
		prn_string("failed at pg="); prn_dword(pg_off);
		//break;
	}
	i = img_length / 4;
	tcpsum(0, i, buf, 1);
	//prn_string("i="); prn_dword(i);

	// Write DWC PHY register
	//prn_string("Write DWC PHY register\n");
	for (j = 0; j < i; j++) {
		//prn_string("j="); prn_dword(j);
		word16 = buf[j] & 0xFFFF;
		dwc_ddrphy_apb_wr(mem_addr+mem_offset, word16);
		//printf_offset_value(mem_offset,word16);
		mem_offset++;
		word16 = (buf[j] >> 16) & 0xFFFF;
		dwc_ddrphy_apb_wr(mem_addr+mem_offset, word16);
		//printf_offset_value(mem_offset,word16);
		mem_offset++;
		img_length -= 4;
	}

	DwcCheckSum(img_name, img_sum);
}

void LoadMEMForNAND(int Train2D, int mem_type)
{
	struct BootProfileHeader *ptr = Get_Header_Profile_Ptr();
	u32 read_bytes = 0, pg_off = 0, pg_cnt = 0;
	u8 *buf = g_io_buf.nand.data;
	int res = 2;
	int sz_sect = GetNANDPageCount_1K60(g_bootinfo.sys_nand.u16PyldLen) * 1024;
	int xbsize;

	//prn_string("LoadMEMForNAND\n");
	prn_string("Train2D="); prn_decimal(Train2D); prn_string("; ");
	prn_string("mem_type="); prn_decimal(mem_type); prn_string("\n");

	// Get NAND infor from Boot Profile header
	pg_off = ptr->xboot_pg_off; 	// Xboot offset
	//prn_string("pg_off="); prn_dword(pg_off);
	//prn_string("sz_sect="); prn_dword(sz_sect);

	if (XBOOT_len == 0) {
		// Get Xboot length from NAND
		//prn_string("Get Xboot length from NAND\n");
		if (bootdevice == SPINAND_BOOT) {
#ifdef CONFIG_HAVE_SPI_NAND
			res = SPINANDReadNANDPage_1K60(NAND_CS0, pg_off, (u32 *)buf, &read_bytes);
#endif
		} else {// PARA NAND
#ifdef CONFIG_HAVE_PARA_NAND
			res = PNANDReadNANDPage_1K60(pg_off, (u32 *)buf, &read_bytes);
#endif
		}
		if (res != ROM_SUCCESS) {
			prn_string("failed at pg="); prn_dword(pg_off);
			//break;
		}

		xbsize = get_xboot_size(buf);
		XBOOT_len = xbsize;
	}
	//prn_string("XBOOT_len="); prn_dword(XBOOT_len);

	if (mem_type == 0) {
		if (Train2D == 0) {
			pg_cnt = XBOOT_len;
		} else {
			//prn_string("DMEM1d_len="); prn_dword(DMEM1d_len);
			pg_cnt = XBOOT_len + IMEM1d_len + DMEM1d_len;
		}
	} else {
		if (Train2D == 0) {
			//prn_string("IMEM1d_len="); prn_dword(IMEM1d_len);
			pg_cnt = XBOOT_len + IMEM1d_len;
		} else {
			//prn_string("IMEM2d_len="); prn_dword(IMEM2d_len);
			pg_cnt = XBOOT_len + IMEM1d_len + DMEM1d_len + IMEM2d_len;
		}
	}
	pg_cnt /= sz_sect;
	pg_off = pg_off + pg_cnt;

	LoadBinForNAND(Train2D, mem_type, pg_off, (u32 *)buf);
}
#endif

/*
 * 1D training:Train2D=0
 * 2D training:Train2D=1
 * Diagnostic:Train2D=2
 */
void dwc_ddrphy_phyinit_D_loadIMEM_of_SP(int Train2D)
{
	mp = 0;
	prn_string("bootdevice:");
	prn_dword(bootdevice);
	if (bootdevice == SPI_NOR_BOOT)
	{
		struct xboot_hdr *xhdr = (struct xboot_hdr*)(SPI_FLASH_BASE + SPI_XBOOT_OFFSET);//xboot start addr
		unsigned int offset;
		offset = sizeof(struct xboot_hdr) + xhdr->length; //xboot length
		xhdr = (struct xboot_hdr*)(SPI_FLASH_BASE + SPI_XBOOT_OFFSET + offset);
		if (Train2D == 0)
			LoadBinCode(0,offset,IMEM_ADDR);
		offset = offset + sizeof(struct xboot_hdr) + xhdr->length;//xboot+im1d  length
		xhdr = (struct xboot_hdr*)(SPI_FLASH_BASE + SPI_XBOOT_OFFSET + offset);
		offset = offset + sizeof(struct xboot_hdr) + xhdr->length;//xboot+im1d+dm1d  length
		xhdr = (struct xboot_hdr*)(SPI_FLASH_BASE + SPI_XBOOT_OFFSET + offset);
		if (Train2D == 1)
			LoadBinCode(1,offset,IMEM_ADDR);
		offset = offset + sizeof(struct xboot_hdr) + xhdr->length;//xboot+im1d+dm1d+im2d  length
		xhdr = (struct xboot_hdr*)(SPI_FLASH_BASE + SPI_XBOOT_OFFSET + offset);
		offset = offset + sizeof(struct xboot_hdr) + xhdr->length;//xboot+im1d+dm1d+im2d+dm2d  length
		xhdr = (struct xboot_hdr*)(SPI_FLASH_BASE + SPI_XBOOT_OFFSET + offset);
		if (Train2D == 2)
			LoadBinCode(2,offset,IMEM_ADDR);
	}
	else if ((bootdevice == EMMC_BOOT) || (bootdevice == SDCARD_ISP) || (bootdevice == USB_ISP))
	{
		unsigned int sectorNo0, total_length, addr;
		u8 buf[64];
		struct xboot_hdr *xhdr = (struct xboot_hdr *)buf;
		int ret, value;

		if (bootdevice == EMMC_BOOT)
		{
			for (addr = 0; addr < mem_size; addr++)
				mem[addr]=0;
			ReadSDSector(0, 1, mem);
			XBOOT_len = mem[2];
		}
		else if ((bootdevice == SDCARD_ISP) || (bootdevice == USB_ISP))
		{
			ret = fat_read_file(0, &g_finfo, g_io_buf.usb.sect_buf, 0, 32, buf); //for get xboot's sector No
			if (ret == FAIL) {
				prn_string("load xboot hdr failed\n");
				return;
			}
			//prn_string("xboot len=");
			//prn_dword(32 + xhdr->length);
			XBOOT_len = xhdr->length;
		}
		if (Train2D == 0)
		{
			prn_string("XBOOT_len="); prn_dword(XBOOT_len);
			total_length = 32 + XBOOT_len + 32; //xboot header lenght + xboot length + IMEM header length
			sectorNo0 = total_length / 512;
			value = total_length % 512;
			//prn_string("sectorNo0="); prn_dword(sectorNo0);
			//prn_string("value="); prn_dword(value);
			if(value < 0x20)
			{
				//prn_string("value < 0x20\n");
				LoadBinCodeForSectorMode(0, sectorNo0-1, IMEM_ADDR);
			}
			else
			{
				LoadBinCodeForSectorMode(0, sectorNo0, IMEM_ADDR);
			}
		}
		else if (Train2D == 1)
		{
			total_length = 32 + XBOOT_len + 32 + IMEM1d_len + 32 + DMEM1d_len + 32; //xboot header lenght + xboot length + IMEM header length
			sectorNo0 = total_length / 512;
			value = total_length % 512;
			//prn_string("sectorNo0="); prn_dword(sectorNo0);
			//prn_string("value="); prn_dword(value);
			if(value < 0x20)
			{
				//prn_string("value < 0x20\n");
				LoadBinCodeForSectorMode(0, sectorNo0-1, IMEM_ADDR);
			}
			else
			{
				LoadBinCodeForSectorMode(0, sectorNo0, IMEM_ADDR);
			}
		}
		else if (Train2D == 2)
		{
			total_length = 32 + XBOOT_len + 32 + IMEM1d_len + 32 + DMEM1d_len + 32 + IMEM2d_len + 32 + DMEM2d_len + 32; //xboot header lenght + xboot length + IMEM header length
			sectorNo0 = total_length / 512;
			value = total_length % 512;
			//prn_string("sectorNo0="); prn_dword(sectorNo0);
			//prn_string("value="); prn_dword(value);
			if(value < 0x20)
			{
				//prn_string("value < 0x20\n");
				LoadBinCodeForSectorMode(0, sectorNo0-1, IMEM_ADDR);
			}
			else
			{
				LoadBinCodeForSectorMode(0, sectorNo0, IMEM_ADDR);
			}
		}
	}
#ifdef CONFIG_HAVE_SPI_NAND
	else if (bootdevice == SPINAND_BOOT)
	{
		LoadMEMForNAND(Train2D, 0);
	}
#endif
#ifdef CONFIG_HAVE_PARA_NAND
	else if (bootdevice == PARA_NAND_BOOT)
	{
		LoadMEMForNAND(Train2D, 0);
	}
#endif
	mp = 1;
}

void dwc_ddrphy_phyinit_F_loadDMEM_of_SP(int pstate, int Train2D)
{
	mp = 0;
	if(bootdevice == SPI_NOR_BOOT)
	{
		struct xboot_hdr *xhdr = (struct xboot_hdr*)(SPI_FLASH_BASE + SPI_XBOOT_OFFSET);//xboot start addr
		unsigned int offset;
		offset = sizeof(struct xboot_hdr) + xhdr->length; //xboot length
		xhdr = (struct xboot_hdr*)(SPI_FLASH_BASE + SPI_XBOOT_OFFSET + offset);
		offset = offset + sizeof(struct xboot_hdr) + xhdr->length;//xboot+im1d  length
		xhdr = (struct xboot_hdr*)(SPI_FLASH_BASE + SPI_XBOOT_OFFSET + offset);
		if (Train2D == 0)
			LoadBinCode(0,offset,DMEM_ADDR);
		offset = offset + sizeof(struct xboot_hdr) + xhdr->length;//xboot+im1d+dm1d  length
		xhdr = (struct xboot_hdr*)(SPI_FLASH_BASE + SPI_XBOOT_OFFSET + offset);
		offset = offset + sizeof(struct xboot_hdr) + xhdr->length;//xboot+im1d+dm1d+im2d  length
		xhdr = (struct xboot_hdr*)(SPI_FLASH_BASE + SPI_XBOOT_OFFSET + offset);
		if (Train2D == 1)
			LoadBinCode(1,offset,DMEM_ADDR);
		offset = offset + sizeof(struct xboot_hdr) + xhdr->length;//xboot+im1d+dm1d+im2d+dm2d  length
		xhdr = (struct xboot_hdr*)(SPI_FLASH_BASE + SPI_XBOOT_OFFSET + offset);
		offset = offset + sizeof(struct xboot_hdr) + xhdr->length;//xboot+im1d+dm1d+im2d+dm2d+imda  length
		xhdr = (struct xboot_hdr*)(SPI_FLASH_BASE + SPI_XBOOT_OFFSET + offset);
		if (Train2D == 2)
			LoadBinCode(2,offset,DIA_DMEM_ADDR);
	}
	else if((bootdevice == EMMC_BOOT) || (bootdevice == SDCARD_ISP) || (bootdevice == USB_ISP))
	{
		int sectorNo0, total_length, value;
		if (Train2D == 0)
		{
			//prn_string("IMEM1d_len="); prn_dword(IMEM1d_len);
			total_length = 32 + XBOOT_len + 32 + IMEM1d_len +32 ; //xboot header lenght + xboot length + IMEM header length
			sectorNo0 = total_length / 512;
			value = total_length % 512;
			//prn_string("sectorNo0="); prn_dword(sectorNo0);
			//prn_string("value="); prn_dword(value);
			if(value < 0x20)
			{
				//prn_string("value < 0x20\n");
				LoadBinCodeForSectorMode(0, sectorNo0-1, DMEM_ADDR);
			}
			else
			{
				LoadBinCodeForSectorMode(0, sectorNo0, DMEM_ADDR);
			}
		}
		else if(Train2D == 1)
		{
			total_length = 32 + XBOOT_len + 32 + IMEM1d_len + 32 + DMEM1d_len + 32 + IMEM2d_len + 32; //xboot header lenght + xboot length + IMEM header length
			sectorNo0 = total_length / 512;
			value = total_length % 512;
			//prn_string("sectorNo0="); prn_dword(sectorNo0);
			//prn_string("value="); prn_dword(value);
			if(value < 0x20)
			{
				//prn_string("value < 0x20\n");
				LoadBinCodeForSectorMode(0, sectorNo0-1, DMEM_ADDR);
			}
			else
			{
				LoadBinCodeForSectorMode(0, sectorNo0, DMEM_ADDR);
			}
		}
		else if(Train2D == 2)
		{
			total_length = 32 + XBOOT_len + 32 + IMEM1d_len + 32 + DMEM1d_len + 32 + IMEM2d_len + 32 + DMEM2d_len + 32 + IMDA_len + 32; //xboot header lenght + xboot length + IMEM header length
			sectorNo0 = total_length / 512;
			value = total_length % 512;
			//prn_string("sectorNo0="); prn_dword(sectorNo0);
			//prn_string("value="); prn_dword(value);
			if(value < 0x20)
			{
				//prn_string("value < 0x20\n");
				LoadBinCodeForSectorMode(0, sectorNo0-1, DMEM_ADDR);
			}
			else
			{
				LoadBinCodeForSectorMode(0, sectorNo0, DMEM_ADDR);
			}
		}
	}
#ifdef CONFIG_HAVE_SPI_NAND
	else if (bootdevice == SPINAND_BOOT)
	{
		LoadMEMForNAND(Train2D, 1);
	}
#endif
#ifdef CONFIG_HAVE_PARA_NAND
	else if (bootdevice == PARA_NAND_BOOT)
	{
		LoadMEMForNAND(Train2D, 1);
	}
#endif
	mp = 1;
}

void dwc_ddrphy_phyinit_saveRetention(void)
{
	volatile unsigned int *addr;
	unsigned int *beg  = (unsigned int *)ADDRESS_CONVERT(0xFA280000);
	prn_string("save retention value: ");
	prn_dword0((unsigned int)ADDRESS_CONVERT(0xFA280000));
	prn_string(" - ");
	prn_dword((unsigned int)ADDRESS_CONVERT(0xFA290000));

	int regIndx=0;
	addr = beg;
	for (regIndx = 0; regIndx < NumRegSaved; regIndx++)
	{
		prn_string("regIndx: ");
		prn_dword0(regIndx);
		prn_string("; Address: ");
		prn_dword0((unsigned int)RetRegList[regIndx].Address);
		*addr = (unsigned int) RetRegList[regIndx].Address;
		addr++;
		prn_string("; Value: ");
		prn_dword0(RetRegList[regIndx].Value);
		prn_string("\n");
		*addr = RetRegList[regIndx].Value;
		addr++;
	}
}

void dwc_ddrphy_phyinit_restoreRetention(void)
{
	volatile unsigned int *addr;
	unsigned int *beg  = (unsigned int *)ADDRESS_CONVERT(0xFA280000);
	unsigned int phyadr, val;
	prn_string("restore retention value: ");
	prn_dword0((unsigned int)ADDRESS_CONVERT(0xFA280000));
	prn_string(" - ");
	prn_dword((unsigned int)ADDRESS_CONVERT(0xFA290000));

	int regIndx=0;
	addr = beg;

	dwc_ddrphy_apb_wr(0xd0000, 0x00);
	dwc_ddrphy_apb_wr(0xc0080, 0x03);
	for (regIndx = 0; regIndx < NumRegSaved; regIndx++)
	{
	    phyadr = *addr;
		prn_string("regIndx: ");
		prn_dword0(regIndx);
		prn_string("; Address: ");
		prn_dword0(phyadr);
		addr++;

	    val = *addr;
		prn_string("; Value: ");
		prn_dword0(val);
		prn_string("\n");
		addr++;
		dwc_ddrphy_apb_wr(phyadr, val);
	}
	dwc_ddrphy_apb_wr(0xc0080, 0x02);
	dwc_ddrphy_apb_wr(0xd0000, 0x01);
}


void dwc_ddrphy_phyinit_main(void)
{
	prn_string("dwc_ddrphy_phyinit_main 20230714\n");
	mp = 1;
	//runtimeConfig.RetEn = 1;

	#ifdef DRAM_TYPE_LPDDR4
	#ifdef SDRAM_SPEED_1600
	#ifdef NT6AN1024F32AV
		prn_string("dwc_ddrphy_phyinit_out_lpddr4_train1d2d_3200_SP7350_nayna\n");
		#include <SP7350/LPDDR4/dwc_ddrphy_phyinit_out_lpddr4_train1d2d_3200_SP7350_nayna.txt>
	#elif defined(MT53E1G32D2_A)
		prn_string("dwc_ddrphy_phyinit_out_lpddr4_train1d2d_33_3200_ASIC_RANK1_WDQS_RxTx2D_04_rdwr2D_3f3f_quick2d\n");
		#include <SP7350/LPDDR4/dwc_ddrphy_phyinit_out_lpddr4_train1d2d_33_3200_ASIC_RANK1_WDQS_RxTx2D_04_rdwr2D_3f3f_quick2d.txt>
	#else
		prn_string("dwc_ddrphy_phyinit_out_lpddr4_train1d2d_3200_SP7350\n");
		#include <SP7350/LPDDR4/dwc_ddrphy_phyinit_out_lpddr4_train1d2d_3200_SP7350.txt>
	#endif
	#endif

	#ifdef SDRAM_SPEED_1333
	prn_string("dwc_ddrphy_phyinit_out_lpddr4_train1d2d_2666_SP7350\n");
	#include <SP7350/LPDDR4/dwc_ddrphy_phyinit_out_lpddr4_train1d2d_2666_SP7350.txt>
	#endif

	#ifdef SDRAM_SPEED_1200
	prn_string("dwc_ddrphy_phyinit_out_lpddr4_train1d2d_2400_SP7350\n");
	#include <SP7350/LPDDR4/dwc_ddrphy_phyinit_out_lpddr4_train1d2d_2400_SP7350.txt>
	#endif

	#ifdef SDRAM_SPEED_1066
	prn_string("dwc_ddrphy_phyinit_out_lpddr4_train1d2d_2133_SP7350\n");
	#include <SP7350/LPDDR4/dwc_ddrphy_phyinit_out_lpddr4_train1d2d_2133_SP7350.txt>
	#endif

	#ifdef SDRAM_SPEED_933
	prn_string("dwc_ddrphy_phyinit_out_lpddr4_train1d2d_1866_SP7350\n");
	#include <SP7350/LPDDR4/dwc_ddrphy_phyinit_out_lpddr4_train1d2d_1866_SP7350.txt>
	#endif

	#ifdef SDRAM_SPEED_800
	prn_string("dwc_ddrphy_phyinit_out_lpddr4_train1d2d_1600_SP7350\n");
	#include <SP7350/LPDDR4/dwc_ddrphy_phyinit_out_lpddr4_train1d2d_1600_SP7350.txt>
	#endif

	#ifdef SDRAM_SPEED_733
	prn_string("dwc_ddrphy_phyinit_out_lpddr4_train1d2d_1466_SP7350\n");
	#include <SP7350/LPDDR4/dwc_ddrphy_phyinit_out_lpddr4_train1d2d_1466_SP7350.txt>
	#endif

	#ifdef SDRAM_SPEED_666
	prn_string("dwc_ddrphy_phyinit_out_lpddr4_train1d2d_1333_SP7350\n");
	#include <SP7350/LPDDR4/dwc_ddrphy_phyinit_out_lpddr4_train1d2d_1333_SP7350.txt>
	#endif

	#ifdef SDRAM_SPEED_600
	prn_string("dwc_ddrphy_phyinit_out_lpddr4_train1d2d_1200_SP7350\n");
	#include <SP7350/LPDDR4/dwc_ddrphy_phyinit_out_lpddr4_train1d2d_1200_SP7350.txt>
	#endif

	#ifdef SDRAM_SPEED_533
	prn_string("dwc_ddrphy_phyinit_out_lpddr4_train1d2d_1066_SP7350\n");
	#include <SP7350/LPDDR4/dwc_ddrphy_phyinit_out_lpddr4_train1d2d_1066_SP7350.txt>
	#endif

	#ifdef SDRAM_SPEED_400
	prn_string("dwc_ddrphy_phyinit_out_lpddr4_train1d2d_800_SP7350\n");
	#include <SP7350/LPDDR4/dwc_ddrphy_phyinit_out_lpddr4_train1d2d_800_SP7350.txt>
	#endif
	#endif

	#ifdef DRAM_TYPE_DDR4
	#ifdef SDRAM_SPEED_1600
	prn_string("dwc_ddrphy_phyinit_out_ddr4_3200_train1d2d_rank1\n");
	#include <SP7350/DDR4/dwc_ddrphy_phyinit_out_ddr4_3200_train1d2d_rank1.txt>
	#endif

	#ifdef SDRAM_SPEED_1333
	#ifdef CHANGE_CLOCK
	prn_string("dwc_ddrphy_phyinit_out_ddr4_2666_2133_1866_1600_train1d2d_rank1\n");
	#include <SP7350/DDR4/dwc_ddrphy_phyinit_out_ddr4_2666_2133_1866_1600_train1d2d_rank1.txt>
	#else
	prn_string("dwc_ddrphy_phyinit_out_ddr4_2666_train1d2d_rank1\n");
	#include <SP7350/DDR4/dwc_ddrphy_phyinit_out_ddr4_2666_train1d2d_rank1.txt>
	#endif
	#endif

	#ifdef SDRAM_SPEED_1066
	prn_string("dwc_ddrphy_phyinit_out_ddr4_2133_train1d2d_rank1_CL18_WL12\n");
	#include <SP7350/DDR4/dwc_ddrphy_phyinit_out_ddr4_2133_train1d2d_rank1_CL18_WL12.txt>
	#endif

	#ifdef SDRAM_SPEED_933
	prn_string("dwc_ddrphy_phyinit_out_ddr4_1866_train1d2d_rank1\n");
	#include <SP7350/DDR4/dwc_ddrphy_phyinit_out_ddr4_1866_train1d2d_rank1.txt>
	#endif

	#ifdef SDRAM_SPEED_800
	prn_string("dwc_ddrphy_phyinit_out_ddr4_1600_train1d2d_rank1_CL_14\n");
	#include <SP7350/DDR4/dwc_ddrphy_phyinit_out_ddr4_1600_train1d2d_rank1_CL_14.txt>
	#endif
	#endif

	#ifdef DRAM_TYPE_DDR3
	#ifdef SDRAM_SPEED_933
	#ifdef CHANGE_CLOCK
	prn_string("dwc_ddrphy_phyinit_out_ddr3_1866_1600_1333_1066_train1d_rank1\n");
	#include <SP7350/DDR3/dwc_ddrphy_phyinit_out_ddr3_1866_1600_1333_1066_train1d_rank1.txt>
	#else
	prn_string("dwc_ddrphy_phyinit_out_ddr3_1866_train1d\n");
	#include <SP7350/DDR3/dwc_ddrphy_phyinit_out_ddr3_1866_train1d.txt>
	#endif
	#endif

	#ifdef SDRAM_SPEED_800
	prn_string("dwc_ddrphy_phyinit_out_ddr3_1600_train1d\n");
	#include <SP7350/DDR3/dwc_ddrphy_phyinit_out_ddr3_1600_train1d.txt>
	#endif

	#ifdef SDRAM_SPEED_666
	prn_string("dwc_ddrphy_phyinit_out_ddr3_1333_train1d\n");
	#include <SP7350/DDR3/dwc_ddrphy_phyinit_out_ddr3_1333_train1d.txt>
	#endif

	#ifdef SDRAM_SPEED_533
	prn_string("dwc_ddrphy_phyinit_out_ddr3_1066_train1d_rank1\n");
	#include <SP7350/DDR3/dwc_ddrphy_phyinit_out_ddr3_1066_train1d_rank1.txt>
	#endif
	#endif

	#ifdef LCDL_testing
	dwc_ddrphy_LCDL_testing();
	#endif
}

// ***********************************************************************
// * FUNC      : startClockResetPhy_of_SP
// * PARAM     : dram_id
// * PURPOSE   :
// ***********************************************************************
void startClockResetPhy_of_SP(void)
{
	//prn_string("startClockResetPhy_of_SP");
	//prn_string("\n");

	//dbg_stamp(0xA000);
	wait_loop(1000);
	//MO3_REG->mo3_reserved[24] = 0x10001000; //PwrOKIn MO_DDRPHY_PWROKIN ddrphy pwrokin
	//SP_REG(0, 22) = RF_MASK_V_CLR(1 << 10);	// presetn MO_UMCTL2_RST_B APB BUS reset
	//SP_REG(0, 25) = RF_MASK_V_CLR(1 << 8);	// PRESETn_APB MO_DDRPHY_RST_B APB bus reset ; CLKDFI_DDRPHY_RST_B dfi_reset

} // end of startClockResetPhy_of_SP

void startClockResetUmctl2_of_SP(void)
{
	//prn_string("startClockResetUmctl2_of_SP");
	//prn_string("\n");

	dbg_stamp(0xA001);
	SP_REG_AO(0, 14) =0x00200020;
	SP_REG_AO(0, 14) =0x00010001;
	wait_loop(1000);
	SP_REG_AO(0, 14) =0x00080008;
	wait_loop(1000);

	dbg_stamp(0xA002);
	dwc_umctl2_init_before_ctl_rst();

	SP_REG_AO(0, 14) =0x00020002;
	SP_REG_AO(0, 14) =0x00040004;
	SP_REG_AO(0, 14) =0x00100010;

	wait_loop(1000);
	wait_loop(1000);
	dwc_umctl2_init_after_ctl_rst();
	//SP_REG(0, 25) = RF_MASK_V_CLR(1 << 8);	// PRESETn_APB MO_DDRPHY_RST_B APB bus reset ; CLKDFI_DDRPHY_RST_B dfi_reset
 	wait_loop(1000);
}

// ***********************************************************************
// * FUNC      : dram_init
// * PARAM     : dram_id
// ***********************************************************************
int dram_init(unsigned int dram_id)
{
	unsigned int max_init_fail_cnt = 15;
	unsigned int loop_time;
	unsigned int ret = 0;

	loop_time = 0;
	prn_string("dram_init\n");

	for (loop_time = 0; loop_time < max_init_fail_cnt; loop_time++) {
		startClockResetPhy_of_SP();
		startClockResetUmctl2_of_SP();
		dwc_ddrphy_phyinit_main();
		ret = ctl_trigger_init_and_wait_normal();

		if (ret == WAIT_FLAG_FAIL) {
			prn_string("WAIT_FLAG_FAIL!\n");
			return FAIL;
		} else if (ret == 0) {
			prn_string("loop_time = ");
			prn_decimal(loop_time);
			prn_string(" ***\n");
		}

		prn_string("dram_init_end\n");
		//dwc_ddrphy_phyinit_saveRetention();
		//dwc_ddrphy_phyinit_restoreRetention();
		return SUCCESS;
	} // end of for loop :: loop_time for initial & training time control

	return SUCCESS;
} // end dram_init

int dram_init_main(unsigned int gbootRom_boot_mode)
{
	bootdevice=gbootRom_boot_mode;
	prn_string("bootdevice=");
	prn_dword(bootdevice);
	// init params
	ckobd_training_flag = 1;

#if !(defined(DRAMSCAN) || defined(SISCOPE))
#ifdef DRAM_INIT_DEBUG
	mp = 0;
	prn_string("Built at " __DATE__ " " __TIME__ "\n");
#endif
	dram_init(0);


#else
	prn_string("Built at " __DATE__ " " __TIME__ "\n");

	while (1) {;}
#endif
	return 1;
}

int Change_freq_main(unsigned int gbootRom_boot_mode, unsigned int pstate)
{
	bootdevice=gbootRom_boot_mode;
	prn_string("bootdevice=");
	prn_dword(bootdevice);
	// init params
	ckobd_training_flag = 1;

	apb_ctl_before_change_clock(pstate);

	return 1;
}

int dwc_ddrphy_phyinit_userCustom_E_setDfiClk (int pstate /*!< Input Pstate indicating associated DfiClk Frequency*/) {
	#ifdef CHANGE_CLOCK
	prn_string("dwc_ddrphy_phyinit_userCustom_E_setDfiClk\n");

	SP_REG_AO(0, 2) =0x00200020;
	#ifdef DRAM_TYPE_LPDDR4
	if (pstate == 0) { //PLLD 800MHz
		prn_string("PLLD: 800MHz\n");
		SP_REG_AO(3, 11) =0xFFFF1008;
		SP_REG_AO(3, 12) =0xFFFFC0BE;
		SP_REG_AO(3, 13) =0xFFFF0107;
    }

    if (pstate == 1) { //PLLD 666MHz
		prn_string("PLLD: 666MHz\n");
		SP_REG_AO(3, 11) =0xFFFF0808;
		SP_REG_AO(3, 12) =0xFFFFC0BE;
		SP_REG_AO(3, 13) =0xFFFF0107;
	}

	if (pstate == 2) { //PLLD 466MHz
		prn_string("PLLD: 466MHz\n");
		SP_REG_AO(3, 11) =0xFFFF180A;
		SP_REG_AO(3, 12) =0xFFFFC0BC;
		SP_REG_AO(3, 13) =0xFFFF0107;
    }

    if (pstate == 3) { //PLLD 333MHz
		prn_string("PLLD: 333MHz\n");
		SP_REG_AO(3, 11) =0xFFFF080A;
		SP_REG_AO(3, 12) =0xFFFFC0BC;
		SP_REG_AO(3, 13) =0xFFFF0107;
	}
	#endif

	#ifdef DRAM_TYPE_DDR4
	if (pstate == 0) { //PLLD 666MHz
		prn_string("PLLD: 666MHz\n");
		SP_REG_AO(3, 11) =0xFFFF0808;
		SP_REG_AO(3, 12) =0xFFFFC0BE;
		SP_REG_AO(3, 13) =0xFFFF0107;
	}

    if (pstate == 1) { //PLLD 533MHz
		prn_string("PLLD: 533MHz\n");
		SP_REG_AO(3, 11) =0xFFFF200A;
		SP_REG_AO(3, 12) =0xFFFFC0BD;
		SP_REG_AO(3, 13) =0xFFFF0107;
	}

	if (pstate == 2) { //PLLD 466MHz
		prn_string("PLLD: 466MHz\n");
		SP_REG_AO(3, 11) =0xFFFF180A;
		SP_REG_AO(3, 12) =0xFFFFC0BC;
		SP_REG_AO(3, 13) =0xFFFF0107;
    }

    if (pstate == 3) { //PLLD 400MHz
		prn_string("PLLD: 400MHz\n");
		SP_REG_AO(3, 11) =0xFFFF100A;
		SP_REG_AO(3, 12) =0xFFFFC0BC;
		SP_REG_AO(3, 13) =0xFFFF0107;
	}
	#endif

	#ifdef DRAM_TYPE_DDR3
	if (pstate == 0) { //PLLD 466MHz
		prn_string("PLLD: 466MHz\n");
		SP_REG_AO(3, 11) =0xFFFF9812;
		SP_REG_AO(3, 12) =0xFFFFC0BF;
		SP_REG_AO(3, 13) =0xFFFF0107;
	}

    if (pstate == 1) { //PLLD 400MHz
		prn_string("PLLD: 400MHz\n");
		SP_REG_AO(3, 11) =0xFFFF100A;
		SP_REG_AO(3, 12) =0xFFFFC0BC;
		SP_REG_AO(3, 13) =0xFFFF0107;
	}

	if (pstate == 2) { //PLLD 333MHz
		prn_string("PLLD: 333MHz\n");
		SP_REG_AO(3, 11) =0xFFFF080A;
		SP_REG_AO(3, 12) =0xFFFFC0BC;
		SP_REG_AO(3, 13) =0xFFFF0107;
    }

    if (pstate == 3) { //PLLD 266MHz
		prn_string("PLLD: 266MHz\n");
		SP_REG_AO(3, 11) =0xFFFF2012;
		SP_REG_AO(3, 12) =0xFFFFC0BD;
		SP_REG_AO(3, 13) =0xFFFF0107;
	}
	#endif
	SP_REG_AO(0, 2) =0x00200000;
	#endif
    return (pstate);
}

#ifdef LCDL_testing
void dwc_ddrphy_LCDL_testing(void)
{
	dwc_ddrphy_apb_wr(0xd0000, 0x0);
	dwc_ddrphy_apb_wr(0x20084, 0x0);/*LcdlCalPhase[8:0] delay sel = [1:511]*/
	dwc_ddrphy_apb_wr(0x20085, 0x9);/*LcdlCalPhaseUpdate*/
	dwc_ddrphy_apb_wr(0x20085, 0x0);/*LcdlCalPhaseUpdate*/
	dwc_ddrphy_apb_wr(0x200d3, 0x4000);/*DlyTestCntDfiClkIV[15:0]*/
	dwc_ddrphy_apb_wr(0x200d0, 0x3);/*DlyTestCntInit*/
	dwc_ddrphy_apb_wr(0x200d0, 0x1);/*DlyTestCntInit*/

	dwc_ddrphy_apb_wr(0xd0000, 0x1);
	UINT16 rd_data;
	for(int i=1; i<512; i=i+17){
		dwc_ddrphy_apb_wr(0x20084, i);/*LcdlCalPhase[8:0] delay sel = [1:511]*/
		dwc_ddrphy_apb_wr(0x20085, 0x9);/*LcdlCalPhaseUpdate*/
		dwc_ddrphy_apb_wr(0x20085, 0x0);/*LcdlCalPhaseUpdate*/
		dwc_ddrphy_apb_wr(0x200d3, 0x4000);/*DlyTestCntDfiClkIV[15:0]*/
		dwc_ddrphy_apb_wr(0x200d0, 0x3);/*DlyTestCntInit*/
		dwc_ddrphy_apb_wr(0x200d0, 0x1);/*DlyTestCntInit*/
	}

	while(1){
		rd_data = dwc_ddrphy_apb_rd(0x200d4);/*DlyTestCntDfiClk[15:0] wait to zero*/
		if(rd_data == 0x0)
			break;
	}
	rd_data = dwc_ddrphy_apb_rd(0x200d5);/*DlyTestCntRing0scDb<$db>[15:0] read data*/
	rd_data = dwc_ddrphy_apb_rd(0x200d6);/*DlyTestCntRing0scDb<$db>[15:0] read data*/
	rd_data = dwc_ddrphy_apb_rd(0x200d7);/*DlyTestCntRing0scDb<$db>[15:0] read data*/
	rd_data = dwc_ddrphy_apb_rd(0x200d8);/*DlyTestCntRing0scDb<$db>[15:0] read data*/
	rd_data = dwc_ddrphy_apb_rd(0x200df);/*DlyTestCntRing0scAc[15:0] read data*/

	prn_string("dwc_ddrphy_LCDL_end\n");
	while (1);
}
#endif
