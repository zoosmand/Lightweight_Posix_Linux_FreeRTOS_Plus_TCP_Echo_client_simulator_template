/**
  ******************************************************************************
  * File Name          : console.h
  * Description        : This file provides definitioan and microses for console
  *                      output
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CONSOLE_H
#define __CONSOLE_H

#ifdef __cplusplus
  extern "C" {
#endif


/* Includes ------------------------------------------------------------------*/
#include "main.h"


/* Exported functions prototypes ---------------------------------------------*/
void console_init(void);
void console_print(const char *fmt, ...);


/*----------------------------------------------------------------------------*/
#ifdef __cplusplus
  }
#endif
#endif /* __CONSOLE_H */
