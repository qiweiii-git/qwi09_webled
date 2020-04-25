//*****************************************************************************
// Qwi09LedCtrl.h
//
// Change History:
//  VER.   Author         DATE              Change Description
//  1.0    Qiwei Wu       Apr. 25, 2020     Initial Release
//*****************************************************************************

#ifndef QWI09_LEDCTRL_H
#define QWI09_LEDCTRL_H

//*****************************************************************************
// Includes
//*****************************************************************************
#include "xil_printf.h"
#include "xil_io.h"
#include "xparameters.h"
#include "Qwi09RegDef.h"

//*****************************************************************************
// Defines
//*****************************************************************************
// Registers control define
#define XRegRead32  Xil_In32
#define XRegWrite32 Xil_Out32

// Base func defines
#define printf xil_printf
#define u32b u32

//*****************************************************************************
// Functions
//*****************************************************************************
int ToggleLed();

#endif
