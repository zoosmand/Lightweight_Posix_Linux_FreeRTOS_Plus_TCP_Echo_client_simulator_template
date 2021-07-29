/**
  ******************************************************************************
  * File Name          : srv_echo.h
  * Description        : This file provides header for Echo service .c file
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SRV_ECHO_H
#define __SRV_ECHO_H

#ifdef __cplusplus
  extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"




/* Remove the whole file if FreeRTOSIPConfig.h is set to exclude TCP. */
/* !!! ------------------------ !!! */
#if ipconfigUSE_TCP == 1
/* !!! ------------------------ !!! */


/* The maximum time to wait for a closing socket to close. */
#define tcpechoSHUTDOWN_DELAY	(pdMS_TO_TICKS(5000))

/* The standard echo port number. */
#define tcpechoPORT_NUMBER		7

/* If ipconfigUSE_TCP_WIN is 1 then the Tx sockets will use a buffer size set by
ipconfigTCP_TX_BUFFER_LENGTH, and the Tx window size will be
configECHO_SERVER_TX_WINDOW_SIZE times the buffer size.  Note
ipconfigTCP_TX_BUFFER_LENGTH is set in FreeRTOSIPConfig.h as it is a standard TCP/IP
stack constant, whereas configECHO_SERVER_TX_WINDOW_SIZE is set in
FreeRTOSConfig.h as it is a demo application constant. */
#ifndef configECHO_SERVER_TX_WINDOW_SIZE
  #define configECHO_SERVER_TX_WINDOW_SIZE	2
#endif

/* If ipconfigUSE_TCP_WIN is 1 then the Rx sockets will use a buffer size set by
ipconfigTCP_RX_BUFFER_LENGTH, and the Rx window size will be
configECHO_SERVER_RX_WINDOW_SIZE times the buffer size.  Note
ipconfigTCP_RX_BUFFER_LENGTH is set in FreeRTOSIPConfig.h as it is a standard TCP/IP
stack constant, whereas configECHO_SERVER_RX_WINDOW_SIZE is set in
FreeRTOSConfig.h as it is a demo application constant. */
#ifndef configECHO_SERVER_RX_WINDOW_SIZE
  #define configECHO_SERVER_RX_WINDOW_SIZE	2
#endif


/* The echo tasks create a socket, send out a number of echo requests, listen
for the echo reply, then close the socket again before starting over.  This
delay is used between each iteration to ensure the network does not get too
congested. */
	#define echoLOOP_DELAY				  (TickType_t)(150 / portTICK_PERIOD_MS)

/* The echo server is assumed to be on port 7, which is the standard echo
protocol port. */
	#define echoECHO_PORT				  7

/* The size of the buffers is a multiple of the MSS - the length of the data
sent is a pseudo random size between 20 and echoBUFFER_SIZES. */
	#define echoBUFFER_SIZE_MULTIPLIER	  3
	#define echoBUFFER_SIZES			  (ipconfigTCP_MSS * echoBUFFER_SIZE_MULTIPLIER)

/* The number of instances of the echo client task to create. */
	#define echoNUM_ECHO_CLIENTS		  1




/* The whole file is excluded if TCP is not compiled in. */
/* !!! ------------------------ !!! */
#endif /* ipconfigUSE_TCP */
/* !!! ------------------------ !!! */



/* Exported functions prototypes ---------------------------------------------*/


/* Services ------------------------------------------------------------------*/
void srvEchoClient( void );





/*----------------------------------------------------------------------------*/
#ifdef __cplusplus
  }
#endif
#endif /*__SRV_ECHO_H */

