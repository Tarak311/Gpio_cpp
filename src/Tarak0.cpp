/*
===============================================================================
 Name        : main.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#endif
#include "gpiopin.hh"

//#include <cr_section_macros.h>

// TODO: insert other include files here

// TODO: insert other definitions and declarations here


#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#define LPC_GPIO                  ((LPC_GPIO_T             *) LPC_GPIO0_BASE)
#define LPC_GPIO1                 ((LPC_GPIO_T             *) LPC_GPIO1_BASE)
#define LPC_GPIO2                 ((LPC_GPIO_T             *) LPC_GPIO2_BASE)
#define LPC_GPIO3                 ((LPC_GPIO_T             *) LPC_GPIO3_BASE)
#define LPC_GPIO4                 ((LPC_GPIO_T             *) LPC_GPIO4_BASE)


///////////*******************All the handles**********************///////////


bool input=false;
bool output=true;
int j;

typedef struct
{

	uint32_t 	gpio_p;
	uint8_t     gpio_port;
	LPC_GPIO_T  *base;

    bool s;
}gpiopin;
xSemaphoreHandle   mu=0;
xSemaphoreHandle   mu1=0;
xSemaphoreHandle   muf=0;
xSemaphoreHandle   the_signal=0;
xSemaphoreHandle   the_signal2=0;
xQueueHandle       	queue_handle = 0;
gpiopin            gp[4];

///////////*******************All the functions**********************///////////

	void gpio_pin_set(gpiopin gp)
		{
        lock lockf(&mu); // resouce aqq is init tecquniqe
		Chip_GPIO_WritePortBit(gp.base,gp.gpio_p,gp.gpio_port,1);
		}
	void gpio_pin_reset(gpiopin gp)
		{
		lock lockf(&mu); // this will deleted if function is exited no matter what execption or no execption
		Chip_GPIO_WritePortBit(gp.base,gp.gpio_p,gp.gpio_port,0);
		}
	bool gpio_pin_read(gpiopin gp)
		{
		lock lockf(&mu);
		return	Chip_GPIO_ReadPortBit(gp.base,gp.gpio_port,gp.gpio_p);
		}

		/* Sets up system hardware */
static void prvSetupHardware(void)
{
	SystemCoreClockUpdate();
	Board_Init();

	/* Initial LED0 state is off */
	Board_LED_Set(0, false);
}

/* LED1 toggle thread */
static void blink(void *pvParameters) {

	//gpio_pin_port* pin_output1 =new gpio_pin_port(LPC_GPIO ,1,27,output,&mu);
	//gpio_pin_port* pin_output2 =new gpio_pin_port(LPC_GPIO ,1,28,output,&mu);
    //gpio_pin_port* pin_output3 =new gpio_pin_port(LPC_GPIO ,1,27,input,&mu);
	gpio_pin_port pin1(LPC_GPIO ,0,2,output,&mu);
	gpio_pin_port pin2(LPC_GPIO ,0,3,output,&mu);
	int b;
	bool pin1_status=true;
	//bool pin_op1=true;
		while (1) {
		//if (xSemaphoreTake(mu1,1000))
        {
        	//if(pin_op1) {pin_op1=false;*pin_output1=true;*pin_output2=false;} else {pin_op1=true;*pin_output1=false;*pin_output2=true;};
        	if(pin1_status){pin1_status=false;pin1=true;pin2=false;}else{pin1_status=true;pin1=false;pin2=true;};
        	//xSemaphoreGive(mu1);
        }
		/* About a 3Hz on/off toggle rate */
       xQueueReceive(queue_handle,&b,10);
        vTaskDelay(configTICK_RATE_HZ /b);
	}
}
static void blink2(void *pvParameters)
{
	gpio_pin_port ledr( LPC_GPIO ,0,22,output,&muf);
	gpio_pin_port ledg( LPC_GPIO ,3,25,output,&muf);
	gpio_pin_port ledb( LPC_GPIO ,3,26,output,&muf);
	//bool tempg;bool tempr;bool tempb;
	while (1)
	{
		 if (xSemaphoreTake(mu,1000))
		 {
		//tempg=ledg;tempr=ledr;tempb=ledb;
          if (ledg && ledb){ledg=false;ledb=true;ledr=true;}else{if(ledr && ledb){ledb=false;ledr=true;ledg=true;}else{ledr=false;ledg=true;ledb=true;}}
          xSemaphoreGive(mu);
          }/* About a 3Hz on/off toggle rate */
         vTaskDelay(configTICK_RATE_HZ / 2);
	}
}
/* LED2 toggle thread */

static void bottonreadandrelay(void *pvParameters) {

    //gpio_pin_port* pin_input;
    gpio_pin_port pin_input1( LPC_GPIO ,1,26,input,&muf);
    gpio_pin_port pin_input2( LPC_GPIO ,1,25,input,&muf);
//    pin_input[1]=pin_input1;
  //  pin_input[2]=pin_input2;
	while (1) {

		//if (xSemaphoreTake(mu1,1000))
		  {



	    if(pin_input1)
	    {
	    	int o=14;
	    	xQueueSend(queue_handle,&o,4000);
	    }
	    if(pin_input2)
		{
	    	int v=8;
	    	xQueueSend(queue_handle,&v,4000);
           }
	    else
	    {
	    	int h=1;
	    	xQueueSend(queue_handle,&h,4000);
	    }
		  }
		// xSemaphoreGive(mu);
		/* About a 7Hz on/off toggle rate */
		vTaskDelay(configTICK_RATE_HZ/4);
	}
}
/*static void processandrelay(void *pvParameters)
{

int o;
int p;
	while (1) {

    if  (xSemaphoreTake(the_signal,portMAX_DELAY))
        		{
    			o=10;
    			//xQueueSend(queue_handle,&o,4000);
        		}
    //use isr intr insted of this..
    else if (xSemaphoreTake(the_signal2,portMAX_DELAY))
            		{
        			 p=8;
        			//xQueueSend(queue_handle,&p,4000);
            		};


		//vTaskDelay(configTICK_RATE_HZ / 100); no need
	}
}*/
/*static void aimptask(void *pvParameters) {
	while (1) {
    if  (xSemaphoreTake(the_signal,portMAX_DELAY))
        		{
 // TASK
        		};

		vTaskDelay(configTICK_RATE_HZ / 100);
	}
}*/
/* UART (or output) thread */
static void vUARTTask(void *pvParameters) {
	int tickCnt = 0;

	while (1) {
		DEBUGOUT("Tick: %d\r\n", tickCnt);
		tickCnt++;

		/* About a 1s delay here */
		vTaskDelay(configTICK_RATE_HZ);
	}
}
void setup()
{

    Chip_GPIO_WriteDirBit(LPC_GPIO, 3, 26, true);
	xTaskCreate(blink2, (signed char *) "blink2",
					configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY + 1UL),
					(xTaskHandle *) NULL);
	xTaskCreate(bottonreadandrelay, (signed char *) "bottonreadandrelay",
					configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY + 1UL),
					(xTaskHandle *) NULL);
	/*xTaskCreate(processandrelay, (signed char *) "processandrelay",
						configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY + 1UL),
						(xTaskHandle *) NULL);*/
    xTaskCreate(blink, (signed char *) "blink",
						configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY + 1UL),
						(xTaskHandle *) NULL);
		/* UART output thread, simply counts seconds */
		xTaskCreate(vUARTTask, (signed char *) "vTaskUart",
					configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY + 1UL),
					(xTaskHandle *) NULL);

}
/*****************************************************************************
 * Public functions
 ****************************************************************************/

/**
 * @brief	main routine for FreeRTOS blinky example
 * @return	Nothing, function should not exit
 */
int main()
{

   prvSetupHardware();
mu = xSemaphoreCreateMutex();
mu1 = xSemaphoreCreateMutex();
vSemaphoreCreateBinary(the_signal);
vSemaphoreCreateBinary(the_signal2);
queue_handle = xQueueCreate(3,sizeof(int));
	/* LED1 toggle thread */
   setup();
	/* Start the scheduler */
	vTaskStartScheduler();

	/* Should never arrive here */
return 1;
}


/**
 * @}
 */
