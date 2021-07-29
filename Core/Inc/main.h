/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
  extern "C" {
#endif

/* Standard includes ---------------------------------------------------------*/
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>
#include <time.h>
#include <sys/time.h>

/* FreeRTOS kernel includes --------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"

/* FreeRTOS Plus IP includes -------------------------------------------------*/
#include "FreeRTOS_IP.h"
#include "FreeRTOS_Sockets.h"

/* Local includes ------------------------------------------------------------*/
#include "console.h"
#include "srv_echo.h"
#include "srv_echo_client.h"



/* Simple UDP client and server task parameters. */
#define mainSIMPLE_UDP_CLIENT_SERVER_TASK_PRIORITY    ( tskIDLE_PRIORITY )
#define mainSIMPLE_UDP_CLIENT_SERVER_PORT             ( 5005UL )

/* Echo client task parameters - used for both TCP and UDP echo clients. */
#define mainECHO_CLIENT_TASK_STACK_SIZE               ( configMINIMAL_STACK_SIZE * 2 )      /* Not used in the linux port. */
#define mainECHO_CLIENT_TASK_PRIORITY                 ( tskIDLE_PRIORITY + 1 )

/* Echo server task parameters. */
#define mainECHO_SERVER_TASK_STACK_SIZE               ( configMINIMAL_STACK_SIZE * 2 )      /* Not used in the linux port. */
#define mainECHO_SERVER_TASK_PRIORITY                 ( tskIDLE_PRIORITY + 1 )

/* Define a name that will be used for LLMNR and NBNS searches. */
#define mainHOST_NAME                                 "RTOSDemo"
#define mainDEVICE_NICK_NAME                          "linux_demo"

/* Set the following constants to 1 or 0 to define which tasks to include and
 * exclude:
 *
 * mainCREATE_TCP_ECHO_TASKS_SINGLE:  When set to 1 a set of tasks are created that
 * send TCP echo requests to the standard echo port (port 7), then wait for and
 * verify the echo reply, from within the same task (Tx and Rx are performed in the
 * same RTOS task).  The IP address of the echo server must be configured using the
 * configECHO_SERVER_ADDR0 to configECHO_SERVER_ADDR3 constants in
 * FreeRTOSConfig.h.
 *
 */
// #define mainCREATE_TCP_ECHO_TASKS_SINGLE              1









// #define    ECHO_CLIENT_DEMO  0

// #define mainSELECTED_APPLICATION ECHO_CLIENT_DEMO

/* This demo uses heap_3.c (the libc provided malloc() and free()). */

/*-----------------------------------------------------------*/
// extern void main_tcp_echo_client_tasks( void );


/*
 * Prototypes for the standard FreeRTOS application hook (callback) functions
 * implemented within this file.  See http://www.freertos.org/a00016.html .
 */
// void vApplicationMallocFailedHook( void );
// void vApplicationIdleHook( void );
// void vApplicationStackOverflowHook( TaskHandle_t pxTask,
//                                     char *pcTaskName );
// void vApplicationTickHook( void );
// void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer,
//                                     StackType_t **ppxIdleTaskStackBuffer,
//                                     uint32_t *pulIdleTaskStackSize );
// void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer,
//                                      StackType_t **ppxTimerTaskStackBuffer,
//                                      uint32_t *pulTimerTaskStackSize );


/*----------------------------------------------------------------------------*/
#ifdef __cplusplus
  }
#endif
#endif /* __MAIN_H */
