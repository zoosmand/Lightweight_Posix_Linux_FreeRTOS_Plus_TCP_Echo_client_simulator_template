/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : This file provides code for the configuration
  *                      of FreeRTOS.
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "freertos.h"

/* Private variables ---------------------------------------------------------*/
static BaseType_t xTraceRunning = pdTRUE;

/* Private function prototypes -----------------------------------------------*/
static void traceOnEnter(void);
static void prvSaveTraceFile(void);
/* Time at start of day (in ns). */
static unsigned long ulStartTimeNs;

/* Global variables ---------------------------------------------------------*/
StackType_t uxTimerTaskStack[configTIMER_TASK_STACK_DEPTH];





/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/**
  * @brief  Runs services and FreeRTOS. The point where FreeRTOS starts.
  * @return None
  */
void FreeRTOS_Run(void) {

  /* run services */
  srvEchoClient();
  
  /* run the scheduler */
  vTaskStartScheduler();

}


/**
  * @brief  Gets called when memory allocation fails.
  * @return None
  */
void vApplicationMallocFailedHook(void) {
  vAssertCalled( __FILE__, __LINE__ );
}


/**
  * @brief  Gets called when the runtime idles.
  * @return None
  */
void vApplicationIdleHook(void) {
  usleep(15000);
  traceOnEnter();
}


/**
  * @brief  Gets called when stack overflow.
  * @return None
  */
void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName) {
  (void)pcTaskName;
  (void)pxTask;

  vAssertCalled( __FILE__, __LINE__ );
  while(1);
}


/**
  * @brief  Gets called on each system tick.
  * @return None
  */
void vApplicationTickHook(void) {
  //
}


/**
  * @brief  Gets called at the application start.
  * @return None
  */
void vApplicationDaemonTaskStartupHook(void) {
  //
}


/**
  * @brief  Gets called on an assertion occured.
  * @return None
  */
void vAssertCalled(const char * const pcFileName, unsigned long ulLine ) {
  static BaseType_t xPrinted = pdFALSE;
  volatile uint32_t ulSetToNonZeroInDebuggerToContinue = 1;

  /* Parameters are not used. */
  (void)ulLine;
  (void)pcFileName;

  /* --------- !!! --------- */
  taskENTER_CRITICAL();
  /* --------- !!! --------- */
  /* Stop the trace recording. */
  if(xPrinted == pdFALSE) {
    xPrinted = pdTRUE;

    if(xTraceRunning == pdTRUE) {
        prvSaveTraceFile();
    }
  }

  /* You can step out of this function to debug the assertion by using
  the debugger to set ulSetToNonZeroInDebuggerToContinue to a non-zero
  value. */
  while(ulSetToNonZeroInDebuggerToContinue == 0) {
    __asm volatile ( "NOP" );
    __asm volatile ( "NOP" );
  }
  /* --------- !!! --------- */
  taskEXIT_CRITICAL();
  /* --------- !!! --------- */
}



/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#if configSUPPORT_STATIC_ALLOCATION == 1

  static StaticTask_t xIdleTaskTCBBuffer;
  static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

  /**
    * @brief  Gets called when configSUPPORT_STATIC_ALLOCATION equals to 1 and is required
    *         for static memory allocation support.
    * @return None
    */
  void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize ) {
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
    *ppxIdleTaskStackBuffer = &xIdleStack[0];
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  }

  
  /**
    * @brief  Gets called when configSUPPORT_STATIC_ALLOCATION equals to 1 and is required
    *         for static memory allocation support.
    * @return None
    */
  void vApplicationGetTimerTaskMemory (StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize) {
    /* Timer task control block and stack */
    static StaticTask_t Timer_TCB;
    static StackType_t  Timer_Stack[configTIMER_TASK_STACK_DEPTH];

    *ppxTimerTaskTCBBuffer   = &Timer_TCB;
    *ppxTimerTaskStackBuffer = &Timer_Stack[0];
    *pulTimerTaskStackSize   = (uint32_t)configTIMER_TASK_STACK_DEPTH;
  }

#endif






/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

/**
  * @brief  Begins runtime tracing.
  * @return None
  */
void traceOnEnter() {
  int ret;
  struct timeval tv = {0L, 0L};
  fd_set fds;
  FD_ZERO(&fds);
  FD_SET(0, &fds);
  ret = select(1, &fds, NULL, NULL, &tv);
  if (ret > 0) {
    if(xTraceRunning == pdTRUE) {
      prvSaveTraceFile();
    }
    /* clear the buffer */
    char buffer[200];
    read(1, &buffer, 200);
  }
}


/**
  * @brief  Saves collected trace to a file.
  * @return None
  */
static void prvSaveTraceFile(void) {
  /* Tracing is not used when code coverage analysis is being performed. */
  #if projCOVERAGE_TEST != 1
      FILE * pxOutputFile;
    vTraceStop();

    pxOutputFile = fopen("Trace.dump", "wb");

    if(pxOutputFile != NULL) {
      fwrite( RecorderDataPtr, sizeof( RecorderDataType ), 1, pxOutputFile );
      fclose( pxOutputFile );
      printf( "\r\nTrace output saved to Trace.dump\r\n" );
    } else {
      printf( "\r\nFailed to create trace dump file\r\n" );
    }
  #endif /* if ( projCOVERAGE_TEST != 1 ) */
}


/**
  * @brief  Prints log messges to console.
  * @param pcFormat: pointer to abuffer to print
  * @return None
  */
void vLoggingPrintf(const char *pcFormat, ...) {
  va_list arg;

  va_start( arg, pcFormat );
  vprintf( pcFormat, arg );
  va_end( arg );
}





/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/


void vConfigureTimerForRunTimeStats(void) {
  struct timespec xNow;

	clock_gettime(CLOCK_MONOTONIC, &xNow);
	ulStartTimeNs = xNow.tv_sec * 1000000000ul + xNow.tv_nsec;
}

unsigned long ulGetRunTimeCounterValue(void) {
  struct timespec xNow;

	/* Time at start. */
	clock_gettime(CLOCK_MONOTONIC, &xNow);

	return xNow.tv_sec * 1000000000ul + xNow.tv_nsec - ulStartTimeNs;
}
