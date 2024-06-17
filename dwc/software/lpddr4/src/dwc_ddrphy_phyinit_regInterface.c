/** \file
 * PhyInit Register Interface
 * --------------------------
 * 
 * This file provides a group of functions that are used to track PHY register
 * writes by intercepting io_write16 function calls.  Once the registers are
 * tracked, their value can be saved at a given time spot, and restored later
 * as required.  This implementation is useful to capture any PHY register
 * programing in any function during PHY initialization.
 * 
 *  \addtogroup SrcFunc
 *  @{
 */

#include <stdint.h>
#include "dwc_ddrphy_phyinit.h"

/*! \def MAX_NUM_RET_REGS
 *  \brief default Max number of retention registers
 *
 * This define is only used by the PhyInit Register interface to define the max
 * amount of registered that can be saved. The user may increase this variable
 * as desired if a larger number of registers need to be restored.
*/
#define MAX_NUM_RET_REGS 5000

/**  Array of Address/value pairs used to store register values for the purpose
 * of retention restore.
 */
Reg_Addr_Val_t RetRegList[MAX_NUM_RET_REGS];  


int NumRegSaved = 0;    ///< Current Number of registers saved.
int TrackEn = 1;        ///< Enabled tracking of registers

/** \brief Tags a register if tracking is enabled in the register
 * interface 
 * 
 * during PhyInit registers writes, keeps track of address
 * for the purpose of restoring the PHY register state during PHY
 * retention exit process.  Tracking can be turned on/off via the
 * dwc_ddrphy_phyinit_regInterface startTrack, stopTrack instructions. By
 * default tracking is always turned on.
 *
 * \return 0: not tracked 1: tracked 
 */
int dwc_ddrphy_phyinit_trackReg(uint32_t adr) {

  // return if tracking is disabled
  if (!TrackEn) return 0;

  // search register array the address,
  int regIndx=0;
  int foundReg=0;
  
  for (regIndx = 0; regIndx < NumRegSaved; regIndx++) 
    {
      if (RetRegList[regIndx].Address == adr)
        {
          foundReg=1;
          return 1;
        }
    }

  if (!foundReg && TrackEn) // register not found, so add it.
    {
      if (NumRegSaved == MAX_NUM_RET_REGS ) 
        {
           dwc_ddrphy_phyinit_assert (0," [dwc_ddrphy_phyinit_regInterface:dwc_ddrphy_phyinit_trackReg]  Max Number of Restore Registers reached: %d. Please recompile PhyInit with MAX_NUM_RET_REG set to larger value.\n",NumRegSaved);
           return 0;
        }
      RetRegList[regIndx].Address = adr;
      NumRegSaved++;
	  //dwc_ddrphy_phyinit_assert (1," [NumRegSaved=%d\n",NumRegSaved);
          
      return 1;
    }
  else
    {
      // should never get here.
      return 0; 
    }

}

/** \brief register interface function used to track, save and restore retention registers.
 * 

 * ### Usage
 * Register tracking is enabled by calling:
 *
 *  \code
 *  dwc_ddrphy_phyinit_regInterface(startTrack,0,0);
 *  \endcode
 *
 * from this point on any call to dwc_ddrphy_phyinit_usercustom_io_write16() in
 * return will be capture by the register interface via a call to
 * dwc_ddrphy_phyinit_trackReg(). Tracking is disabled by calling:
 *
 *  \code
 *  dwc_ddrphy_phyinit_regInterface(stopTrack,0,0);
 *  \endcode
 * 
 * On calling this function, register write via
 * dwc_ddrphy_phyinit_usercustom_io_write16 are no longer tracked until a
 * stratTrack call is made.  Once all the register write are complete, saveRegs
 * command can be issue to save register values into the internal data array of
 * the register interface.  Upon retention exit restoreRegs are command can be
 * used to issue register write commands to the PHY based on values stored in
 * the array.   
 *  \code
 *   dwc_ddrphy_phyinit_regInterface(saveRegs,0,0);
 *   dwc_ddrphy_phyinit_regInterface(restoreRegs,0,0);
 *  \endcode
 * \return 1 on success. 
 */
int dwc_ddrphy_phyinit_regInterface(regInstr myRegInstr, uint32_t adr, uint16_t dat) {
  if (myRegInstr == saveRegs)
    {

      // go through all the tracked registers, issue a register read and place
      // the result in the data structure for future recovery.
      int regIndx=0;
      uint16_t data;
      for (regIndx = 0; regIndx < NumRegSaved; regIndx++) 
        {
          data=dwc_ddrphy_phyinit_userCustom_io_read16(RetRegList[regIndx].Address);  
          RetRegList[regIndx].Value=data;
        }

      return 1;
    }
  else if ( myRegInstr == restoreRegs )
    { 
      // write PHY registers based on Address, Data value pairs stores in
      // RetRegList
      int regIndx=0;
      for (regIndx = 0; regIndx < NumRegSaved; regIndx++) 
        {
          dwc_ddrphy_phyinit_userCustom_io_write16(RetRegList[regIndx].Address,RetRegList[regIndx].Value);
        }
      return 1;
    }
  else if ( myRegInstr == startTrack ) // Enable tracking
    {
      TrackEn=1;
      return 1;
    }
  else if ( myRegInstr == stopTrack ) // Disable tracking
    {
      TrackEn=0;
      return 1;
    }
  else if ( myRegInstr == dumpRegs ) // Dump restore state to file.
    {
      // TBD 
      return 1;
    }
  else if ( myRegInstr == importRegs ) // import register state from file.
    {
      // TBD 
      return 1;
    }
  else 
    {
      // future instructions.
      return 0;
    }
}
/** @} */
