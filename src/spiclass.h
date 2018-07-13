/*
 * spiclass.h
 *
 *  Created on: Jul 10, 2018
 *      Author: Tarak
 */
#include "board.h"
#include "stdio.h"
#include "../inc/spi.hh"
#include "../freeRTOS/inc/list.h"
#include "../inc/portmacro.h"



class spi_class
{

public:

	spi_class(xLIST& ls,int* a,LPC_SPI_T* pSPI,SPI_BITS_T BITLN,SPI_CLOCK_MODE_T CLKM,SPI_DATA_ORDER_T DA);
	virtual ~spi_class();
	void spiRW();
	struct spi_data
		{
			LPC_SPI_T*			  pSPI0;
			SPI_CONFIG_FORMAT_T   spi_format;
			SPI_DATA_SETUP_T     spi_xf;
			void* 				  pointer; //INFO: This pointer points to parent class.
			xListItem			  listitem;//TODO: Initialize list item in constructor function and add it to list.
			int spi_xfer_completed;
		}spidev;
private:

	uint8_t spi_tx_buf[BUFFER_SIZE];
	uint8_t spi_rx_buf[BUFFER_SIZE];
	uint8_t spi_aux_tx_buf[BUFFER_SIZE];
	uint8_t spi_aux_rx_buf[BUFFER_SIZE];

	 xLIST *lists;
	int spi_xfer_completed;
};


