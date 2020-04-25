//*****************************************************************************
// Qwi09LedCtrl.c
//
// Change History:
//  VER.   Author         DATE              Change Description
//  1.0    Qiwei Wu       Apr. 25, 2020     Initial Release
//*****************************************************************************

#ifndef QWI09_LEDCTRL_C
#define QWI09_LEDCTRL_C

//*****************************************************************************
// Includes
//*****************************************************************************
#include "Qwi09LedCtrl.h"

//*****************************************************************************
// Functions
//*****************************************************************************
int ToggleLed()
{
   u32b regValue;

   regValue = XRegRead32(REGLEDCTRL);

   regValue = ~regValue & 0x1;

   // switch LED status
   XRegWrite32(REGLEDCTRL, regValue);

   printf("%s: LED status is %d\r\n", __func__, regValue);

   return (int)regValue;
}

#endif
