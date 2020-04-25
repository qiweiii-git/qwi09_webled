//*****************************************************************************
// Qwi09Define.h
//
// This module is the global defines of Qwi09WebLed project.
//
// Change History:
//  VER.   Author         DATE              Change Description
//  1.0    Qiwei Wu       Apr. 22, 2020     Initial Release
//*****************************************************************************

#ifndef QWI09_DEFINE
#define QWI09_DEFINE

//*****************************************************************************
// Includes
//*****************************************************************************
#include "xil_printf.h"
#include "xil_io.h"
#include "xparameters.h"

// Reg includes
#include "Qwi09RegDef.h"

// Lwip
#include "lwip/init.h"
#include "lwip/tcp.h"
#include "netif/xadapter.h"
#include "platform.h"
#include "webserver.h"

//*****************************************************************************
// Defines
//*****************************************************************************
// Project Defines
#define PRJ_N "Qwi09WebLed"

// Registers control define
#define XRegRead32  Xil_In32
#define XRegWrite32 Xil_Out32

// Base func defines
#define printf xil_printf
#define u32b u32

#endif
