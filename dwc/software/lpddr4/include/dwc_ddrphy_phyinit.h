/** \file */

#include "dwc_ddrphy_phyinit_userCustom.h"
/*! \def DWC_DDRPHY_PHYINIT_RID
 * cdefine for a PhyInit Revision ID
 */
#define DWC_DDRPHY_PHYINIT_RID 202006
//#define Diagnostic_test
//#############################################################################
// Global Structures : instantiated in dwc_ddrphy_globals.c
//#############################################################################
extern runtime_config_t              runtimeConfig;

extern user_input_basic_t            userInputBasic;
extern user_input_advanced_t         userInputAdvanced;
extern user_input_sim_t              userInputSim;

extern PMU_SMB_LPDDR4_1D_t        mb_LPDDR4_1D[4];
extern PMU_SMB_LPDDR4_1D_t        shdw_LPDDR4_1D[4];
extern PMU_SMB_LPDDR4_2D_t        mb_LPDDR4_2D[4];
extern PMU_SMB_LPDDR4_2D_t        shdw_LPDDR4_2D[4];
extern PMU_SMB_DIAG_t        	  mb_LPDDR4_DIAG;

// Function definitions
int dwc_ddrphy_phyinit_setMb (int ps, char *field, int value, int Train2D);
int dwc_ddrphy_phyinit_softSetMb (int ps, char *field, int value, int Train2D);
void dwc_ddrphy_phyinit_initStruct(int Train2D);

void dwc_ddrphy_apb_wr(unsigned int adr, unsigned int dat);
unsigned short dwc_ddrphy_apb_rd(unsigned int adr);

void dwc_ddrphy_phyinit_D_loadIMEM_of_SP(int Train2D);
void dwc_ddrphy_phyinit_F_loadDMEM_of_SP(int pstate, int Train2D);
#ifdef PLATFORM_SP7350
void Changing_SDRAMRDQS_of_Zebu_Emulation( void);
#endif
void dwc_ddrphy_phyinit_userCustom_G_waitFwDone_of_SP(void);
void dwc_dvdrphy_phyinit_Write_DMEM(void);
void dwc_ddrphy_phyinit_Diagnostic_test(void);

int floor(float val);
int ceil(float val);
float fmodf (float x, float y);
float fmod (float x, float y);

extern int NumRegSaved;    ///< Current Number of registers saved.
extern int mp;

