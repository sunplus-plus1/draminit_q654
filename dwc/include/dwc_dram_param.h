#include <auto_config.h>

#if defined(CONFIG_LPDDR4)
#define DRAM_TYPE_LPDDR4
#define SDRAM_SPEED_1600	//PLL 800MHz
#elif defined(CONFIG_DDR4)
#define DRAM_TYPE_DDR4
#define SDRAM_SPEED_1333	//PLL 666MHz
#elif defined(CONFIG_LPDDR3)
#define DRAM_TYPE_LPDDR3
#elif defined(CONFIG_DDR3)
#define DRAM_TYPE_DDR3
#define SDRAM_SPEED_933		//PLL 466.6MHz
#endif
//#define SDRAM_SPEED_1600	//PLL 800MHz
//#define SDRAM_SPEED_1333	//PLL 666MHz
//#define SDRAM_SPEED_1200	//PLL 600MHz
//#define SDRAM_SPEED_1066	//PLL 533.3MHz
//#define SDRAM_SPEED_933		//PLL 466.6MHz
//#define SDRAM_SPEED_800		//PLL 400MHz
//#define SDRAM_SPEED_733		//PLL 366.6MHz
//#define SDRAM_SPEED_666		//PLL 333.3MHz
//#define SDRAM_SPEED_600		//PLL 300MHz
//#define SDRAM_SPEED_533		//PLL 266.6MHz
//#define SDRAM_SPEED_400		//PLL 200MHz

//#define STREAM_MESSAGE
//#define CHANGE_CLOCK
//#define LCDL_testing
#define DRAM_X32
//#define DRAM_X16
#if defined(CONFIG_NT6AN1024F32AV)
#define NT6AN1024F32AV
#elif defined(CONFIG_MT53D1024M32D4)
#define MT53D1024M32D4 /*Micron 1Gbx32, 2 rank, FBGA code = D9WHT*/
#elif defined(CONFIG_MT53E1G32D2_A)
#define MT53E1G32D2_A  /*Micron 1Gbx32, 1 rank, FBGA code = D9ZQX*/
#elif defined(CONFIG_MT53E1G32D2_B)
#define MT53E1G32D2_B  /*Micron 1Gbx32, 2 rank, FBGA code = D8CJG*/
#elif defined(CONFIG_MT53E2G32D4_C)
#define MT53E2G32D4_C  /*Micron 2Gbx32, 2 rank, FBGA code = D8CJN*/
#elif defined(CONFIG_RS1G32LO4D2BDS)
#define RS1G32LO4D2BDS  /*Rayson 1Gbx32, 2 rank*/
#elif defined(CONFIG_HG4XD04G)
#define HG4XD04G  /*Hosinglobal 1Gbx32, 2 rank*/
#elif defined(CONFIG_BWMZCX32H2A)
#define BWMZCX32H2A  /*BIWIN 1Gbx32, 1 rank*/
#elif defined(CONFIG_MT40A512M16TB)
#define MT40A512M16TB  /*Micron 512Mx16, 1 rank, FBGA code = D8BPK*/
#elif defined(CONFIG_MT41K512M16VRP)
#define MT41K512M16VRP  /*Micron 512Mx16, 1 rank, FBGA code = D9ZWN*/
#elif defined(CONFIG_K4AAG165WA)
#define K4AAG165WA  /*SAMSUNG 1Gx16, 1 rank*/
#elif defined(CONFIG_K4B8G1646D)
#define K4B8G1646D  /*SAMSUNG 512Mx16, 2 rank*/
#endif

int dwc_umctl2_init_before_ctl_rst(void);
int dwc_umctl2_init_after_ctl_rst(void);
void polling_sw_cfg_done();
void dwc_ddrphy_phyinit_userCustom_G_waitFwDone ();
void run_customPostTrain();
int ctl_trigger_init_and_wait_normal();
void dwc_ddrphy_apb_wr(UINT32 adr, UINT32 dat);
UINT16 dwc_ddrphy_apb_rd(UINT32 adr);
int ReadSDSector(unsigned int sectorNo, unsigned int pageCount, unsigned int *ptrPyldData);
int usb_readSector(u32 lba, u32 count, u32 *dest);
int usb2_readSector(u32 lba, u32 count, u32 *dest);
void apb_ctl_before_change_clock(unsigned int ps);
int dwc_ddrphy_phyinit_userCustom_E_setDfiClk (int pstate);
