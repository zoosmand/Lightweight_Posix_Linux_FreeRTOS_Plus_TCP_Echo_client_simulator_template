/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"



/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/**
  * @brief  The application entry point.
  * @return int
  */
int main(void) {
  /* Do not include trace code when performing a code coverage analysis. */
  #if projCOVERAGE_TEST != 1
    /* Initialise the trace recorder.  Use of the trace recorder is optional.
    See http://www.FreeRTOS.org/trace for more information. */
    vTraceEnable( TRC_START );

    /* Start the trace recording - the recording is written to a file if
    configASSERT() is called. */
    printf("\r\nTrace started.\r\nThe trace will be dumped to disk if a call to configASSERT() fails.\r\n");
    printf("\r\nThe trace will be dumped to disk if Enter is hit.\r\n");
    uiTraceStart();
  #endif

  console_init();

  FreeRTOS_Run();

  exit(-1);
}
