/**
  ******************************************************************************
  * File Name          : freertos.h
  * Description        : This file provides header for the configuration
  *                      of FreeRTOS and its services.
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FREERTOS_H
#define __FREERTOS_H
#ifdef __cplusplus
  extern "C" {
#endif


/* Spetial defines  ----------------------------------------------------------*/
#define _POSIX_C_SOURCE 199309L


/* Includes ------------------------------------------------------------------*/
#include "main.h"



/* Exported functions prototypes ---------------------------------------------*/
void FreeRTOS_Run(void);

/* Services ------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/
#ifdef __cplusplus
  }
#endif
#endif /*__FREERTOS_H */

