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
#if defined(CONFIG_NT6AN1024F32AV)
#define NT6AN1024F32AV
#elif defined(CONFIG_MT53D1024M32D4)
#define MT53D1024M32D4 /*Micron 1Gbx32, 2 rank, FBGA code = D9WHT*/
#elif defined(CONFIG_MT53E1G32D2_A)
#define MT53E1G32D2_A  /*Micron 1Gbx32, 1 rank, FBGA code = D9ZQX*/
#elif defined(CONFIG_MT53E1G32D2_B)
#define MT53E1G32D2_B  /*Micron 1Gbx32, 2 rank, FBGA code = D8CJG*/
#elif defined(CONFIG_MT40A512M16TB)
#define MT40A512M16TB  /*Micron 512Mx16, 1 rank, FBGA code = D8BPK*/
#elif defined(CONFIG_MT41K512M16VRP)
#define MT41K512M16VRP  /*Micron 512Mx16, 1 rank, FBGA code = D9ZWN*/
#elif defined(CONFIG_K4AAG165WA)
#define K4AAG165WA  /*SAMSUNG 1Gx16, 1 rank*/
#elif defined(CONFIG_K4B8G1646D)
#define K4B8G1646D  /*SAMSUNG 512Mx16, 2 rank*/
#endif

#define UMCTL2_0_1      2 //device_config: x16 device
#define UMCTL2_0_2      0 //frequency_ratio: 1:2 Mode
#define UMCTL2_0_3      8 //burst_rdwr: Burst length of 16
#define UMCTL2_0_4      0 //dll_off_mode
#define UMCTL2_0_5      1 //lpddr4
#define UMCTL2_0        ((UMCTL2_0_1)   << 30 |\
						 (3)   << 24 |\
						 (UMCTL2_0_2)   << 22 |\
                         (UMCTL2_0_3)   << 16 |\
                         (UMCTL2_0_4)   << 15 |\
                         (UMCTL2_0_5)   <<  5 )

#define UMCTL2_10       0x40000030

#define UMCTL2_14       0x0002fcec

#define UMCTL2_1C       0x5ff0a936

#define UMCTL2_20       0x00001202

#define UMCTL2_24       0x69293bb1

#define UMCTL2_2C       0x00000000

#define UMCTL2_30_1     1
#define UMCTL2_30_2     0x20
#define UMCTL2_30_3     0x120
#define UMCTL2_30(n)    ((n) << 0)



#define UMCTL2_34_1     10
#define UMCTL2_34_2     74
#define UMCTL2_34       ((UMCTL2_34_1)  << 16 |\
                         (UMCTL2_34_2)  <<  8 )

#define UMCTL2_38_1             41
#define UMCTL2_38_2             1
#define UMCTL2_38_3             0
#define UMCTL2_38               ((UMCTL2_38_1)  << 16 |\
                                 (UMCTL2_38_2)  <<  1 |\
                                 (UMCTL2_38_3)  <<  0 )

#define UMCTL2_50_1     8
#define UMCTL2_50_2     15
#define UMCTL2_50_3     6
#define UMCTL2_50_4     0
#define UMCTL2_50       ((UMCTL2_50_1)  << 20 |\
                         (UMCTL2_50_2)  << 12 |\
                         (UMCTL2_50_3)  <<  4 |\
                         (UMCTL2_50_4)  <<  2 )

#define UMCTL2_54		0x004a0015

#define UMCTL2_60_1     0
#define UMCTL2_60_2     0
#define UMCTL2_60_3     0
#define UMCTL2_60_4     0
#define UMCTL2_60       ((UMCTL2_60_1)  << 16 |\
                         (UMCTL2_60_2)  <<  4 |\
                         (UMCTL2_60_3)  <<  1 |\
                         (UMCTL2_60_4)  <<  0 )

#define UMCTL2_64	0x00610130

#define UMCTL2_68_1     72
#define UMCTL2_68_2     0
#define UMCTL2_68       ((UMCTL2_68_1)  << 16 |\
                         (UMCTL2_68_2)  << 0 )

#define UMCTL2_C0_1     0
#define UMCTL2_C0_2     0
#define UMCTL2_C0_3     0
#define UMCTL2_C0_4     0
#define UMCTL2_C0_5     0
#define UMCTL2_C0_6     0
#define UMCTL2_C0_7     0
#define UMCTL2_C0       ((UMCTL2_C0_1)  << 16 |\
                         (UMCTL2_C0_2)  << 15 |\
                         (UMCTL2_C0_3)  <<  8 |\
                         (UMCTL2_C0_4)  <<  4 |\
                         (UMCTL2_C0_5)  <<  2 |\
                         (UMCTL2_C0_6)  <<  1 |\
                         (UMCTL2_C0_7)  <<  0 )

#define UMCTL2_C4_1     1
#define UMCTL2_C4_2     0
#define UMCTL2_C4(n)       ((n)  << 12 )

#define UMCTL2_D0_1     3
#define UMCTL2_D0_2     3
#define UMCTL2_D0_3     3
#define UMCTL2_D0       ((UMCTL2_D0_1)  << 30 |\
                         (UMCTL2_D0_2)  << 16 |\
                         (UMCTL2_D0_3)  <<  0 )

#define UMCTL2_D4_1     1
#define UMCTL2_D4_2     13
#define UMCTL2_D4       ((UMCTL2_D4_1)  << 16 |\
                         (UMCTL2_D4_2)  <<  0 )

#define UMCTL2_D8	0x00008A05
#define UMCTL2_DC	0x0054002d

#define UMCTL2_E0       0x00f10028
#define UMCTL2_E4_1     4
#define UMCTL2_E4_2     9
#define UMCTL2_E4       ((UMCTL2_E4_1)  << 16 |\
                         (UMCTL2_E4_2)  <<  0 )

#define UMCTL2_E8       0x0000004d

#define UMCTL2_EC       0x0000004d

#define UMCTL2_F0_1     0
#define UMCTL2_F0_2     0
#define UMCTL2_F0_3     0
#define UMCTL2_F0_4     0
#define UMCTL2_F0_5     0
#define UMCTL2_F0_6     1
#define UMCTL2_F0_7     0
#define UMCTL2_F0_8     0
#define UMCTL2_F0_9     0
#define UMCTL2_F0_10    0
#define UMCTL2_F0_11    0
#define UMCTL2_F0       ((UMCTL2_F0_1)  << 14 |\
                         (UMCTL2_F0_2)  << 13 |\
                         (UMCTL2_F0_3)  << 12 |\
                         (UMCTL2_F0_4)  <<  8 |\
                         (UMCTL2_F0_5)  <<  6 |\
                         (UMCTL2_F0_6)  <<  5 |\
                         (UMCTL2_F0_7)  <<  4 |\
                         (UMCTL2_F0_8)  <<  3 |\
                         (UMCTL2_F0_9)  <<  2 |\
                         (UMCTL2_F0_10) <<  1 |\
                         (UMCTL2_F0_11) <<  0 )

#define UMCTL2_F4		 0x0000e745
#define UMCTL2_100       0x1b203622
#define UMCTL2_104       0x00060630
#define UMCTL2_108       0x070e1517 //RL=28 WL=14
#define UMCTL2_10C       0x00b0c006
#define UMCTL2_110       0x0f04080f
#define UMCTL2_114       0x09520c0c
#define UMCTL2_118       0x0905000c
#define UMCTL2_11C       0x0000080c

#define UMCTL2_120_1    1
#define UMCTL2_120_2    1
#define UMCTL2_120_3    1
#define UMCTL2_120_4    1
#define UMCTL2_120      ((UMCTL2_120_1) << 24 |\
                         (UMCTL2_120_2) << 16 |\
                         (UMCTL2_120_3) <<  8 |\
                         (UMCTL2_120_4) <<  0 )

#define UMCTL2_124       0x00000028
#define UMCTL2_128       0x0009080b
#define UMCTL2_12C       0x01010005
#define UMCTL2_130       0x00020000
#define UMCTL2_134       0x0c100002
#define UMCTL2_138       0x00000f6b

#define UMCTL2_13C_1    1
#define UMCTL2_13C_2    0
#define UMCTL2_13C_3    0
#define UMCTL2_13C      ((UMCTL2_13C_1) << 31 |\
                         (UMCTL2_13C_2) << 24 |\
                         (UMCTL2_13C_3) <<  0 )

#define UMCTL2_180_1    0
#define UMCTL2_180_2    0
#define UMCTL2_180_3    0
#define UMCTL2_180_4    0
#define UMCTL2_180_5    800
#define UMCTL2_180_6    24
#define UMCTL2_180      ((UMCTL2_180_1) << 31 |\
                         (UMCTL2_180_2) << 30 |\
                         (UMCTL2_180_3) << 29 |\
                         (UMCTL2_180_4) << 28 |\
                         (UMCTL2_180_5) << 16 |\
                         (UMCTL2_180_6) <<  0 )

#define UMCTL2_184_1    40
#define UMCTL2_184_2    112
#define UMCTL2_184      ((UMCTL2_184_1) << 20 |\
                         (UMCTL2_184_2) <<  0 )

#define UMCTL2_188	0x00000000

#define UMCTL2_190_1    3
#define UMCTL2_190_2    1
#define UMCTL2_190_3    23 //t_rddata_en = RL-5 = 23
#define UMCTL2_190_4    1
#define UMCTL2_190_5    2
#define UMCTL2_190_6    10
#define UMCTL2_190      ((UMCTL2_190_1) << 24 |\
                         (UMCTL2_190_2) << 23 |\
                         (UMCTL2_190_3) << 16 |\
                         (UMCTL2_190_4) << 15 |\
                         (UMCTL2_190_5) <<  8 |\
                         (UMCTL2_190_6) <<  0 )

#define UMCTL2_194_1    0
#define UMCTL2_194_2    0
#define UMCTL2_194_3    9
#define UMCTL2_194_4    2
#define UMCTL2_194_5    2
#define UMCTL2_194      ((UMCTL2_194_1) << 28 |\
                         (UMCTL2_194_2) << 24 |\
                         (UMCTL2_194_3) << 16 |\
                         (UMCTL2_194_4) <<  8 |\
                         (UMCTL2_194_5) <<  0 )

#define UMCTL2_198_1    7
#define UMCTL2_198_2    15
#define UMCTL2_198_3    1
#define UMCTL2_198_4    10
#define UMCTL2_198_5    0
#define UMCTL2_198_6    0
#define UMCTL2_198_7    1
#define UMCTL2_198      ((UMCTL2_198_1) << 24 |\
                         (UMCTL2_198_2) << 20 |\
                         (UMCTL2_198_3) << 16 |\
                         (UMCTL2_198_4) << 12 |\
                         (UMCTL2_198_5) <<  8 |\
                         (UMCTL2_198_6) <<  4 |\
                         (UMCTL2_198_7) <<  0 )

#define UMCTL2_19C	0x00000020


#define UMCTL2_1A0_1    0
#define UMCTL2_1A0_2    0
#define UMCTL2_1A0_3    1
#define UMCTL2_1A0_4    64
#define UMCTL2_1A0_5    24
#define UMCTL2_1A0      ((UMCTL2_1A0_1) << 31 |\
                         (UMCTL2_1A0_2) << 30 |\
                         (UMCTL2_1A0_3) << 29 |\
                         (UMCTL2_1A0_4) << 16 |\
                         (UMCTL2_1A0_5) <<  0 )

#define UMCTL2_1A4_1    1
#define UMCTL2_1A4_2    250
#define UMCTL2_1A4      ((UMCTL2_1A4_1) << 16 |\
                         (UMCTL2_1A4_2) <<  0 )

#define UMCTL2_1A8_1    0
#define UMCTL2_1A8      ((UMCTL2_1A8_1) << 31 )


#define UMCTL2_1B0_4    0
#define UMCTL2_1B0_5    0
#define UMCTL2_1B0_6    1
#define UMCTL2_1B0_7    0
#define UMCTL2_1B0_8    1
#define UMCTL2_1B0_9    0
#define UMCTL2_1B0_10   0
#define UMCTL2_1B0_11   0
#define UMCTL2_1B0_12   0
#define UMCTL2_1B0_13   1
#define UMCTL2_1B0_14   1
#define UMCTL2_1B0_15   0
#define UMCTL2_1B0_1    ((UMCTL2_1B0_8)         << 4 |\
                         (UMCTL2_1B0_10)        << 3 |\
                         (UMCTL2_1B0_11)        << 2 |\
                         (UMCTL2_1B0_12)        << 1 |\
                         (UMCTL2_1B0_14)        << 0 )//bit:10001
#define UMCTL2_1B0_2    ((UMCTL2_1B0_8)         << 4 |\
                         (UMCTL2_1B0_10)        << 3 |\
                         (UMCTL2_1B0_11)        << 2 |\
                         (UMCTL2_1B0_12)        << 1 |\
                         (UMCTL2_1B0_15)        << 0 )//bit:10000
#define UMCTL2_1B0(n)   ((UMCTL2_1B0_4)         << 8 |\
                         (UMCTL2_1B0_5)         << 7 |\
                         (UMCTL2_1B0_6)         << 6 |\
                         (UMCTL2_1B0_7)         << 5 |\
                         (n)                    << 0 )

#define UMCTL2_1B4_1    23 //rdcslat=RL-5
#define UMCTL2_1B4_2    10
#define UMCTL2_1B4      ((UMCTL2_1B4_1) << 8 |\
                         (UMCTL2_1B4_2) << 0 )

#define UMCTL2_1B8	0x0000001f
#define UMCTL2_1C0	0x00000001	//bit0=1:open write mask

#define UMCTL2_1C4_1    1
#define UMCTL2_1C4_2    0
#define UMCTL2_1C4(n)      ((n) << 0 )

#ifdef PLATFORM_SP7350
#define UMCTL2_200	0x00000008 //for 2G size
#else
#define UMCTL2_200	0x00000007 //for 4G size
#endif
#define UMCTL2_204	0x00000204
#define UMCTL2_208	0x02040000
#define UMCTL2_20C	0x00050406
#define UMCTL2_210	0x00001f1f
#define UMCTL2_214	0x0a080309
#define UMCTL2_218	0x09080608
#ifdef PLATFORM_SP7350
#define UMCTL2_21C	0x00000f07 //for 2G size
#else
#define UMCTL2_21C	0x00000f08 //for 4G size
#endif
#define UMCTL2_220	0x00000000

#define UMCTL2_224	0x09090b0b
#define UMCTL2_228	0x0b0a0602
#define UMCTL2_22C	0x0000000a
#define UMCTL2_240	0x0f050f40

#define UMCTL2_244_1    0
#define UMCTL2_244_2    0
#define UMCTL2_244_3    0
#define UMCTL2_244_4    0
#define UMCTL2_244_5    0
#define UMCTL2_244_6    0
#define UMCTL2_244_7    0
#define UMCTL2_244_8    0
#define UMCTL2_244      ((UMCTL2_244_1) << 28 |\
                         (UMCTL2_244_2) << 24 |\
                         (UMCTL2_244_3) << 20 |\
                         (UMCTL2_244_4) << 16 |\
                         (UMCTL2_244_5) << 12 |\
                         (UMCTL2_244_6) <<  8 |\
                         (UMCTL2_244_7) <<  4 |\
                         (UMCTL2_244_8) <<  0 )

#define UMCTL2_250		0x260b1786
#define UMCTL2_254_1    4
#define UMCTL2_254      ((UMCTL2_254_1) << 0 )
#define UMCTL2_25C		0x6100a3f5
#define UMCTL2_264    	0x4200594c
#define UMCTL2_26C      0x47005edf

#define UMCTL2_300_1    0
#define UMCTL2_300_2    0
#define UMCTL2_300_3    0
#define UMCTL2_300_4    1
#define UMCTL2_300      ((UMCTL2_300_1) << 4 |\
                         (UMCTL2_300_2) << 2 |\
                         (UMCTL2_300_3) << 1 |\
                         (UMCTL2_300_4) << 0 )

#define UMCTL2_304_3    0
#define UMCTL2_304_1    1
#define UMCTL2_304_2    0
#define UMCTL2_304_4    ((UMCTL2_304_1)         <<  1 |\
                         (UMCTL2_304_2)         <<  0 )  //bit:10
#define UMCTL2_304_5    ((UMCTL2_304_2)         <<  1 |\
                         (UMCTL2_304_1)         <<  0 )  //bit:01
#define UMCTL2_304(n)   ((n) << 0 )

#define UMCTL2_30C_1    0
#define UMCTL2_30C_2    0
#define UMCTL2_30C_3    0
#define UMCTL2_30C_4    0
#define UMCTL2_30C_5    0
#define UMCTL2_30C_6    0
#define UMCTL2_30C_7    0
#define UMCTL2_30C_8    0
#define UMCTL2_30C_9    0
#define UMCTL2_30C_10   0
#define UMCTL2_30C_11   0
#define UMCTL2_30C_12   0
#define UMCTL2_30C_13   0
#define UMCTL2_30C_14   0
#define UMCTL2_30C_15   0
#define UMCTL2_30C_16   0
#define UMCTL2_30C_17   0
#define UMCTL2_30C_18   0
#define UMCTL2_30C      ((UMCTL2_30C_1)         << 19 |\
                         (UMCTL2_30C_2)         << 18 |\
                         (UMCTL2_30C_3)         << 17 |\
                         (UMCTL2_30C_4)         << 16 |\
                         (UMCTL2_30C_5)         << 15 |\
                         (UMCTL2_30C_6)         << 14 |\
                         (UMCTL2_30C_7)         << 13 |\
                         (UMCTL2_30C_8)         << 12 |\
                         (UMCTL2_30C_9)         << 11 |\
                         (UMCTL2_30C_10)        << 10 |\
                         (UMCTL2_30C_11)        <<  9 |\
                         (UMCTL2_30C_12)        <<  8 |\
                         (UMCTL2_30C_13)        <<  5 |\
                         (UMCTL2_30C_14)        <<  4 |\
                         (UMCTL2_30C_15)        <<  3 |\
                         (UMCTL2_30C_16)        <<  2 |\
                         (UMCTL2_30C_17)        <<  1 |\
                         (UMCTL2_30C_18)        <<  0 )

#define UMCTL2_320_1    1
#define UMCTL2_320_2    0
#define UMCTL2_320(n)   ((n)    << 0 )

#define UMCTL2_328	0x00000000

#define UMCTL2_36C_1    0
#define UMCTL2_36C_2    0
#define UMCTL2_36C_3    1
#define UMCTL2_36C_4    0
#define UMCTL2_36C_5    0
#define UMCTL2_36C_6    0
#define UMCTL2_36C      ((UMCTL2_36C_1) << 24 |\
                         (UMCTL2_36C_2) << 20 |\
                         (UMCTL2_36C_3) << 16 |\
                         (UMCTL2_36C_4) <<  8 |\
                         (UMCTL2_36C_5) <<  4 |\
                         (UMCTL2_36C_6) <<  0 )

#define UMCTL2_400_1    1
#define UMCTL2_400_2    1
#define UMCTL2_400_3    0
#define UMCTL2_400      ((UMCTL2_400_1) << 8 |\
                         (UMCTL2_400_2) << 4 |\
                         (UMCTL2_400_3) << 0 )

#define UMCTL2_404_2    0
#define UMCTL2_404_3    0
#define UMCTL2_404_4    1
#define UMCTL2_404_5    0
#define UMCTL2_404_6    0
#define UMCTL2_404_7    479
#define UMCTL2_404		((UMCTL2_404_2)         << 16 |\
						 (UMCTL2_404_3)         << 14 |\
						 (UMCTL2_404_4)         << 13 |\
						 (UMCTL2_404_5)         << 12 |\
                         (UMCTL2_404_6)        << 11 |\
                         (UMCTL2_404_7)        <<  0 )

#define UMCTL2_408_5    1
#define UMCTL2_408_6    0
#define UMCTL2_408_7    0
#define UMCTL2_408_8    1
#define UMCTL2_408_9    0
#define UMCTL2_408_10   105
#define UMCTL2_408_1    ((UMCTL2_408_5)         <<  2 |\
                         (UMCTL2_408_7)         <<  1 |\
                         (UMCTL2_408_9)         <<  0 )
#define UMCTL2_408_2    ((UMCTL2_408_5)         <<  2 |\
                         (UMCTL2_408_7)         <<  1 |\
                         (UMCTL2_408_8)         <<  0 )
#define UMCTL2_408_3    ((UMCTL2_408_6)         <<  2 |\
                         (UMCTL2_408_7)         <<  1 |\
                         (UMCTL2_408_8)         <<  0 )
#define UMCTL2_408_4    ((UMCTL2_408_5)         <<  2 |\
                         (UMCTL2_408_8)         <<  1 |\
                         (UMCTL2_408_9)         <<  0 )
#define UMCTL2_408(n)   ((n)                    << 12 |\
                         (UMCTL2_408_10)        <<  0 )

#define UMCTL2_490_1    1
#define UMCTL2_490      ((UMCTL2_490_1) << 0 )

#define UMCTL2_494	0x00210001

#define UMCTL2_498	0x01da0521

#define UMCTL2_49C	0x01100806

#define UMCTL2_4A0	0x04590572

#define uMCTL2_register_offset  0x9c107000

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
