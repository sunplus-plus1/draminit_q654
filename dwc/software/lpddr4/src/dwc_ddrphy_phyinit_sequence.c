/** \file
 *  \addtogroup SrcFunc
 *  @{
 */
#include "dwc_ddrphy_phyinit.h"

/** \brief this function implements the flow of PhyInit software to initialize the PHY.
 *
 * The execution sequence follows the overview figure provided in the PhyInit App Note.
 *
 * \returns 0 on completion of the sequence, EXIT_FAILURE on error.
 */

void dwc_ddrphy_phyinit_Diagnostic_test(void)
{
	int count;
	dwc_ddrphy_apb_wr(0xd0099,0x1);
	dwc_ddrphy_apb_wr(0xd0000,0);
	dwc_ddrphy_apb_wr(0xc0080,0x3);


	dwc_ddrphy_phyinit_D_loadIMEM( 2 ); /* load Diagnostic image */
	dwc_ddrphy_phyinit_F_loadDMEM(0, 2);

	dwc_ddrphy_phyinit_userCustom_io_write16(0xd0000,0);
	#if 0
	dwc_ddrphy_phyinit_userCustom_io_write16(0x54200,0x06);
	dwc_ddrphy_phyinit_userCustom_io_write16(0x54201,0x01);
	dwc_ddrphy_phyinit_userCustom_io_write16(0x54202,0x3f00);
	dwc_ddrphy_phyinit_userCustom_io_write16(0x54203,0xff);
	dwc_ddrphy_phyinit_userCustom_io_write16(0x54204,0x100);
	dwc_ddrphy_phyinit_userCustom_io_write16(0x54205,0xff00);
	#else
	dwc_ddrphy_phyinit_userCustom_io_write16(0x54200,0x05);
	dwc_ddrphy_phyinit_userCustom_io_write16(0x54201,0x01);
	dwc_ddrphy_phyinit_userCustom_io_write16(0x54202,0x1100);
	dwc_ddrphy_phyinit_userCustom_io_write16(0x54203,0x0);
	dwc_ddrphy_phyinit_userCustom_io_write16(0x54204,0x100);
	dwc_ddrphy_phyinit_userCustom_io_write16(0x54205,0x0);
	#endif
	dwc_ddrphy_phyinit_userCustom_io_write16(0x54206,0);
	dwc_ddrphy_phyinit_userCustom_io_write16(0x54207,0);
	dwc_ddrphy_phyinit_userCustom_io_write16(0x54208,0);
	dwc_ddrphy_phyinit_userCustom_io_write16(0x54209,0);
	dwc_ddrphy_phyinit_userCustom_io_write16(0x5420a,0);
	dwc_ddrphy_phyinit_userCustom_io_write16(0xd0000,1);
	dwc_ddrphy_phyinit_userCustom_io_write16((tDRTUB | csr_UctWriteProt_ADDR), 0x01);
	dwc_ddrphy_phyinit_userCustom_io_write16((tAPBONLY | csr_DctWriteProt_ADDR), 0x01);
	dwc_ddrphy_phyinit_userCustom_io_write16((tDRTUB | csr_UctWriteOnly_ADDR), 0);
	dwc_ddrphy_phyinit_userCustom_io_write16((tAPBONLY | csr_MicroContMuxSel_ADDR), 0x01);
	dwc_ddrphy_phyinit_userCustom_io_write16((tAPBONLY | csr_MicroReset_ADDR), 0x09);
	dwc_ddrphy_phyinit_userCustom_io_write16((tAPBONLY | csr_MicroReset_ADDR), 0x01);
	dwc_ddrphy_phyinit_userCustom_io_write16((tAPBONLY | csr_MicroReset_ADDR), 0);
	/* Wait for the diagnostics test to finish*/
	dwc_ddrphy_phyinit_userCustom_G_waitFwDone();
	dwc_ddrphy_phyinit_userCustom_io_write16((tAPBONLY | csr_MicroReset_ADDR), 1);
	dwc_ddrphy_phyinit_userCustom_io_write16((tAPBONLY | csr_MicroContMuxSel_ADDR), 0);
	/* Read back diagnostics return data */

	dwc_ddrphy_phyinit_userCustom_io_write16(0xd0000,0);
	for (count=0; count<0x1DF4; count++)
		dwc_ddrphy_phyinit_userCustom_io_read16(0x5420b+count);
	dwc_ddrphy_phyinit_userCustom_io_write16(0xd0000,1);
}

int dwc_ddrphy_phyinit_sequence (int skip_training, int Train2D, int debug) {

    int pstate;
    char *printf_header;
    printf_header = "// [dwc_ddrphy_phyinit_sequence]";

    // registering function inputs
    runtimeConfig.skip_train = skip_training;
    runtimeConfig.Train2D    = Train2D;
    runtimeConfig.debug      = debug;

    dwc_ddrphy_phyinit_print ("%s Start of dwc_ddrphy_phyinit_sequence()\n", printf_header);

    // Initialize structures
    dwc_ddrphy_phyinit_initStruct(Train2D);

    // Enter user input
    dwc_ddrphy_phyinit_setDefault(Train2D);

    // User-editable function to override any user input set in dwc_ddrphy_phyinit_enterUserInput()
    dwc_ddrphy_phyinit_userCustom_overrideUserInput();

    // Re-calculate Firmware Message Block input based on final user input
    dwc_ddrphy_phyinit_calcMb(Train2D);

    // Printing values of user input
    if (debug == 1)
        dwc_ddrphy_phyinit_print_dat();

    // (A) Bring up VDD, VDDQ, and VAA
    dwc_ddrphy_phyinit_userCustom_A_bringupPower();

    // (B) Start Clocks and Reset the PHY
    dwc_ddrphy_phyinit_userCustom_B_startClockResetPhy();

    // (C) Initialize PHY Configuration
    dwc_ddrphy_phyinit_C_initPhyConfig ();

    // Customize any register write desired; This can include any CSR not covered by PhyInit or user wish
    // to override values calculated in step_C
    dwc_ddrphy_phyinit_userCustom_customPreTrain ();

    // Stop retention register tracking for training firmware related registers
    dwc_ddrphy_phyinit_regInterface(stopTrack,0,0);

    if (skip_training == 1)
    {
        // Skip running training firmware entirely
        dwc_ddrphy_phyinit_progCsrSkipTrain(skip_training);
    }
    else
    {
        if (skip_training == 2)
        {
            // Only execute training firmware to initialize the DRAM and
            // skip all training steps.
            runtimeConfig.Train2D = 0;
            // Set sequence Ctrl to 0x1 to only do device initialization.
            for (pstate=0; pstate<userInputBasic.NumPStates; pstate++)
            {
                mb_LPDDR4_1D[pstate].SequenceCtrl = 0x1;
            }

            dwc_ddrphy_phyinit_progCsrSkipTrain(skip_training);
        }

        switch (userInputAdvanced.PhyInitSequenceNum)
        {

            case 1: // Run 2D P0  after 1D P0 so 1D P1/P2/P3 can be run at the vrefDAC0 settings trained by 2D. See Share2DVrefResult messageblock field.
                // (D) Load the IMEM Memory for P0 1D training
                dwc_ddrphy_phyinit_D_loadIMEM( 0 );  // 0==1D

                for (pstate=0; pstate<userInputBasic.NumPStates; pstate++)
                {
                    // (E) Set the PHY input clocks to the desired frequency
                    dwc_ddrphy_phyinit_userCustom_E_setDfiClk( pstate );
                    // Note: this routine implies other items such as DfiFreqRatio, DfiCtlClk are also set properly.
                    // Because the clocks are controlled in the SOC, external to the software and PHY, this step intended to be
                    // replaced by the user with the necessary SOC operations to achieve the new input frequency to the PHY.

                    // (F) Write the Message Block parameters for the training firmware
                    dwc_ddrphy_phyinit_F_loadDMEM(pstate, 0);  // 2nd argument: 0==1D

                    // (G) Execute the Training Firmware
                    dwc_ddrphy_phyinit_G_execFW();

                    // (H) Read the Message Block results
                    dwc_ddrphy_phyinit_H_readMsgBlock(0);

                    // Now optionally perform P0 2D training for protocols that allow it
                    if (pstate == 0 && runtimeConfig.Train2D == 1)
                    {
                        // Step names here mimic the 1D lettering (E,F,G,H).
                        // They can be found in the Training Firmware Application Note

                        //if (debug == 1)
                        //    dwc_ddrphy_phyinit_printMb(pstate, 1);  // 2nd argument: 1==2D

                        // 2D-E Set the PHY input clocks to the highest frequency
                        dwc_ddrphy_phyinit_userCustom_E_setDfiClk( 0 );  // pstate==0; DfiClk fixed 2:1 ratio with MemClk

                        // 2D-F
                        dwc_ddrphy_phyinit_D_loadIMEM( 1 ); // 2D image

                        // 2D-F, cont.  Write the Message Block parameters for the training firmware
                        dwc_ddrphy_phyinit_F_loadDMEM(0, 1);  // 1st argument: pstate==0; 2nd argument: 1==2D

                        // 2D-G Execute the Training Firmware
                        dwc_ddrphy_phyinit_G_execFW();

                        // 2D-H Read the Message Block results
                        dwc_ddrphy_phyinit_H_readMsgBlock(1);

                        // (D) Reload the IMEM Memory for P1,P2,and P3 1D
                        dwc_ddrphy_phyinit_D_loadIMEM( 0 );  // 0==1D
                    }
                }
                break;

            default: // Run all 1D power states, then 2D P0, to reduce total Imem/Dmem loads.
                // (D) Load the IMEM Memory for 1D training
                dwc_ddrphy_phyinit_D_loadIMEM( 0 );  // 0==1D

                for (pstate=0; pstate<userInputBasic.NumPStates; pstate++)
                {

                    // Printing values of message block input/inout for current p-state
                    //if (debug == 1)
                    //    dwc_ddrphy_phyinit_printMb(pstate, 0);  // 2nd argument: 0==1D

                    // (E) Set the PHY input clocks to the desired frequency
                    dwc_ddrphy_phyinit_userCustom_E_setDfiClk( pstate );
                    // Note: this routine implies other items such as DfiFreqRatio, DfiCtlClk are also set properly.
                    // Because the clocks are controlled in the SOC, external to the software and PHY, this step intended to be
                    // replaced by the user with the necessary SOC operations to achieve the new input frequency to the PHY.

                    // (F) Write the Message Block parameters for the training firmware
                    dwc_ddrphy_phyinit_F_loadDMEM(pstate, 0);  // 2nd argument: 0==1D

                    // (G) Execute the Training Firmware
                    dwc_ddrphy_phyinit_G_execFW();

                    // (H) Read the Message Block results
                    dwc_ddrphy_phyinit_H_readMsgBlock(0);
                }

              // Now optionally perform 2D training for protocols that allow it
              if (runtimeConfig.Train2D == 1)
              {
                  // Step names here mimic the 1D lettering (E,F,G,H).
                  // They can be found in the Training Firmware Application Note

                  //if (debug == 1)
                  //    dwc_ddrphy_phyinit_printMb(pstate, 1);  // 2nd argument: 1==2D

                  // 2D-E Set the PHY input clocks to the highest frequency
                  dwc_ddrphy_phyinit_userCustom_E_setDfiClk( 0 );  // pstate==0; DfiClk fixed 2:1 ratio with MemClk

                  // 2D-F
                  dwc_ddrphy_phyinit_D_loadIMEM( 1 ); // 2D image

                  // 2D-F, cont.  Write the Message Block parameters for the training firmware
                  dwc_ddrphy_phyinit_F_loadDMEM(0, 1);  // 1st argument: pstate==0; 2nd argument: 1==2D

                  // 2D-G Execute the Training Firmware
                  dwc_ddrphy_phyinit_G_execFW();

                  // 2D-H Read the Message Block results
                  dwc_ddrphy_phyinit_H_readMsgBlock(1);
              }

        }
    }

	#ifdef Diagnostic_test
	if (runtimeConfig.Train2D == 1)
		dwc_ddrphy_phyinit_Diagnostic_test();
	return 0;
	#endif

    // Start retention register tracking for training firmware related registers
    dwc_ddrphy_phyinit_regInterface(startTrack,0,0);

    // (I) Load PHY Init Engine Image
    dwc_ddrphy_phyinit_I_loadPIEImage(skip_training);

    // Customize any CSR write desired to override values programmed by firmware or dwc_ddrphy_phyinit_I_loadPIEImage()
    dwc_ddrphy_phyinit_userCustom_customPostTrain ();

    if (runtimeConfig.RetEn)
      {
        // Save value of tracked registers for retention restore sequence.
        dwc_ddrphy_phyinit_userCustom_saveRetRegs();
      }
    // (J) Initialize the PHY to Mission Mode through DFI Initialization
    dwc_ddrphy_phyinit_userCustom_J_enterMissionMode();


    dwc_ddrphy_phyinit_print ("%s End of dwc_ddrphy_phyinit_sequence()\n", printf_header);

    return 0;
}
/** @} */
