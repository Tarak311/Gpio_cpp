/*
===============================================================================
 Name        : Tarak0.cpp
 Author      : Tarak Patel
 Version     :0.0.1
 Copyright   : GPL
 Description : RTOS program which comunicate with raspberry  pi
  	  	  	   via spi bus and reads temprature value by i2c sensor
===============================================================================
 */

#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#endif
#include "../inc/gpiopin.hh"
#include "../inc/spi.hh"
#include "../freeRTOS/inc/list.h"
#include "../inc/portmacro.h"
#include "spiclass.h"

//#include <cr_section_macros.h>

// TODO: insert other include files here

// TODO: insert other definitions and declarations here
//using namespace spi;
static xLIST list;

#include "../freeRTOS/inc/FreeRTOS.h"
#include "../freeRTOS/inc/task.h"
#include "../freeRTOS/inc/queue.h"
#include "../freeRTOS/inc/semphr.h"
#define LPC_GPIO                  ((LPC_GPIO_T             *) LPC_GPIO0_BASE)
#define LPC_GPIO1                 ((LPC_GPIO_T             *) LPC_GPIO1_BASE)
#define LPC_GPIO2                 ((LPC_GPIO_T             *) LPC_GPIO2_BASE)
#define LPC_GPIO3                 ((LPC_GPIO_T             *) LPC_GPIO3_BASE)
#define LPC_GPIO4                 ((LPC_GPIO_T             *) LPC_GPIO4_BASE)


///////////*******************All the handles**********************///////////
//xLIST* list;

bool input=false;
bool output=true;
int j;



xSemaphoreHandle   mu=0;
xSemaphoreHandle   mu1=0;
xSemaphoreHandle   muf=0;
xSemaphoreHandle   the_signal=0;
xSemaphoreHandle   the_signal2=0;
xQueueHandle       	queue_handle = 0;


///////////*******************All the functions**********************///////////


/* Sets up system hardware */
static void prvSetupHardware(void)
{
	SystemCoreClockUpdate();
	Board_Init();
	Board_LED_Set(0, false);
	Board_SPI_Init(false);

	Chip_SPI_Init(LPC_SPI);
	NVIC_EnableIRQ(SPI_IRQn);
}

/* LED1 toggle thread */
static void blink(void *pvParameters) {


	gpio_pin_port pin1(LPC_GPIO ,0,2,output,&mu);
	gpio_pin_port pin2(LPC_GPIO ,0,3,output,&mu);
	int b;
	bool pin1_status=true;

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

	while (1)
	{
		if (xSemaphoreTake(mu,1000))
		{
			/* blinks led in cycle*/
			if (ledg && ledb){ledg=false;ledb=true;ledr=true;}else{if(ledr && ledb){ledb=false;ledr=true;ledg=true;}else{ledr=false;ledg=true;ledb=true;}}
			xSemaphoreGive(mu);
		}
		vTaskDelay(configTICK_RATE_HZ / 2);
	}
}
/* LED2 toggle thread */

static void bottonreadandrelay(void *pvParameters) {


	gpio_pin_port pin_input1( LPC_GPIO ,1,26,input,&muf);
	gpio_pin_port pin_input2( LPC_GPIO ,1,25,input,&muf);

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
				int v=6;
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
/*static void spi_data(void *pvParameters)
{

	int o,p;
	o=43;
	spi_class spisend(list,&o,LPC_SPI,SPI_BITS_8,SPI_CLOCK_MODE0,SPI_DATA_MSB_FIRST);
	while (1)
	{
		if (spi::spi_xfer_completed) { spi::spi_xfer_completed=0;appSPIRun();}

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
	/*xTaskCreate(spi_data, (signed char *) "TX/RX function",
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
void spi_setup(void)
{

	int o;o=2;
	Chip_SPI_Int_FlushData(LPC_SPI);
	spi_class spi_main_thread(list,&o,LPC_SPI,SPI_BITS_8,SPI_CLOCK_MODE0,SPI_DATA_MSB_FIRST);
	spi_main_thread.spiRW();
	spi_class::spi_data* sdev = (spi_class::spi_data*)listGET_OWNER_OF_HEAD_ENTRY(&list);
	Chip_SPI_Int_RWFrames8Bits(LPC_SPI, &(sdev->spi_xf));
	Chip_SPI_Int_Enable(LPC_SPI);

}
int main()
{


	prvSetupHardware();

	mu = xSemaphoreCreateMutex();
	mu1 = xSemaphoreCreateMutex();
	vSemaphoreCreateBinary(the_signal);
	vSemaphoreCreateBinary(the_signal2);
	queue_handle = xQueueCreate(3,sizeof(int));



	setup();
	vListInitialise(&list);
	SPI_CONFIG_FORMAT_T  spi_format;
	spi_format.bits = SPI_BITS_8;
		spi_format.clockMode = SPI_CLOCK_MODE0;
		spi_format.dataOrder = SPI_DATA_MSB_FIRST;
		Chip_SPI_SetFormat(LPC_SPI, &spi_format);
	spi_setup();

	vTaskStartScheduler();

	return 1;

}

void SPI_IRQHandler(void)
{

	Chip_SPI_Int_Disable(LPC_SPI);	/* Disable all interrupt */

	spi_class::spi_data* spidev = (spi_class::spi_data*)listGET_OWNER_OF_HEAD_ENTRY(&list);
	spi_class::spi_data* spidev_next = (spi_class::spi_data*)(spidev->listitem.pxNext);
	if(!(spidev == spidev_next))
	{
		if (spidev->spi_xfer_completed)
		{
			uxListRemove(&(spidev->listitem));
			spidev=spidev_next;
			spidev_next = (spi_class::spi_data*)(spidev->listitem.pxNext);
		}
		Chip_SPI_Int_FlushData(spidev->pSPI0);
		Chip_SPI_Int_RWFrames8Bits(LPC_SPI,&(spidev->spi_xf));

		if ((spidev->spi_xf.cnt) < (spidev->spi_xf.length))
		{
			Chip_SPI_Int_Enable(LPC_SPI);	/* enable all interrupts */
		}
		else if(spidev)
		{
			spidev->spi_xfer_completed = 1;
			uxListRemove(&(spidev->listitem));
			Chip_SPI_Int_Enable(LPC_SPI);
		}
	}
	else
	{
		if (spidev->spi_xfer_completed){}
		else if ((spidev->spi_xf.cnt) < (spidev->spi_xf.length)) {Chip_SPI_Int_Enable(LPC_SPI);}
		else {spidev->spi_xfer_completed = 1;}
	}

}


/**
 * @}
 */
