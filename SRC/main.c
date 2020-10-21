/* Include files */
#include <stdio.h>
#include <stdlib.h>
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"

#include "GPIO_STM32F10x.h"


/* The task functions prototype*/
void vTask1( void *pvParameters );
void vTask2( void *pvParameters );
void Si7021_Task( void *pvParameters );
  
/* Task parameter to be sent to the task function */
const char *pvTask1  = "Task1 is running.";
const char *pvTask2  = "Task2 is running.";
const char *pvSi7021_Task = "Si7021 Task is running.";
/* Extern functions */
extern void SystemInit(void);
extern void SystemCoreClockUpdate(void);
extern int stdout_init (void);


static void vTimerCallback5SecExpired(xTimerHandle pxTimer);

/*-----------------------------------------------------------*/
/* Global semaphore variable */
SemaphoreHandle_t xSemaphore = NULL; 

/* function */





int 
main( void )
{
    /* Board initializations */
    SystemInit();
    /* This function initializes the MCU clock, PLL will be used to generate Main MCU clock */  
    SystemCoreClockUpdate();
    /* Initialize the serial I/O(console ), making standard output to be send to USART1 */
    stdout_init();
  
  
    //GPIO_PinConfigure(GPIOA,4,GPIO_OUT_OPENDRAIN,GPIO_MODE_OUT2MHZ);
    //GPIO_PinWrite(GPIOA,4,0);
  
    GPIO_PinConfigure(GPIOC,13,GPIO_OUT_OPENDRAIN,GPIO_MODE_OUT2MHZ);
    //GPIO_PinWrite(GPIOC,13,0);
  
  
  /* TODO: Si7021 Initialization Call */
  
  
    xTimerHandle timerHndl1Sec;
    timerHndl1Sec = xTimerCreate(
      "timer5Sec", /* name */
      pdMS_TO_TICKS(5000), /* period/time */
      pdTRUE, /* auto reload */
      (void*)0, /* timer ID */
      vTimerCallback5SecExpired); /* callback */
     if (timerHndl1Sec==NULL) 
      {
        for(;;); /* failure! */
      }
  
   xTimerStart( timerHndl1Sec, 0 );
  
    printf("Initialization is done.\r\n");
    /* Create one of the two tasks. */
    xTaskCreate(vTask1, /* Pointer to the function that implements the task. */
        "Task 1", /* Text name for the task.  This is to facilitate debugging only. */
        configMINIMAL_STACK_SIZE, /* Stack depth in words. */
	(void*)pvTask1,		/* We are not using the task parameter. */
	1,			/* This task will run at priority 1. */
	NULL );		/* We are not using the task handle. */
	
        /* Create the other task in exactly the same way. */
    xTaskCreate( vTask2, "Task 2", configMINIMAL_STACK_SIZE, (void*)pvTask2, 1, NULL );
  
    /* Create the other task in exactly the same way. */
    xTaskCreate( Si7021_Task, "Si7021_Task", configMINIMAL_STACK_SIZE, (void*)pvSi7021_Task, 1, NULL );
  
        /* Create a binary semaphore */
	xSemaphore = xSemaphoreCreateBinary();
	/* make the semaphore token available for the first time */
  xSemaphoreGive( xSemaphore);
	/* Start the scheduler so our tasks start executing. */
	vTaskStartScheduler();
	/* If all is well we will never reach here as the scheduler will now be
	running.  If we do reach here then it is likely that there was insufficient
	heap available for the idle task to be created. */
	for( ;; );
}
/*-----------------------------------------------------------*/
void vTask1( void *pvParameters )
{
char *pcTaskName = (char *) pvParameters;
	/* Task is implemented in an infinite loop. */
	for( ;; )
	{
		/* Take semaphore */
		xSemaphoreTake(xSemaphore,(TickType_t) portMAX_DELAY);
		/* Print out the name of this task. */
	  	printf( "%s\r\n",pcTaskName );
		/* Give semaphore */
		xSemaphoreGive(xSemaphore);
		/* Delay for a period. */
		vTaskDelay( 2000 / portTICK_PERIOD_MS );
	}
}
/*-----------------------------------------------------------*/
void vTask2( void *pvParameters )
{
char *pcTaskName = (char *) pvParameters;
	/* Task is implemented in an infinite loop. */
	for( ;; )
	{
		/* Take semaphore */
		xSemaphoreTake(xSemaphore,(TickType_t) portMAX_DELAY);
		/* Print out the name of this task. */
	  	printf( "%s\r\n",pcTaskName );
		/* Give semaphore */
		xSemaphoreGive(xSemaphore);
		/* Delay for a period. */
		vTaskDelay( 2000 / portTICK_PERIOD_MS );
	}
} 



void Si7021_Task( void *pvParameters )
{
  char *pcTaskName = (char *) pvParameters;
	/* Task is implemented in an infinite loop. */
	for( ;; )
	{
		/* Take semaphore */
		xSemaphoreTake(xSemaphore,(TickType_t) portMAX_DELAY);
		/* Print out the name of this task. */
	  	printf( "%s\r\n",pcTaskName );
    
    
    /* Read Temperature and print */
    
    
		/* Give semaphore */
		xSemaphoreGive(xSemaphore);
		/* Delay for a period. */
		vTaskDelay( 2000 / portTICK_PERIOD_MS );
	}
} 




static void vTimerCallback5SecExpired(xTimerHandle pxTimer) {
  /* this timer callback turns off the green LED */
  //GPIO_PortSet(BOARD_INITPINS_LCD_BACKGROUND_LIGHT, 1<<BOARD_INITPINS_LCD_BACKGROUND_LIGHT_PIN); /* turn off LCD Light */
  
  /* put a blinky code */
    static uint32_t blink_led;
   GPIO_PinWrite(GPIOC,13,blink_led = ~blink_led);
}


/*==================================================================*/

