
#include "srv_echo_client.h"

/* Exclude the whole file if FreeRTOSIPConfig.h is configured to use UDP only. */

/* !!! ------------------------ !!! */
#if ipconfigUSE_TCP == 1
/* !!! ------------------------ !!! */


/*
 * Uses a socket to send data to, then receive data from, the standard echo
 * port number 7.
 */
static void prvEchoClientTask(void *pvParameters);

/*
 * Creates a pseudo random sized buffer of data to send to the echo server.
 */
static BaseType_t prvCreateTxData(char *ucBuffer, uint32_t ulBufferLength);


/* Rx and Tx time outs are used to ensure the sockets do not wait too long for
missing data. */
static const TickType_t xReceiveTimeOut = pdMS_TO_TICKS(4000);
static const TickType_t xSendTimeOut = pdMS_TO_TICKS(2000);

/* Counters for each created task - for inspection only. */
static uint32_t 
  ulTxRxCycles[echoNUM_ECHO_CLIENTS] = {0},
	ulTxRxFailures[echoNUM_ECHO_CLIENTS] = {0},
	ulConnections[echoNUM_ECHO_CLIENTS] = {0};

/* Rx and Tx buffers for each created task. */
static char cTxBuffers[echoNUM_ECHO_CLIENTS][echoBUFFER_SIZES],
				cRxBuffers[echoNUM_ECHO_CLIENTS][echoBUFFER_SIZES];

/*-----------------------------------------------------------*/












void vStartTCPEchoClientTasks_SingleTasks(uint16_t usTaskStackSize, UBaseType_t uxTaskPriority) {
  BaseType_t x;

  /* Create the echo client tasks. */
  for (x = 0; x < echoNUM_ECHO_CLIENTS; x++) {
    xTaskCreate(prvEchoClientTask, "Echo0", usTaskStackSize, (void*)x, uxTaskPriority, NULL);
  }
}









/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

static void prvEchoClientTask(void *pvParameters) {
  Socket_t xSocket;
  
  struct freertos_sockaddr xEchoServerAddress;
  int32_t lLoopCount = 0UL;
  
  const int32_t lMaxLoopCount = 1;
  volatile uint32_t ulTxCount = 0UL;

  BaseType_t xReceivedBytes, xReturned, xInstance;
  BaseType_t lTransmitted, lStringLength;
  
  char *pcTransmittedString; 
  char *pcReceivedString;
  
  WinProperties_t xWinProps;
  TickType_t xTimeOnEntering;
  BaseType_t ret;

  /* Fill in the buffer and window sizes that will be used by the socket. */
  xWinProps.lTxBufSize = 6 * ipconfigTCP_MSS;
  xWinProps.lTxWinSize = 3;
  xWinProps.lRxBufSize = 6 * ipconfigTCP_MSS;
  xWinProps.lRxWinSize = 3;

  /* This task can be created a number of times.  Each instance is numbered
  to enable each instance to use a different Rx and Tx buffer.  The number is
  passed in as the task's parameter. */
  xInstance = (BaseType_t)pvParameters;

  /* Point to the buffers to be used by this instance of this task. */
  pcTransmittedString = &(cTxBuffers[xInstance][0]);
  pcReceivedString = &(cRxBuffers[xInstance][0]);

  /* Echo requests are sent to the echo server.  The address of the echo
  server is configured by the constants configECHO_SERVER_ADDR0 to
  configECHO_SERVER_ADDR3 in FreeRTOSConfig.h. */
  xEchoServerAddress.sin_port = FreeRTOS_htons(echoECHO_PORT);
  xEchoServerAddress.sin_addr = FreeRTOS_inet_addr_quick(configECHO_SERVER_ADDR0, configECHO_SERVER_ADDR1, configECHO_SERVER_ADDR2, configECHO_SERVER_ADDR3 );

  while (1) {
    /* Create a TCP socket. */
    xSocket = FreeRTOS_socket(FREERTOS_AF_INET, FREERTOS_SOCK_STREAM, FREERTOS_IPPROTO_TCP);
    configASSERT(xSocket != FREERTOS_INVALID_SOCKET);

    /* Set a time out so a missing reply does not cause the task to block
    indefinitely. */
    FreeRTOS_setsockopt(xSocket, 0, FREERTOS_SO_RCVTIMEO, &xReceiveTimeOut, sizeof(xReceiveTimeOut));
    FreeRTOS_setsockopt(xSocket, 0, FREERTOS_SO_SNDTIMEO, &xSendTimeOut, sizeof(xSendTimeOut));

    /* Set the window and buffer sizes. */
    FreeRTOS_setsockopt(xSocket, 0, FREERTOS_SO_WIN_PROPERTIES, (void*)&xWinProps, sizeof(xWinProps));

    /* Connect to the echo server. */
    printf("connecting to echo server....\n");

    ret = FreeRTOS_connect(xSocket, &xEchoServerAddress, sizeof(xEchoServerAddress));

    if (ret == 0) {
      printf("Connected to server.. \n");
      ulConnections[xInstance]++;

      /* Send a number of echo requests. */
      for (lLoopCount = 0; lLoopCount < lMaxLoopCount; lLoopCount++) {
        /* Create the string that is sent to the echo server. */
        lStringLength = prvCreateTxData(pcTransmittedString, echoBUFFER_SIZES);

        /* Add in some unique text at the front of the string. */
        // sprintf(pcTransmittedString, "TxRx message number %u", ulTxCount);
        // ulTxCount++;

        printf("sending data to the echo server \n");

        /* Send the string to the socket. */
        lTransmitted = FreeRTOS_send(xSocket, (void*)pcTransmittedString, lStringLength, 0);                            /* No flags. */

        if (lTransmitted < 0) {
          /* Error? */
          break;
        }

        /* Clear the buffer into which the echoed string will be
        placed. */
        memset((void*)pcReceivedString, 0x00, echoBUFFER_SIZES);
        xReceivedBytes = 0;

        /* Receive data echoed back to the socket. */
        while (xReceivedBytes < lTransmitted) {
          xReturned = FreeRTOS_recv(xSocket, &(pcReceivedString[xReceivedBytes]), (lStringLength - xReceivedBytes), 0);                                     /* No flags. */

          if (xReturned < 0) {
            /* Error occurred.  Latch it so it can be detected
            below. */
            xReceivedBytes = xReturned;
            break;
          } else if(xReturned == 0) {
            /* Timed out. */
            break;
          } else {
            /* Keep a count of the bytes received so far. */
            xReceivedBytes += xReturned;
          }
        }

        /* If an error occurred it will be latched in xReceivedBytes,
        otherwise xReceived bytes will be just that - the number of
        bytes received from the echo server. */
        if (xReceivedBytes > 0) {
          /* Compare the transmitted string to the received string. */
          configASSERT(strncmp(pcReceivedString, pcTransmittedString, lTransmitted) == 0);

          if (strncmp(pcReceivedString, pcTransmittedString, lTransmitted) == 0) {
            /* The echo reply was received without error. */
            ulTxRxCycles[xInstance]++;
          } else {
            /* The received string did not match the transmitted
            string. */
            ulTxRxFailures[xInstance]++;
            break;
          }
        } else if (xReceivedBytes < 0) {
          /* FreeRTOS_recv() returned an error. */
          break;
        } else {
          /* Timed out without receiving anything? */
          break;
        }
      }

      /* Finished using the connected socket, initiate a graceful close:
      FIN, FIN+ACK, ACK. */
      FreeRTOS_shutdown(xSocket, FREERTOS_SHUT_RDWR);

      /* Expect FreeRTOS_recv() to return an error once the shutdown is complete. */
      xTimeOnEntering = xTaskGetTickCount();

      do {
        xReturned = FreeRTOS_recv(xSocket, &(pcReceivedString[0]), echoBUFFER_SIZES, 0);

        if (xReturned < 0) {
          break;
        }
      } while ((xTaskGetTickCount() - xTimeOnEntering ) < xReceiveTimeOut);
    } else {
      printf("Could not connect to server %ld\n", ret);
    }

    /* Close this socket before looping back to create another. */
    FreeRTOS_closesocket(xSocket);

    /* Pause for a short while to ensure the network is not too
    congested. */
    vTaskDelay(echoLOOP_DELAY);
  }
}
/*-----------------------------------------------------------*/













static BaseType_t prvCreateTxData(char *cBuffer, uint32_t ulBufferLength) {
  BaseType_t 
    lCharactersToAdd, 
    lCharacter;
  
  // char cChar = '0';
  // const BaseType_t lMinimumLength = 60;

  /* Randomise the number of characters that will be sent in the echo
  request. */
  // do {
  //   lCharactersToAdd = ipconfigRAND32() % (ulBufferLength - 20UL);
  // } while((lCharactersToAdd == 0) || (lCharactersToAdd < lMinimumLength)); /* Must be at least enough to add the unique text to the start of the string later. */

  /* Fill the buffer. */
  // for( lCharacter = 0; lCharacter < lCharactersToAdd; lCharacter++ )
  // {
  // 	cBuffer[ lCharacter ] = cChar;
  // 	cChar++;

  // 	if( cChar > '~' )
  // 	{
  // 		cChar = '0';
  // 	}
  // }

  // for (uint8_t i = 0; i < 30; i++) {
  // 	cBuffer[i] = i + 50;
  // }

  cBuffer[0] = ' ';
  cBuffer[1] = 'H';
  cBuffer[2] = 'e';
  cBuffer[3] = 'l';
  cBuffer[4] = 'l';
  cBuffer[5] = 'o';
  cBuffer[6] = '!';
  cBuffer[7] = '!';
  cBuffer[8] = '!';
  cBuffer[9] = '\n';

  // cBuffer[23] = ' ';
  // cBuffer[24] = 'H';
  // cBuffer[25] = 'e';
  // cBuffer[26] = 'l';
  // cBuffer[27] = 'l';
  // cBuffer[28] = 'o';
  // cBuffer[29] = '!';
  // cBuffer[30] = '!';
  // cBuffer[31] = '!';
  // cBuffer[32] = '\n';

  lCharactersToAdd = 10;

  return lCharactersToAdd;
}
/*-----------------------------------------------------------*/









BaseType_t xAreSingleTaskTCPEchoClientsStillRunning(void) {

  static uint32_t 
    ulLastEchoSocketCount[echoNUM_ECHO_CLIENTS] = {0},
    ulLastConnections[echoNUM_ECHO_CLIENTS] = {0};

  BaseType_t xReturn = pdPASS, x;

  /* Return fail is the number of cycles does not increment between
  consecutive calls. */
  for (x = 0; x < echoNUM_ECHO_CLIENTS; x++) {
    if (ulTxRxCycles[x] == ulLastEchoSocketCount[x]) {
      xReturn = pdFAIL;
    } else {
      ulLastEchoSocketCount[x] = ulTxRxCycles[x];
    }

    if (ulConnections[x] == ulLastConnections[x]) {
      xReturn = pdFAIL;
    } else {
      ulConnections[x] = ulLastConnections[x];
    }
  }

  return xReturn;
}






/* !!! ------------------------ !!! */
#endif /* ipconfigUSE_TCP */
/* !!! ------------------------ !!! */
