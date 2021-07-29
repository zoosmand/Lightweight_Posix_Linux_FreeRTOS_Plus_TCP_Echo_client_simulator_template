/**
  ******************************************************************************
  * File Name          : freertos.h
  * Description        : This file provides header ECHO client service
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SRV_ECHO_CLIENT
#define __SRV_ECHO_CLIENT

#ifdef __cplusplus
  extern "C" {
#endif


/* Includes ------------------------------------------------------------------*/
#include "main.h"



/* Exported functions prototypes ---------------------------------------------*/
void vStartTCPEchoClientTasks_SingleTasks(uint16_t usTaskStackSize, UBaseType_t uxTaskPriority);
BaseType_t xAreSingleTaskTCPEchoClientsStillRunning(void);




/*----------------------------------------------------------------------------*/
#ifdef __cplusplus
  }
#endif
#endif /* __SRV_ECHO_CLIENT */
