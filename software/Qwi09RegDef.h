//*****************************************************************************
// Qwi09RegDef.h
//
// This module is the global defines of Qwi09WebLed project.
//
// Change History:
//  VER.   Author         DATE              Change Description
//  1.0    Qiwei Wu       Apr. 22, 2020     Initial Release
//*****************************************************************************

#ifndef QWI09_REGDEF
#define QWI09_REGDEF

// Base address define
#define REGCTRL_BASEADDRESS XPAR_AXI_REG_CTRL_S_AXI_BASEADDR

enum {
   REGFWVER   = REGCTRL_BASEADDRESS + 0x0,
   REGLEDCTRL = REGCTRL_BASEADDRESS + 0x10,
}e_regdef;

#endif
