/**
  ******************************************************************************
  * File Name          : console.c
  * Description        : This file provides code for console output
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "console.h"

/* Private variables ---------------------------------------------------------*/
SemaphoreHandle_t xStdioMutex;
StaticSemaphore_t xStdioMutexBuffer;





/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/**
  * @brief  Initialize console.
  * @return None
  */
void console_init(void) {
    xStdioMutex = xSemaphoreCreateMutexStatic(&xStdioMutexBuffer);
}


/**
  * @brief  Prints message to console.
  * @param  fmt: Buffer contains text od data to be printed
  * @return None
  */
void console_print(const char *fmt, ...) {
    va_list vargs;
    va_start(vargs, fmt);

    xSemaphoreTake(xStdioMutex, portMAX_DELAY);

    vprintf(fmt, vargs);
    va_end(vargs);
}
