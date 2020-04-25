/*
 * Copyright (c) 2007 Xilinx, Inc.  All rights reserved.
 *
 * Xilinx, Inc.
 * XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR
 * STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION.
 * XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 */
#include "arch/cc.h"
#include "lwipopts.h"
#include "platform.h"
#include "xil_types.h"
#include "xil_io.h"
#include "xil_assert.h"
#include "xparameters.h"
#include "stdio.h"
#include "sleep.h"
#include "xparameters.h"
#include "xparameters_ps.h"	/* defines XPAR values */
#include "xil_types.h"
#include "xil_assert.h"
#include "xil_io.h"
#include "xil_exception.h"
#include "xpseudo_asm.h"
#include "xil_cache.h"
#include "xil_printf.h"
#include "xuartps.h"
#include "xscugic.h"
#include "xscutimer.h"
#include "xemacps.h"		/* defines XEmacPs API */
#include "lwip/tcp.h"
#include "mfs_config.h"

volatile int TcpFastTmrFlag = 0;
volatile int TcpSlowTmrFlag = 0;

#define EMACPS_DEVICE_ID   0
#define INTC_DEVICE_ID      XPAR_SCUGIC_SINGLE_DEVICE_ID
#define UART_DEVICE_ID      XPAR_XUARTPS_0_DEVICE_ID
#define TIMER_DEVICE_ID		XPAR_SCUTIMER_DEVICE_ID
#define EMACPS_IRPT_INTR   XPS_GEM0_INT_ID
#define INTC_BASE_ADDR		XPAR_SCUGIC_CPU_BASEADDR
#define INTC_DIST_BASE_ADDR	XPAR_SCUGIC_DIST_BASEADDR
#define TIMER_IRPT_INTR		XPAR_SCUTIMER_INTR

static XScuTimer TimerInstance;
volatile int TxPerfConnMonCntr = 0;

void timer_callback(XScuTimer * TimerInstance)
{
	/* we need to call tcp_fasttmr & tcp_slowtmr at intervals specified by lwIP.
	 * It is not important that the timing is absoluetly accurate.
	 */
	static int odd = 1;
	TcpFastTmrFlag = 1;
	odd = !odd;
	if (odd) {
		TxPerfConnMonCntr++;
		TcpSlowTmrFlag = 1;
	}
	XScuTimer_ClearInterruptStatus(TimerInstance);
}

int Init_ScuTimer(void)
{
	int Status = XST_SUCCESS;
	XScuTimer_Config *ConfigPtr;
	int TimerLoadValue = 0;

	ConfigPtr = XScuTimer_LookupConfig(TIMER_DEVICE_ID);
	Status = XScuTimer_CfgInitialize(&TimerInstance, ConfigPtr,
			ConfigPtr->BaseAddr);
	if (Status != XST_SUCCESS) {

		xil_printf("In %s: Scutimer Cfg initialization failed...\r\n", __func__);

		return XST_FAILURE;
	}

	Status = XScuTimer_SelfTest(&TimerInstance);
	if (Status != XST_SUCCESS) {

		xil_printf("In %s: Scutimer Self test failed...\r\n", __func__);

		return XST_FAILURE;
	}

	XScuTimer_EnableAutoReload(&TimerInstance);
	/*
	 * Set for 250 milli seconds timeout.
	 */
	TimerLoadValue = XPAR_CPU_CORTEXA9_0_CPU_CLK_FREQ_HZ / 8;

	XScuTimer_LoadTimer(&TimerInstance, TimerLoadValue);
	return XST_SUCCESS;
}

int SetupIntrSystem( XScuTimer * TimerInstancePtr,
		u16 TimerIntrId)
{
	Xil_ExceptionInit();

	XScuGic_DeviceInitialize(INTC_DEVICE_ID);

	/*
	 * Connect the interrupt controller interrupt handler to the hardware
     * interrupt handling logic in the processor.
	 */
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_IRQ_INT,
						(Xil_ExceptionHandler)XScuGic_DeviceInterruptHandler,
						(void *)INTC_DEVICE_ID);

	/*
	 * Connect the device driver handler that will be called when an
	 * interrupt for the device occurs, the handler defined above performs
	 * the specific interrupt processing for the device.
	 */
	XScuGic_RegisterHandler(INTC_BASE_ADDR,
							TimerIntrId,
							(Xil_ExceptionHandler)timer_callback,
							(void *)&TimerInstance);
	/*
	 * Enable the interrupt for scu timer.
	 */
	XScuGic_EnableIntr(INTC_DIST_BASE_ADDR, TimerIntrId);
	return XST_SUCCESS;
}

void platform_enable_interrupts()
{
	/*
	 * Enable non-critical exceptions.
	 */
	Xil_ExceptionEnableMask(XIL_EXCEPTION_IRQ);
	XScuTimer_EnableInterrupt(&TimerInstance);
	XScuTimer_Start(&TimerInstance);
}

int init_platform()
{
	if (Init_ScuTimer() != XST_SUCCESS) while(1);

	SetupIntrSystem(&TimerInstance, TIMER_IRPT_INTR);

	/* initialize file system layer */
	if (platform_init_fs() < 0)
            return -1;

	xil_printf("%s: success\r\n", __func__);
        return 0;
}

int platform_init_fs()
{
	/* initialize the memory file system (MFS) image pre-loaded into memory */
	mfs_init_fs(MFS_NUMBYTES, (char *)(MFS_BASE_ADDRESS+4), MFS_INIT_TYPE);

	/* check if we can access index.html */
	if (mfs_exists_file("index.html") == 0) {
		xil_printf("%s: ERROR: unable to locate index.html in MFS\r\n", __FUNCTION__);
		xil_printf("One of your applications requires a Memory File System to be loaded.\r\n");
                xil_printf("Please check if MFS has been loaded, "
				"and it has index.html file in root directory\r\n");
		return -1;
	}

	return 0;
}

