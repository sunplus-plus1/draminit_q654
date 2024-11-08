#include <types.h>
#include <common.h>
#include <config.h>
#include <dwc_dram_param.h>
#include <dwc_ddrphy_phyinit.h>

struct umctl2_regs {
	unsigned int umctl2_reg[1024];	/* change the size here, (area >> 2) */
};
static volatile struct umctl2_regs *umctl2_reg_ptr = (volatile struct umctl2_regs *)(UMCTL2_REG_Base);
#define UMCTL2_REG(OFFSET)		(umctl2_reg_ptr->umctl2_reg[OFFSET >> 2])

void ctl_apb_wr(UINT32 adr, UINT32 dat)
{
	UMCTL2_REG(adr) = dat;
}
int ctl_apb_rd(UINT32 adr)
{
	UINT32 value;
	value = UMCTL2_REG(adr);
	return value;
}

int dwc_umctl2_init_before_ctl_rst(void)
{
#ifdef DRAM_TYPE_LPDDR4
#ifdef SDRAM_SPEED_1600
#ifdef DRAM_X32
	prn_string("dwc_umctl2_lpddr4_1600_SP7350_for_realchip\n");
	#include <SP7350/LPDDR4/dwc_umctl2_lpddr4_1600_SP7350_for_realchip.txt>
#elif defined(DRAM_X16)
	prn_string("dwc_umctl2_lpddr4_1600_SP7350_for_realchip_x16\n");
	#include <SP7350/LPDDR4/dwc_umctl2_lpddr4_1600_SP7350_for_realchip_x16.txt>
#endif
#endif

#ifdef SDRAM_SPEED_1333
	prn_string("dwc_umctl2_lpddr4_1333_SP7350\n");
	#include <SP7350/LPDDR4/dwc_umctl2_lpddr4_1333_SP7350.txt>
#endif

#ifdef SDRAM_SPEED_1200
	prn_string("dwc_umctl2_lpddr4_1200_SP7350\n");
	#include <SP7350/LPDDR4/dwc_umctl2_lpddr4_1200_SP7350.txt>
#endif

#ifdef SDRAM_SPEED_1066
	prn_string("dwc_umctl2_lpddr4_1066_SP7350\n");
	#include <SP7350/LPDDR4/dwc_umctl2_lpddr4_1066_SP7350.txt>
#endif

#ifdef SDRAM_SPEED_933
	prn_string("dwc_umctl2_lpddr4_933_SP7350\n");
	#include <SP7350/LPDDR4/dwc_umctl2_lpddr4_933_SP7350.txt>
#endif

#ifdef SDRAM_SPEED_800
	prn_string("dwc_umctl2_lpddr4_800_SP7350\n");
	#include <SP7350/LPDDR4/dwc_umctl2_lpddr4_800_SP7350.txt>
#endif

#ifdef SDRAM_SPEED_733
	prn_string("dwc_umctl2_lpddr4_733_SP7350\n");
	#include <SP7350/LPDDR4/dwc_umctl2_lpddr4_733_SP7350.txt>
#endif

#ifdef SDRAM_SPEED_666
	prn_string("dwc_umctl2_lpddr4_666_SP7350\n");
	#include <SP7350/LPDDR4/dwc_umctl2_lpddr4_666_SP7350.txt>
#endif

#ifdef SDRAM_SPEED_600
	prn_string("dwc_umctl2_lpddr4_600_SP7350\n");
	#include <SP7350/LPDDR4/dwc_umctl2_lpddr4_600_SP7350.txt>
#endif

#ifdef SDRAM_SPEED_533
	prn_string("dwc_umctl2_lpddr4_533_SP7350\n");
	#include <SP7350/LPDDR4/dwc_umctl2_lpddr4_533_SP7350.txt>
#endif

#ifdef SDRAM_SPEED_400
	prn_string("dwc_umctl2_lpddr4_400_SP7350\n");
	#include <SP7350/LPDDR4/dwc_umctl2_lpddr4_400_SP7350.txt>
#endif
#endif

#ifdef DRAM_TYPE_DDR4
#ifdef SDRAM_SPEED_1600
	prn_string("Q654_DDR4_ASIC_SDRAM_DATARATE_3200\n");
	#include <SP7350/DDR4/Q654_DDR4_ASIC_SDRAM_DATARATE_3200.txt>
#endif

#ifdef SDRAM_SPEED_1333
#ifdef DRAM_X32
	prn_string("Q654_DDR4_ASIC_SDRAM_DATARATE_2666\n");
	#include <SP7350/DDR4/Q654_DDR4_ASIC_SDRAM_DATARATE_2666.txt>
#elif defined(DRAM_X16)
	prn_string("Q654_DDR4_ASIC_SDRAM_DATARATE_2666_1GB\n");
	#include <SP7350/DDR4/Q654_DDR4_ASIC_SDRAM_DATARATE_2666_1GB.txt>
#endif
#endif

#ifdef SDRAM_SPEED_1066
#ifdef DDP
	prn_string("Q654_DDR4_ASIC_SDRAM_DATARATE_2400_DDP\n");
	#include <SP7350/DDR4/Q654_DDR4_ASIC_SDRAM_DATARATE_2400_DDP.txt>
#else
	prn_string("Q654_DDR4_ASIC_SDRAM_DATARATE_2400\n");
	#include <SP7350/DDR4/Q654_DDR4_ASIC_SDRAM_DATARATE_2400.txt>
#endif
#endif

#ifdef SDRAM_SPEED_933
	prn_string("Q654_DDR4_ASIC_SDRAM_DATARATE_1866\n");
	#include <SP7350/DDR4/Q654_DDR4_ASIC_SDRAM_DATARATE_1866.txt>
#endif

#ifdef SDRAM_SPEED_800
	prn_string("Q654_DDR4_ASIC_SDRAM_DATARATE_1600\n");
	#include <SP7350/DDR4/Q654_DDR4_ASIC_SDRAM_DATARATE_1600.txt>
#endif
#endif

#ifdef DRAM_TYPE_DDR3
#ifdef SDRAM_SPEED_933
	prn_string("Q654_DDR3_ASIC_SDRAM_DATARATE_1866\n");
	#include <SP7350/DDR3/Q654_DDR3_ASIC_SDRAM_DATARATE_1866.txt>
#endif

#ifdef SDRAM_SPEED_800
	prn_string("Q654_DDR3_ASIC_SDRAM_DATARATE_1600\n");
	#include <SP7350/DDR3/Q654_DDR3_ASIC_SDRAM_DATARATE_1600.txt>
#endif

#ifdef SDRAM_SPEED_666
	prn_string("Q654_DDR3_ASIC_SDRAM_DATARATE_1333\n");
	#include <SP7350/DDR3/Q654_DDR3_ASIC_SDRAM_DATARATE_1333.txt>
#endif

#ifdef SDRAM_SPEED_533
	prn_string("Q654_DDR3_ASIC_SDRAM_DATARATE_1066\n");
	#include <SP7350/DDR3/Q654_DDR3_ASIC_SDRAM_DATARATE_1066.txt>
#endif
#endif

#ifdef NT6AN1024F32AV
	prn_string("NT6AN1024F32AV\n");
#elif defined(MT53D1024M32D4)
	prn_string("MT53D1024M32D4, 2rank, FBGA=D9WHT\n");
#elif defined(MT53E1G32D2_A)
	prn_string("MT53E1G32D2_A, 1rank, FBGA=D9ZQX\n");
#elif defined(MT53E1G32D2_B)
	prn_string("MT53E1G32D2_B, 2rank, FBGA=D8CJG\n");
#elif defined(MT53E2G32D4_C)
	prn_string("MT53E2G32D4_C, 2rank, FBGA=D8CJN\n");
#elif defined(RS1G32LO4D2BDS)
	prn_string("RS1G32LO4D2BDS, 2rank\n");
#elif defined(HG4XD04G)
	prn_string("HG4XD04G, 2rank\n");
#elif defined(MT40A512M16TB)
	prn_string("MT40A512M16TB, 1rank, FBGA=D8BPK\n");
#elif defined(MT41K512M16VRP)
	prn_string("MT41K512M16VRP, 1rank, FBGA=D9ZWN\n");
#elif defined(K4B8G1646D)
	prn_string("K4B8G1646D, SAMSUNG 512Mx16, 2rank\n");
#elif defined(BWMZCX32H2A)
	prn_string("BWMZCX32H2A, 1rank\n");
#endif
	return 0;
}

void polling_sw_cfg_done()
{
	UINT16 rd_data;

	while (1) {
		rd_data = ctl_apb_rd(0x324);
		if (rd_data == 1) {
			prn_string("Register programming done!!!\n");
			break;
		}
	}
}

void dwc_ddrphy_phyinit_userCustom_G_waitFwDone_of_SP()
{
	#ifdef STREAM_MESSAGE
 	UINT32 value;
	#endif
	UINT16 rd_data, args, i, low_16bit, upper_16bit;
	UINT8 train_test = 0;
    mp = 0;
	prn_string("Start to wait for the training firmware to complete v.00 !!!\n");
	while (train_test == 0) {
		while (1) {
			rd_data = dwc_ddrphy_apb_rd(0xd0004);/* When set to 0, the PMU has a message for the user */
			if ((rd_data & 0x01) == 0) {
				break;
			}
		}

		low_16bit = dwc_ddrphy_apb_rd(0xd0032);/* Used to pass the lower 16 bits for streaming messages. */
		upper_16bit = dwc_ddrphy_apb_rd(0xd0034);/* Used to pass the upper 16 bits for streaming messages. */

		dwc_ddrphy_apb_wr(0xd0031,0);
		while(1)
		{
		   rd_data = dwc_ddrphy_apb_rd(0xd0004);
		   if((rd_data & 0x01) == 1)
			   break;
		}
		dwc_ddrphy_apb_wr(0xd0031,1);

		rd_data = (upper_16bit << 16) | low_16bit;
		#ifdef STREAM_MESSAGE
		prn_string("major =");prn_dword0(rd_data);
		prn_string("\n");
		#endif
		switch (rd_data & 0xffff) {
			case 0x00:
				//train_test = 1;
				prn_string("End of initialization.\n");
				break;
			case 0x01:
				prn_string("End of fine write leveling.\n");
				break;
			case 0x02:
				prn_string("End of read enable training.\n");
				break;
			case 0x03:
				prn_string("End of read delay center optimization.\n");
				break;
			case 0x04:
				prn_string("End of write delay center optimization.\n");
				break;
			case 0x05:
				prn_string("End of 2D read delay/voltage center optimization.\n");
				break;
			case 0x06:
				prn_string("End of 2D write delay/voltage center optimization.\n");
				break;
			case 0x07:
				train_test = 1;
				prn_string("Training has run successfully.(firmware complete)\n");
				mp = 1;
				break;
			case 0x08:
				#ifdef STREAM_MESSAGE
				prn_string("Start streaming message mode.\n");
				#endif
				while(1)
                {
					rd_data = dwc_ddrphy_apb_rd(0xd0004);
					if((rd_data & 0x01) == 0)
						break;
				}

				low_16bit = dwc_ddrphy_apb_rd(0xd0032);/* Used to pass the lower 16 bits for streaming messages. */
				upper_16bit = dwc_ddrphy_apb_rd(0xd0034);/* Used to pass the upper 16 bits for streaming messages. */

				dwc_ddrphy_apb_wr(0xd0031,0);
				while(1)
				{
				   rd_data = dwc_ddrphy_apb_rd(0xd0004);
				   if((rd_data & 0x01) == 1)
					   break;
				}
				dwc_ddrphy_apb_wr(0xd0031,1);

				#ifdef STREAM_MESSAGE
				value = (upper_16bit << 16) | low_16bit;
				prn_string("string_index=");prn_dword0(value);
				prn_string("\n");
				#endif
				args = low_16bit & 0xffff;
				for(i = 0; i < args; i++) {
					while(1)
	                {
						rd_data = dwc_ddrphy_apb_rd(0xd0004);
						if((rd_data & 0x01) == 0)
							break;
					}
					low_16bit = dwc_ddrphy_apb_rd(0xd0032);/* Used to pass the lower 16 bits for streaming messages. */
					upper_16bit = dwc_ddrphy_apb_rd(0xd0034);/* Used to pass the upper 16 bits for streaming messages. */

					dwc_ddrphy_apb_wr(0xd0031,0);
					while(1)
					{
					   rd_data = dwc_ddrphy_apb_rd(0xd0004);
					   if((rd_data & 0x01) == 1)
						   break;
					}
					dwc_ddrphy_apb_wr(0xd0031,1);
					#ifdef STREAM_MESSAGE
					value = (upper_16bit << 16) | low_16bit;
					prn_string("args=");prn_dword0(value);
					prn_string("\n");
					#endif
				}
				break;
			case 0x09:
				prn_string("End of max read latency training.\n");
				break;
			case 0x0a:
				prn_string("End of read dq deskew training.\n");
				break;
			case 0x0b:
				prn_string("Reserved.\n");
				break;
			case 0x0c:
				prn_string("End of all DB training(MREP/DWL/MRD/MWD complete).\n");
				break;
			case 0x0d:
				prn_string("End of CA training.\n");
				break;
			case 0xfd:
				prn_string("End of MPR read delay center optimization.\n");
				break;
			case 0xfe:
				prn_string("End of Wrtie leveling coarse delay.\n");
				break;
			case 0xff:
				prn_string("Training has failed.(firmware complete)\n");
				while(1);
				return;
		}
	}
}

void run_customPostTrain()
{
	UINT16 rd_data;

	dwc_ddrphy_apb_wr(0xd0000,0);
	rd_data = dwc_ddrphy_apb_rd(0x20010);
	prn_string("32'h20010 rd_data = ");prn_dword0(rd_data);
	prn_string("\n");
	dwc_ddrphy_apb_wr(0x20010,0x6a);
	dwc_ddrphy_apb_wr(0x20010,0x6a);
	rd_data = dwc_ddrphy_apb_rd(0x2001d);
	prn_string("32'h2001d rd_data = ");prn_dword0(rd_data);
	prn_string("\n");
	dwc_ddrphy_apb_wr(0x2001d,0x01);
	rd_data = dwc_ddrphy_apb_rd(0x20097);
	prn_string("32'h20097 rd_data = ");prn_dword0(rd_data);
	prn_string("\n");
}

int ctl_trigger_init_and_wait_normal()
{
	UINT16 rd_data;

	ctl_apb_wr(0x320,0x00000000);//SWCTL
	ctl_apb_wr(0x1b0,0x00000070);   //trigger dfi_init_start
	//prn_string("Start to wait for dfi_init_complete !!!");
	//prn_string("\n");
	while (1) {
		rd_data = ctl_apb_rd(0x1bc);
		if (rd_data == 1) {
			//prn_string("Wait for dfi_init_complete, done!!!");
			//prn_string("\n");
			break;
		}
	}
	//->TRIG_VIP_REFRESH_CHECK;

	ctl_apb_wr(0x1b0,0x00000050);
	#ifdef DRAM_TYPE_LPDDR4
    ctl_apb_wr(0x1b0,0x00000055); //trigger SDRAM initilaztion.
	#else
	ctl_apb_wr(0x1b0,0x00000051); //trigger SDRAM initilaztion.
	#endif
	ctl_apb_wr(0x030,0x00000000); //PWRCTL
	ctl_apb_wr(0x320,0x00000001); //SWCTL
	
	polling_sw_cfg_done();

	//prn_string("Start to wait for ctl into normal mode !!!");
	//prn_string("\n");
	while (1) {
		rd_data = ctl_apb_rd(0x004);
		if (rd_data == 1) {
			//prn_string("Wait for ctl into normal mode, done!!!");
			//prn_string("\n");
			break;
		}
	}

	ctl_apb_wr(0x1c4,0x80000001); //enable the PHY master interface.

	return 1;
}

void apb_ctl_before_change_clock(unsigned int ps) //Ch6.3.3.6
{
	UINT32 rd_data;
	prn_string("apb_ctl_before_change_clock\n");

	rd_data = ctl_apb_rd(0x490);
	ctl_apb_wr(0x490, rd_data & 0xfffffffe);  //Step1. This blocks AXI ports from taking anymore transactions

	rd_data = ctl_apb_rd(0x3fc);
	while(rd_data != 0){
	rd_data = ctl_apb_rd(0x3fc);  //Step2. Wait until all AXI ports are IDLE.
	}
	//$display($time, "[Debug] All AXI ports are IDLE.!!!");

	rd_data = ctl_apb_rd(0x20);
	ctl_apb_wr(0x20, rd_data | (1<<13));  //Step5.


	rd_data = ctl_apb_rd(0x304);
	ctl_apb_wr(0x304, rd_data | (1<<1));  //Step6. New command are accepted by uMCTL2.

	rd_data = ctl_apb_rd(0x308);
	while((rd_data & (3<<28)) != (3<<28)){
	rd_data = ctl_apb_rd(0x308);  //Step7. Ensure all outstanding commands have been sent on the DFi.
	}
	//$display($time, "[Debug] Step7. Ensure all outstanding commands have been sent on the DFi.");

	rd_data = ctl_apb_rd(0x308);
	while((rd_data & (3<<25)) != (3<<25)){
	  rd_data = ctl_apb_rd(0x308);  //Step8. Ensure that write and read data buffer are empty.
	}
	//$display($time, "[Debug] Step8. Ensure that write and read data buffer are empty.");

	// Skip Step9. for DDR4
	////////////////////////////////////////////////
	//LPDDR4 MR UPDATE
	#ifdef DRAM_TYPE_LPDDR4
	if(ps == 0) {
		while(1) {
			rd_data = ctl_apb_rd(0x018);
			if((rd_data & 1) == 0){
				//$display($time,, "Mode reg setting by MC");
				ctl_apb_wr(0x014,0x022d);
				ctl_apb_wr(0x010,0x80000038);
				break;
			}
		}
		while(1) {
			rd_data = ctl_apb_rd(0x018);
			if((rd_data & 1) == 0){
				//$display($time,, "Mode reg setting by MC");
				ctl_apb_wr(0x014,0x0154);
				ctl_apb_wr(0x010,0x80000038);
				break;
			}
		}
	}
	else {
		while(1) {
				rd_data = ctl_apb_rd(0x018);
				if((rd_data & 1) == 0){
				 //$display($time,, "Mode reg setting by MC");
				ctl_apb_wr(0x014,0x0224);
				ctl_apb_wr(0x010,0x80000038);
				 break;
				}
			}
			//prn_string("0006\n");

			while(1) {
				rd_data = ctl_apb_rd(0x018);
				if((rd_data & 1) == 0){
				 //$display($time,, "Mode reg setting by MC");
				ctl_apb_wr(0x014,0x0144);
				ctl_apb_wr(0x010,0x80000038);
				 break;
				}
			}
	}
	#endif

	#ifdef DRAM_TYPE_DDR3
	while(1) {
		rd_data = ctl_apb_rd(0x018);
		if((rd_data & 1) == 0) break;
	}
	if(ps==0)		ctl_apb_wr(0x014,0x0f14);		// 1866 datarate
	else if(ps == 1) ctl_apb_wr(0x014,0x0d70);		// 1600 datarate
	else if(ps == 2) ctl_apb_wr(0x014,0x0b50);		// 1333 datarate
	else if(ps == 3) ctl_apb_wr(0x014,0x0930);		// 1066 datarate
	ctl_apb_wr(0x010,0x800000f8);

	while(1) {
		rd_data = ctl_apb_rd(0x018);
		if((rd_data & 1) == 0) break;
	}
	if(ps==0)		ctl_apb_wr(0x014,0x0420);		// 1866 datarate
	else if(ps == 1) ctl_apb_wr(0x014,0x0418);		// 1600 datarate
	else if(ps == 2) ctl_apb_wr(0x014,0x0410);		// 1333 datarate
	else if(ps == 3) ctl_apb_wr(0x014,0x0408);		// 1066 datarate
	ctl_apb_wr(0x010,0x800020f8);
	#endif

	//prn_string("0007\n");
	///////////////////////////////////////////////////////////////
	rd_data = ctl_apb_rd(0x198);
	ctl_apb_wr(0x198, rd_data & 0xfffffeff);  //Step10.Setting DFILPCF0.dfi_lp_en_sr = 0.
	rd_data = ctl_apb_rd(0x1bc);
	while((rd_data & (1<<1)) != 0){
	   rd_data = ctl_apb_rd(0x1bc);  //Step10.Poll DFISTAT.dfi_lp_ack = 0.
	}
	//$display($time, "[Debug] Step10. Poll DFISTAT.dfi_lp_ack = 0.");

	rd_data = ctl_apb_rd(0x04);
	while((rd_data & 0x7) == 0x3){
	  rd_data = ctl_apb_rd(0x04);  //Step11. Indicating that the DWC_ddr_umclt2 controller is not in self-refresh mode
	}
	//$display($time, "[Debug] Step11. Indicating that the DWC_ddr_umclt2 controller is not in self-refresh mode");

	rd_data = ctl_apb_rd(0x30);
	ctl_apb_wr(0x30, (rd_data | (1<<5)));  //Step12. Enter the self-refresh mode.
	rd_data = ctl_apb_rd(0x30);
	//_delay_1ms(200);
	rd_data = ctl_apb_rd(0x04);
	while((rd_data & 0x3) != 0x3){
	  rd_data = ctl_apb_rd(0x04);  //Step12. Indicating that the DWC_ddr_umclt2 controller is in self-refresh mode
	}
	rd_data = ctl_apb_rd(0x04);
	//$display($time, "[Debug] Step12. Indicating that the DWC_ddr_umclt2 controller is in self-refresh mode");
	while((rd_data & (3<<4)) != (2<<4)){
	  rd_data = ctl_apb_rd(0x04);  //Step12. Ensure transition to self-refresh is due to software.
	}
	//$display($time, "[Debug] Step12. Ensure transition to self-refresh is due to software.");

	rd_data = ctl_apb_rd(0x308);
	while((rd_data & (3<<28)) != (3<<28)){
	  rd_data = ctl_apb_rd(0x308);  //Step13. Ensure all outstanding commands have been sent on the DFi.
	}
	//$display($time, "[Debug] Step13. Ensure all outstanding commands have been sent on the DFi.");

	ctl_apb_wr(0x320,0x00000000);   //SWCTL

	rd_data = ctl_apb_rd(0x1b0);
	ctl_apb_wr(0x1b0, rd_data & 0xfffffffe);  //Step14.  set dfi_init_complete_en = 0, Ensure the uMCTL2 initizlization state isn't reset
	//$display($time, "[Debug] Step14. Ensure the uMCTL2 initizlization state isn't reset.");

	rd_data = ctl_apb_rd(0x1b0);
	if(ps == 0)
		ctl_apb_wr(0x1b0, rd_data&0xffffe0ff);  //Step15. dfi_freq[4:0] = 0x00, Go to PHY state P0.
	else
		ctl_apb_wr(0x1b0, ((rd_data & 0xffffe0ff) | (1<<8) ));
	//$display($time, "[Debug] Step15. Go to PHY state P0");

	rd_data = ctl_apb_rd(0x1b0);
	ctl_apb_wr(0x1b0, rd_data|(1<<5));  //Step16. dfi_init_start = 1
	rd_data = ctl_apb_rd(0x1bc);
	while((rd_data & 1) != 0){
	  rd_data = ctl_apb_rd(0x1bc);  //Step16. Poll dfi_init_complete = 0;
	}
	//$display($time, "[Debug] Step16. Poll dfi_init_complete = 0;");

	//change frequency
	dwc_ddrphy_phyinit_userCustom_E_setDfiClk (ps);
	//$display($time, "[Debug] Step17. change frequency to 1600");
	//Step17. uMCTL2 frequency change.
	//Step18. Update any register which may be require to change for the new frequency
#ifdef DRAM_TYPE_LPDDR4
	if(ps == 0)
	{
		ctl_apb_wr(0x050, 0x00210004);
		ctl_apb_wr(0x054, 0x00100042);
		ctl_apb_wr(0x060, 0x00000000);
		//ctl_apb_wr(0x064, 0x00618130);
		ctl_apb_wr(0x064, 0x00090072);
		ctl_apb_wr(0x068, 0x00480000);
		ctl_apb_wr(0x0d8, 0x0000e205);
		ctl_apb_wr(0x0dc, 0x0054002d);
		ctl_apb_wr(0x100, 0x1b203622);
		ctl_apb_wr(0x104, 0x00060630);
		ctl_apb_wr(0x108, 0x070e0f14);
		ctl_apb_wr(0x10c, 0x00b0c006);
		ctl_apb_wr(0x110, 0x0f04080f);
		ctl_apb_wr(0x114, 0x02040c0c);
		ctl_apb_wr(0x118, 0x01010007);
		ctl_apb_wr(0x124, 0x00000008);
		ctl_apb_wr(0x134, 0x0c100002);
		ctl_apb_wr(0x180, 0xd3200018);
		ctl_apb_wr(0x184, 0x028b282b);
		ctl_apb_wr(0x190, 0x0397820a);
		ctl_apb_wr(0x1b4, 0x0000170a);
	}
	else
	{
		ctl_apb_wr(0x050, 0x00210000);
		ctl_apb_wr(0x054, 0x00100042);
		ctl_apb_wr(0x060, 0x00000000);
		//ctl_apb_wr(0x064, 0x00618130);
		ctl_apb_wr(0x064, 0x00090072);
		ctl_apb_wr(0x068, 0x00480000);
		ctl_apb_wr(0x0d8, 0x0000e205);
		ctl_apb_wr(0x0dc, 0x0054002d);
		ctl_apb_wr(0x100, 0x1b203622);
		ctl_apb_wr(0x104, 0x00060630);
		ctl_apb_wr(0x108, 0x070e0f14);
		ctl_apb_wr(0x10c, 0x00b0c006);
		ctl_apb_wr(0x110, 0x0f04080f);
		ctl_apb_wr(0x114, 0x02040c0c);
		ctl_apb_wr(0x118, 0x01010007);
		ctl_apb_wr(0x124, 0x00000008);
		ctl_apb_wr(0x128, 0x00020a00);
		ctl_apb_wr(0x134, 0x0c100002);
		ctl_apb_wr(0x180, 0xd3200018);
		ctl_apb_wr(0x184, 0x028b282b);
		ctl_apb_wr(0x190, 0x03938208);
		ctl_apb_wr(0x1b4, 0x00001308);
	}
#endif

#ifdef DRAM_TYPE_DDR4
	if(ps==0){
		ctl_apb_wr(0x064 , 0x00a2016f); //2666 data-rate
	ctl_apb_wr(0x100 , 0x14182d16);
	ctl_apb_wr(0x104 , 0x00040520);
	ctl_apb_wr(0x108 , 0x070a060e);
	ctl_apb_wr(0x10c , 0x0000400d);
	ctl_apb_wr(0x110 , 0x0a05050b);
	ctl_apb_wr(0x114 , 0x07070504);
	ctl_apb_wr(0x118 , 0x01010000);
	ctl_apb_wr(0x11c , 0x00000707);
	ctl_apb_wr(0x120 , 0x0c0c160c);
	ctl_apb_wr(0x124 , 0x0002040b);
	ctl_apb_wr(0x128 , 0x000e0c0a);
	ctl_apb_wr(0x12c , 0x0001000e);
	ctl_apb_wr(0x130 , 0x0d000008);
	ctl_apb_wr(0x190 , 0x038f8209);
	ctl_apb_wr(0x1b4 , 0x00000f09);
	}
	else if(ps==1){
	ctl_apb_wr(0x064 , 0x009200d2); //2133 data-rate
	ctl_apb_wr(0x100 , 0x12152814);
		ctl_apb_wr(0x104 , 0x0004051d);
		ctl_apb_wr(0x108 , 0x0609060d);
		ctl_apb_wr(0x10c , 0x0000400c);
		ctl_apb_wr(0x110 , 0x0904040a);
		ctl_apb_wr(0x114 , 0x06060403);
		ctl_apb_wr(0x118 , 0x01010000);
		ctl_apb_wr(0x11c , 0x00000606);
		ctl_apb_wr(0x120 , 0x07071407);
		ctl_apb_wr(0x124 , 0x0002040a);
		ctl_apb_wr(0x128 , 0x00000005);
		ctl_apb_wr(0x12c , 0x1a09010e);
		ctl_apb_wr(0x130 , 0x0c000008);
		ctl_apb_wr(0x190 , 0x038d8207);
		ctl_apb_wr(0x194 , 0x00080101);
		ctl_apb_wr(0x1b4 , 0x00000d07);
	}
	else if(ps==2){
	ctl_apb_wr(0x064 , 0x00710101);  //1866 data-rate
	ctl_apb_wr(0x100 , 0x0f111f0f);
		ctl_apb_wr(0x104 , 0x00030416);
		ctl_apb_wr(0x108 , 0x0507050b);
		ctl_apb_wr(0x10c , 0x0000400c);
		ctl_apb_wr(0x110 , 0x07030308);
		ctl_apb_wr(0x114 , 0x05050403);
		ctl_apb_wr(0x118 , 0x01010000);
		ctl_apb_wr(0x11c , 0x00000505);
		ctl_apb_wr(0x120 , 0x09090f09);
		ctl_apb_wr(0x124 , 0x00020309);
		ctl_apb_wr(0x128 , 0x00000005);
		ctl_apb_wr(0x12c , 0x1807010e);
		ctl_apb_wr(0x130 , 0x0c000008);
		ctl_apb_wr(0x190 , 0x03898205);
		ctl_apb_wr(0x194 , 0x00080101);
		ctl_apb_wr(0x1b4 , 0x00000905);
	}
	else if(ps==3){
	ctl_apb_wr(0x064 , 0x0061008c); //1600 data-rate
	ctl_apb_wr(0x100 , 0x0e0e1a0d);
		ctl_apb_wr(0x104 , 0x00030313);
		ctl_apb_wr(0x108 , 0x0507050a);
		ctl_apb_wr(0x10c , 0x0000400c);
		ctl_apb_wr(0x110 , 0x06030307);
		ctl_apb_wr(0x114 , 0x04040302);
		ctl_apb_wr(0x118 , 0x01010000);
		ctl_apb_wr(0x11c , 0x00000404);
		ctl_apb_wr(0x120 , 0x05050d05);
		ctl_apb_wr(0x124 , 0x00020308);
		ctl_apb_wr(0x128 , 0x00000005);
		ctl_apb_wr(0x12c , 0x1206010e);
		ctl_apb_wr(0x130 , 0x0c000008);
		ctl_apb_wr(0x190 , 0x03898205);
		ctl_apb_wr(0x194 , 0x00080101);
		ctl_apb_wr(0x1b4 , 0x00000905);
	}
#endif

#ifdef DRAM_TYPE_DDR3
	if(ps==0){
	ctl_apb_wr(0x064 , 0x007100a4);  //1866 data-rate
	ctl_apb_wr(0x100 , 0x0e111f10);
		ctl_apb_wr(0x104 , 0x00030416);
		ctl_apb_wr(0x108 , 0x0507050a);
		ctl_apb_wr(0x10c , 0x00002007);
		ctl_apb_wr(0x110 , 0x07020308);
		ctl_apb_wr(0x114 , 0x05050403);
		ctl_apb_wr(0x118 , 0x01010000);
		ctl_apb_wr(0x11c , 0x00000505);
		ctl_apb_wr(0x120 , 0x00000801);
		ctl_apb_wr(0x180 , 0x10960026);
		ctl_apb_wr(0x190 , 0x03888204);
		ctl_apb_wr(0x1b4 , 0x00000804);
	}
	else if(ps==1){
	ctl_apb_wr(0x064 , 0x0061008c);  //1600 data-rate
	ctl_apb_wr(0x100 , 0x0d101a0e);
		ctl_apb_wr(0x104 , 0x00030313);
		ctl_apb_wr(0x108 , 0x04060509);
		ctl_apb_wr(0x10c , 0x00002006);
		ctl_apb_wr(0x110 , 0x06020307);
		ctl_apb_wr(0x114 , 0x04040302);
		ctl_apb_wr(0x118 , 0x01010000);
		ctl_apb_wr(0x11c , 0x00000404);
		ctl_apb_wr(0x120 , 0x00000701);
		ctl_apb_wr(0x180 , 0x10800020);
		ctl_apb_wr(0x190 , 0x03868203);
		ctl_apb_wr(0x1b4 , 0x00000603);
	}
	else if(ps==2){
	ctl_apb_wr(0x064 , 0x00510075);  //1333 data-rate
	ctl_apb_wr(0x100 , 0x0b0f160b);
		ctl_apb_wr(0x104 , 0x00020310);
		ctl_apb_wr(0x108 , 0x04050408);
		ctl_apb_wr(0x10c , 0x00002007);
		ctl_apb_wr(0x110 , 0x05020206);
		ctl_apb_wr(0x114 , 0x04040302);
		ctl_apb_wr(0x118 , 0x01010000);
		ctl_apb_wr(0x11c , 0x00000404);
		ctl_apb_wr(0x120 , 0x00000601);
		ctl_apb_wr(0x180 , 0x10800020);
		ctl_apb_wr(0x190 , 0x03848202);
		ctl_apb_wr(0x1b4 , 0x00000402);
	}
	else if(ps==3){
	ctl_apb_wr(0x064 , 0x0040005e);  //1066 data-rate
	ctl_apb_wr(0x100 , 0x0a0e1109);
		ctl_apb_wr(0x104 , 0x0002020d);
		ctl_apb_wr(0x108 , 0x03040407);
		ctl_apb_wr(0x10c , 0x00002006);
		ctl_apb_wr(0x110 , 0x04020205);
		ctl_apb_wr(0x114 , 0x03030302);
		ctl_apb_wr(0x118 , 0x01010000);
		ctl_apb_wr(0x11c , 0x00000303);
		ctl_apb_wr(0x120 , 0x00000501);
		ctl_apb_wr(0x180 , 0x10800020);
		ctl_apb_wr(0x190 , 0x03828201);
		ctl_apb_wr(0x1b4 , 0x00000201);
	}
#endif



	rd_data = ctl_apb_rd(0x1b0);
	ctl_apb_wr(0x1b0, rd_data&0xffffffdf);  //Step19. Set  DFIMISC.dfi_init_start to 0. The PHY performs internal sequences to relock PLLs and calibrate ZQ/Delay-Line
	//$display($time, "[Debug] Step19. Set  DFIMISC.dfi_init_start to 0.");
	rd_data = ctl_apb_rd(0x1bc);
	while((rd_data & 1) != 1){  //Step20. PHY asserts dfi_init_compele. The controller polls DFSTAT.dfi_init_complete = 1.
	  rd_data = ctl_apb_rd(0x1bc);
	}
	//$display($time, "[Debug] Step20. PHY asserts dfi_init_compele. The controller polls DFSTAT.dfi_init_complete = 1.");

	rd_data = ctl_apb_rd(0x30);
	ctl_apb_wr(0x30, rd_data & 0xffffffdf);  //Step21 Request the DWC_ddr_umctl2_controller to exit self-refresh
	rd_data = ctl_apb_rd(0x04);
	while((rd_data & 7) == 3){
	  rd_data = ctl_apb_rd(0x04);  //Step21. Indicating that the DWC_ddr_umclt2 controller is not in self-refresh mode
	}
	//$display($time, "[Debug] Step21. Indicating that the DWC_ddr_umclt2 controller is not in self-refresh mode.");

	rd_data = ctl_apb_rd(0x198);
	ctl_apb_wr(0x198, rd_data | (1<<8));  //Step22. Self-refresh entry.

	#ifdef DRAM_TYPE_DDR4
        while(1) {
	    rd_data = ctl_apb_rd(0x018);
	    if((rd_data & 1) == 0) break;
	}
	if(ps==0) ctl_apb_wr(0x014,0x0b44);
	else if(ps == 1) ctl_apb_wr(0x014,0x0940);
	else if(ps == 2) ctl_apb_wr(0x014,0x0524);
	else if(ps == 3) ctl_apb_wr(0x014,0x0324);
	ctl_apb_wr(0x010,0x800000f8);  //mode register 0

        while(1) {
	    rd_data = ctl_apb_rd(0x018);
	    if((rd_data & 1) == 0) break;
	}
	if(ps==0) ctl_apb_wr(0x014,0x0020);
	else if(ps == 1) ctl_apb_wr(0x014,0x0018);
	else if(ps == 2) ctl_apb_wr(0x014,0x0008);
	else if(ps == 3) ctl_apb_wr(0x014,0x0008);
	ctl_apb_wr(0x010,0x800020f8);  //mode register 2

        while(1) {
	    rd_data = ctl_apb_rd(0x018);
	    if((rd_data & 1) == 0) break;
	}
	if(ps==3) ctl_apb_wr(0x014,0x0000);
	else          ctl_apb_wr(0x014,0x0200);
	ctl_apb_wr(0x010,0x800030f8);  //mode register 3

        while(1) {
	    rd_data = ctl_apb_rd(0x018);
	    if((rd_data & 1) == 0) break;
	}
	if(ps==0) ctl_apb_wr(0x014,0x1017);
	else if(ps == 1) ctl_apb_wr(0x014,0x0817);
	else                 ctl_apb_wr(0x014,0x0417);
	ctl_apb_wr(0x010,0x800060f8);  //mode register 6

	#endif

	//Step23. If necessary due to frequency change, Update MR register setting of the DRAM
	// Update MR register in Step 9
	//Step24. For DDR4
	//Step25. Enable HIF commands
	rd_data = ctl_apb_rd(0x304);
	ctl_apb_wr(0x304, rd_data & 0xfffffffd);  //Step25. Enable HIF commands

	ctl_apb_wr(0x490,0x00000001); //axi port 0 enable.
}
