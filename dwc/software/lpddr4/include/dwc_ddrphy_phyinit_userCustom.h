
#include <stdint.h>
#include "dwc_ddrphy_phyinit_struct.h"
/** \file
 * \brief structures and enumeration definitions
 */

//-------------------------------------------------------------
// Define some basic min/max macro's
//-------------------------------------------------------------
#define max(a,b) \
  ({ typeof (a) _a = (a); \
     typeof (b) _b = (b); \
    _a > _b ? _a : _b; })

#define min(a,b) \
  ({ typeof (a) _a = (a); \
     typeof (b) _b = (b); \
    _a < _b ? _a : _b; })

//-------------------------------------------------------------
// Defines for Firmware Images 
// - point to IMEM/DMEM incv files, 
// - indicate IMEM/DMEM size (bytes) 
//-------------------------------------------------------------
/*! \def FW_FILES_LOC
  \brief set the location of training firmware uncompressed path.  

  PhyInit will use this path to load the imem and dmem incv files of the
    firmware image.
 */
/*! \def IMEM_INCV_FILENAME
  \brief firmware instruction memory (imem) filename for 1D training
 */
/*! \def DMEM_INCV_FILENAME
  \brief firmware instruction memory (imem) filename for 1D training.
 */
/*! \def IMEM_SIZE
  \brief max size of instruction memory.
 */
/*! \def DMEM_SIZE
  \brief max size of data memory.
 */
/*! \def DMEM_ST_ADDR
  \brief start of DMEM address in memory.
 */
#ifndef FW_FILES_LOC
#define FW_FILES_LOC "./fw" 
#endif 
#define IMEM_INCV_FILENAME               FW_FILES_LOC"/lpddr4/lpddr4_pmu_train_imem.incv"
#define DMEM_INCV_FILENAME               FW_FILES_LOC"/lpddr4/lpddr4_pmu_train_dmem.incv"

/*! \def IMEM_INCV_FILENAME_2D
  \brief firmware instruction memory (imem) filename for 2D training
 */
/*! \def DMEM_INCV_FILENAME_2D
  \brief firmware instruction memory (imem) filename for 2D training.
 */
#define IMEM_INCV_FILENAME_2D            FW_FILES_LOC"/lpddr4_2d/lpddr4_2d_pmu_train_imem.incv"
#define DMEM_INCV_FILENAME_2D            FW_FILES_LOC"/lpddr4_2d/lpddr4_2d_pmu_train_dmem.incv"

#define IMEM_SIZE 16384
#define DMEM_SIZE 8192
#define DMEM_ST_ADDR 0x54000

///> Added for multiple PHY interface.
#ifndef DWC_DDRPHY_NUM_PHY 
#define DWC_DDRPHY_NUM_PHY (1)
#endif
//-------------------------------------------------------------
// Defines for SR Firmware Images 
// - point to IMEM incv files, 
// - indicate IMEM size (bytes) 
//-------------------------------------------------------------
/*! \def SR_FW_FILES_LOC
  \brief location of optional retention save restore firmware image.
 */
/*! \def SR_IMEM_SIZE
  \brief max IMEM size of retention save/restore firmware.
 */
/*! \def SR_IMEM_INCV_FILENAME
  \brief file name of retention save/restore IMEM image.
 */
#ifndef SR_FW_FILES_LOC
#define SR_FW_FILES_LOC FW_FILES_LOC"/save_restore"
#endif

#define SR_IMEM_SIZE 16384
#define SR_IMEM_INCV_FILENAME       SR_FW_FILES_LOC"/dwc_ddrphy_io_retention_save_restore_imem.incv"

// Message Block Structure Definitions. 
#include "mnPmuSramMsgBlock_lpddr4.h"
#include "mnPmuSramMsgBlock_lpddr4_2d.h"
#include "DiagMsgBlock_lpddr4.h"



//------------------
// Type definitions
//------------------

/// A structure used to SRAM memory address space.
typedef enum {return_offset, return_lastaddr} return_offset_lastaddr_t;

/// A structure to store the sequence function runtime input variables.
typedef struct runtime_config {
  int Train2D;      ///< train2d input parameter
  int skip_train;   ///< skip_train input parameter
  int debug;        ///< print debug messages
  int RetEn;        ///< Retention Enable input parameter, instructs phyinit to \n
                    ///< issue register reads during initialization to retention registers.
} runtime_config_t;

/// enumeration of instructions for PhyInit Register Interface 
typedef enum {
  startTrack,       ///< start register tracking 
  stopTrack,        ///< stop register tracking 
  saveRegs,         ///< save(read) tracked register values 
  restoreRegs,      ///< restore (write) saved register values 
  dumpRegs,         ///< write register address,value pairs to file
  importRegs        ///< import register address,value pairs to file
} regInstr;

/// data structure to store register address, value pairs
typedef struct Reg_Addr_Val {

  uint32_t Address; ///< register address 
  uint16_t Value;   ///< register value 
} Reg_Addr_Val_t;

/// TargetCSR Target CSR for the impedance value for dwc_ddrphy_phyinit_mapDrvStren()
enum DrvType {
  DrvStrenFSDqP,  ///< 
  DrvStrenFSDqN,  ///< 
  ODTStrenP,      ///< 
  ODTStrenN,      ///< 
  ADrvStrenP,     ///< 
  ADrvStrenN      ///< 
};

//-------------------------------
// Global variables - defined in dwc_ddrphy_phyinit_globals.c
//-------------------------------

extern int ARdPtrInitVal[4];

/*! \def MAX_NUM_RET_REGS
 *  \brief default Max number of retention registers
 *
 * This define is only used by the PhyInit Register interface to define the max
 * amount of registered that can be saved. The user may increase this variable
 * as desired if a larger number of registers need to be restored.
*/
//#define MAX_NUM_RET_REGS 5000

/**  Array of Address/value pairs used to store register values for the purpose
 * of retention restore.
 */
//Reg_Addr_Val_t RetRegList[MAX_NUM_RET_REGS];  

//-------------------------------------------------------------
// Fixed Function prototypes
//-------------------------------------------------------------
int dwc_ddrphy_phyinit_sequence(int skip_training, int Train2D, int debug);
int dwc_ddrphy_phyinit_restore_sequence();
void dwc_ddrphy_phyinit_C_initPhyConfig (void);
void dwc_ddrphy_phyinit_D_loadIMEM(int Train2D);
void dwc_ddrphy_phyinit_progCsrSkipTrain(int skip_training);
void dwc_ddrphy_phyinit_F_loadDMEM(int pstate, int Train2D);
void dwc_ddrphy_phyinit_G_execFW(void);
void dwc_ddrphy_phyinit_H_readMsgBlock(int Train2D);
void dwc_ddrphy_phyinit_I_loadPIEImage(int skip_training);
void dwc_ddrphy_phyinit_assert(int Svrty, const char *fmt,...);
void dwc_ddrphy_phyinit_cmnt(const char *fmt,...);
void dwc_ddrphy_phyinit_print(const char *fmt,...);
void dwc_ddrphy_phyinit_print_dat(void);
void dwc_ddrphy_phyinit_LoadPieProdCode(void);
int dwc_ddrphy_phyinit_mapDrvStren (int DrvStren_ohm, enum DrvType TargetCSR, int HardMacroVer);
void dwc_ddrphy_phyinit_storeMsgBlk (void *msgBlkPtr,  int sizeOfMsgBlk, int mem[]);
void dwc_ddrphy_phyinit_setDefault (int Train2D);
void dwc_ddrphy_phyinit_calcMb(int Train2D);
int  dwc_ddrphy_phyinit_storeIncvFile (char * incv_file_name, int mem[], return_offset_lastaddr_t return_type);
void dwc_ddrphy_phyinit_storeMsgBlk (void *msgBlkPtr,  int size_of_msgBlk, int mem[]);
void dwc_ddrphy_phyinit_WriteOutMem (int mem[], int mem_offset, int mem_size);
int dwc_ddrphy_phyinit_IsDbyteDisabled(int DbyteNumber);
int dwc_ddrphy_phyinit_getUserInput (char *field);
int dwc_ddrphy_phyinit_getMb (int ps, char *field, int Train2D); 
int dwc_ddrphy_phyinit_setUserInput (char *field, int value);
int dwc_ddrphy_phyinit_trackReg(uint32_t adr) ;
int dwc_ddrphy_phyinit_regInterface(regInstr myRegInstr, uint32_t adr, uint16_t dat);

extern void dwc_ddrphy_phyinit_userCustom_overrideUserInput(void);
extern void dwc_ddrphy_phyinit_userCustom_A_bringupPower(void);
extern void dwc_ddrphy_phyinit_userCustom_B_startClockResetPhy(void);
extern void dwc_ddrphy_phyinit_userCustom_customPreTrain(void);
extern void dwc_ddrphy_phyinit_userCustom_customPostTrain(void);
extern int dwc_ddrphy_phyinit_userCustom_E_setDfiClk(int dfi_frequency);
extern void dwc_ddrphy_phyinit_userCustom_G_waitFwDone(void);
extern void dwc_ddrphy_phyinit_userCustom_H_readMsgBlock(int Train2D);
extern void dwc_ddrphy_phyinit_userCustom_J_enterMissionMode(void);
extern void dwc_ddrphy_phyinit_userCustom_io_write16(uint32_t adr, uint16_t dat); 
extern uint16_t dwc_ddrphy_phyinit_userCustom_io_read16(uint32_t adr); 
extern void dwc_ddrphy_phyinit_userCustom_saveRetRegs(void);

#include "dwc_ddrphy_csr_ALL_cdefines.h"



