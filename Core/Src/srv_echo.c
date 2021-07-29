/**
  ******************************************************************************
  * File Name          : srv_echo.c
  * Description        : This file provides code for Echo service
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "srv_echo.h"

/* Private variables ---------------------------------------------------------*/
const uint8_t ucMACAddress[6] = {configMAC_ADDR0, configMAC_ADDR1, configMAC_ADDR2, configMAC_ADDR3, configMAC_ADDR4, configMAC_ADDR5};
static const uint8_t ucIPAddress[4] = {configIP_ADDR0, configIP_ADDR1, configIP_ADDR2, configIP_ADDR3};
static const uint8_t ucNetMask[4] = {configNET_MASK0, configNET_MASK1, configNET_MASK2, configNET_MASK3};
static const uint8_t ucGatewayAddress[4] = {configGATEWAY_ADDR0, configGATEWAY_ADDR1, configGATEWAY_ADDR2, configGATEWAY_ADDR3};
static const uint8_t ucDNSServerAddress[4] = {configDNS_SERVER_ADDR0, configDNS_SERVER_ADDR1, configDNS_SERVER_ADDR2, configDNS_SERVER_ADDR3};
/* Set the following constant to pdTRUE to log using the method indicated by the
 * name of the constant, or pdFALSE to not log using the method indicated by the
 * name of the constant.  Options include to standard out (xLogToStdout), to a disk
 * file (xLogToFile), and to a UDP port (xLogToUDP).  If xLogToUDP is set to pdTRUE
 * then UDP messages are sent to the IP address configured as the echo server
 * address (see the configECHO_SERVER_ADDR0 definitions in FreeRTOSConfig.h) and
 * the port number set by configPRINT_PORT in FreeRTOSConfig.h. */
const BaseType_t xLogToStdout = pdTRUE, xLogToFile = pdFALSE, xLogToUDP = pdFALSE;


/* Private function prototypes -----------------------------------------------*/
/*
 * Just seeds the simple pseudo random number generator.
 */
static void prvSRand( UBaseType_t ulSeed );
/*
 * Miscellaneous initialisation including preparing the logging and seeding the
 * random number generator.
 */
// static void prvMiscInitialisation( void );

/* Use by the pseudo random number generator. */
static UBaseType_t ulNextRand;










/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/**
  * @brief  Handles echo service.
  * @return None
  */
void srvEchoClient(void) {
//   const uint32_t ulLongTime_ms = pdMS_TO_TICKS( 1000UL );

  /* Miscellaneous initialisation including preparing the logging and seeding
    * the random number generator. */
//   prvMiscInitialisation();

  /* Initialise the network interface. */
//   vLoggingPrintf("FreeRTOS_IPInit\n");

  FreeRTOS_IPInit(ucIPAddress, ucNetMask, ucGatewayAddress, ucDNSServerAddress, ucMACAddress);
}
/*-----------------------------------------------------------*/







/* Called by FreeRTOS+TCP when the network connects or disconnects.  Disconnect
 * events are only received if implemented in the MAC driver. */
void vApplicationIPNetworkEventHook(eIPCallbackEvent_t eNetworkEvent) {
  // uint32_t ulIPAddress, ulNetMask, ulGatewayAddress, ulDNSServerAddress;
  // char cBuffer[16];
  static BaseType_t xTasksAlreadyCreated = pdFALSE;

  /* If the network has just come up...*/
  if (eNetworkEvent == eNetworkUp) {
    /* Create the tasks that use the IP stack if they have not already been
      * created. */
    if (xTasksAlreadyCreated == pdFALSE) {

      // #if mainCREATE_TCP_ECHO_TASKS_SINGLE == 1
      vStartTCPEchoClientTasks_SingleTasks(mainECHO_CLIENT_TASK_STACK_SIZE, mainECHO_CLIENT_TASK_PRIORITY);
      // #endif /* mainCREATE_TCP_ECHO_TASKS_SINGLE */

      xTasksAlreadyCreated = pdTRUE;
    }

    /* Print out the network configuration, which may have come from a DHCP
      * server. */
    // FreeRTOS_GetAddressConfiguration(&ulIPAddress, &ulNetMask, &ulGatewayAddress, &ulDNSServerAddress);
    // FreeRTOS_inet_ntoa( ulIPAddress, cBuffer );

    // vLoggingPrintf(("\r\n\r\nIP Address: %s\r\n", cBuffer));

    // FreeRTOS_inet_ntoa( ulNetMask, cBuffer );
    // vLoggingPrintf(("Subnet Mask: %s\r\n", cBuffer));

    // FreeRTOS_inet_ntoa(ulGatewayAddress, cBuffer);
    // vLoggingPrintf(("Gateway Address: %s\r\n", cBuffer));

    // FreeRTOS_inet_ntoa(ulDNSServerAddress, cBuffer);
    // vLoggingPrintf(("DNS Server Address: %s\r\n\r\n\r\n", cBuffer));

  } else {
    vLoggingPrintf("Application idle hook network down\n");
  }
}
/*-----------------------------------------------------------*/









UBaseType_t uxRand(void) {
  const uint32_t ulMultiplier = 0x015a4e35UL, ulIncrement = 1UL;

  /* Utility function to generate a pseudo random number. */

  ulNextRand = (ulMultiplier * ulNextRand) + ulIncrement;
  return((int)(ulNextRand >> 16UL) & 0x7fffUL);
}
/*-----------------------------------------------------------*/







static void prvSRand(UBaseType_t ulSeed) {
  /* Utility function to seed the pseudo random number generator. */
  ulNextRand = ulSeed;
}
/*-----------------------------------------------------------*/











// static void prvMiscInitialisation(void) {
    // time_t xTimeNow;

    // /* Seed the random number generator. */
    // time(&xTimeNow);
    // // FreeRTOS_debug_printf( ( "Seed for randomiser: %lu\n", xTimeNow ) );

    // prvSRand((uint32_t)xTimeNow);
    
    // FreeRTOS_debug_printf( ( "Random numbers: %08X %08X %08X %08X\n",
    //                          ipconfigRAND32(),
    //                          ipconfigRAND32(),
    //                          ipconfigRAND32(),
    //                          ipconfigRAND32() ) );
// }
/*-----------------------------------------------------------*/















#if (ipconfigUSE_LLMNR != 0) || (ipconfigUSE_NBNS != 0) || (ipconfigDHCP_REGISTER_HOSTNAME == 1)

  const char * pcApplicationHostnameHook(void) {
    /* Assign the name "FreeRTOS" to this network node.  This function will
    * be called during the DHCP: the machine will be registered with an IP
    * address plus this name. */
    return mainHOST_NAME;
  }

#endif
/*-----------------------------------------------------------*/










#if (ipconfigUSE_LLMNR != 0) || (ipconfigUSE_NBNS != 0)

  BaseType_t xApplicationDNSQueryHook(const char* pcName) {
    BaseType_t xReturn;

    /* Determine if a name lookup is for this node.  Two names are given
      * to this node: that returned by pcApplicationHostnameHook() and that set
      * by mainDEVICE_NICK_NAME. */
    if (strcasecmp(pcName, pcApplicationHostnameHook()) == 0) {
        xReturn = pdPASS;
    } else if (strcasecmp(pcName, mainDEVICE_NICK_NAME) == 0) {
      xReturn = pdPASS;
    } else {
      xReturn = pdFAIL;
    }

    return xReturn;
  }

#endif /* if ( ipconfigUSE_LLMNR != 0 ) || ( ipconfigUSE_NBNS != 0 ) */








/*
 * Callback that provides the inputs necessary to generate a randomized TCP
 * Initial Sequence Number per RFC 6528.  THIS IS ONLY A DUMMY IMPLEMENTATION
 * THAT RETURNS A PSEUDO RANDOM NUMBER SO IS NOT INTENDED FOR USE IN PRODUCTION
 * SYSTEMS.
 */
uint32_t ulApplicationGetNextSequenceNumber(uint32_t ulSourceAddress, uint16_t usSourcePort, uint32_t ulDestinationAddress, uint16_t usDestinationPort) {
  (void)ulSourceAddress;
  (void)usSourcePort;
  (void)ulDestinationAddress;
  (void)usDestinationPort;

  return uxRand();
}







/*
 * Supply a random number to FreeRTOS+TCP stack.
 * THIS IS ONLY A DUMMY IMPLEMENTATION THAT RETURNS A PSEUDO RANDOM NUMBER
 * SO IS NOT INTENDED FOR USE IN PRODUCTION SYSTEMS.
 */
BaseType_t xApplicationGetRandomNumber(uint32_t* pulNumber) {
  *(pulNumber) = uxRand();
  return pdTRUE;
}
























