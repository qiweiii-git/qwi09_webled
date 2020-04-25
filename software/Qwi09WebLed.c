//*****************************************************************************
// Qwi09WebLed.c
//
// This module is the MAIN files of Qwi09WebLed project.
//
// Change History:
//  VER.   Author         DATE              Change Description
//  1.0    Qiwei Wu       Apr. 22, 2020     Initial Release
//*****************************************************************************

//*****************************************************************************
// Includes
//*****************************************************************************
#include "Qwi09Define.h"

//*****************************************************************************
// Variables
//*****************************************************************************
// Mac address
unsigned char macEthernetAddress[] = { 0xF8, 0xA9, 0x63, 0xBB, 0xBB, 0xBB };

extern volatile int TxPerfConnMonCntr;
extern volatile int TcpFastTmrFlag;
extern volatile int TcpSlowTmrFlag;

//*****************************************************************************
// Functions
//*****************************************************************************
void tcp_fasttmr(void);
void tcp_slowtmr(void);

//*****************************************************************************
// Main
//*****************************************************************************
int main()
{
   u32b regValue;
   u32b fwVer;

   // Get FW version
   regValue = XRegRead32(REGFWVER);
   fwVer = regValue;

   printf("\r\n========================================\r\n");
   printf("=   Project: %s. Ver:v%d.%d     =\r\n", PRJ_N, (fwVer >> 8) & 0xFF, fwVer & 0xFF);
   printf("=   Date: %s, %s        =\r\n", __DATE__, __TIME__);
   printf("========================================\r\n");

   //*****************************************************************************
   // Web Server
   //*****************************************************************************
   struct netif *netif, serverNetIf;
   struct ip_addr ipAddr, netMask, gateWay;

   netif = &serverNetIf;

   if (init_platform() < 0)
   {
      printf("%s:ERROR initializing platform.\r\n", __func__);
      return -1;
   }

   // Set IPs
   IP4_ADDR(&ipAddr,  192, 168,   1, 10);
   IP4_ADDR(&netMask, 255, 255, 255,  0);
   IP4_ADDR(&gateWay, 192, 168,   1,  1);

   // LWIP init
   lwip_init();

   // Add network interface to the netif_list
   if (!xemac_add(netif, &ipAddr, &netMask, &gateWay, macEthernetAddress, XPAR_XEMACPS_0_BASEADDR))
   {
      printf("%s:Error adding N/W interface\r\n", __func__);
      return -1;
   }

   // Set network interface as default
   netif_set_default(netif);

   // Specify that the network if is up
   netif_set_up(netif);

   // enable interrupts
   platform_enable_interrupts();

   // Start Web Server
   start_web_application();

   while (1)
   {
      if (TcpFastTmrFlag)
      {
         tcp_fasttmr();
         TcpFastTmrFlag = 0;
      }
      if (TcpSlowTmrFlag)
      {
         tcp_slowtmr();
         TcpSlowTmrFlag = 0;
      }
      xemacif_input(netif);
   }

   return 0;
}

