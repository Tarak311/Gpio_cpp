/*
 * spiclass.cpp
 *
 *  Created on: Jul 10, 2018
 *      Author: tarak
 */

#include "spiclass.h"
#include "../inc/spi.hh"
#include "../freeRTOS/inc/list.h"
#include "../inc/portmacro.h"

spi_class::spi_class(xLIST& ls,int* a,LPC_SPI_T* pSPI,SPI_BITS_T BITLN,SPI_CLOCK_MODE_T CLKM,SPI_DATA_ORDER_T DA)
{


	SPI_CONFIG_FORMAT_T spiformat;
	spiformat.bits = SPI_BITS_8;
	spiformat.clockMode = SPI_CLOCK_MODE0;
	spiformat.dataOrder = SPI_DATA_MSB_FIRST;
	Chip_SPI_SetFormat(pSPI, &(spiformat));
	this->spidev.spi_format=spiformat;

	//Chip_GPDMA_Init(LPC_GPDMA);
	SPI_DATA_SETUP_T transfer;
	transfer.fnBefFrame =  NULL;
	transfer.fnAftFrame =  NULL;
	transfer.fnBefTransfer = NULL;
	transfer.fnAftTransfer = NULL;
	this->spidev.spi_xf=transfer;
	/*this->spidev.spi_xf.fnAftFrame=NULL;
	this->spidev.spi_xf.fnAftTransfer=NULL;
	this->spidev.spi_xf.fnBefFrame=NULL;
	this->spidev.spi_xf.fnBefTransfer=NULL;*/
	Chip_SPI_SetMode(pSPI, SPI_MODE_SLAVE);




	this->lists = &ls;
	this->spidev.pointer = this;
	this->spidev.pSPI0 =pSPI;
}

spi_class::~spi_class() {
	// TODO Auto-generated destructor stub
}
void spi_class::spiRW()
{
	this->spidev.spi_xf.cnt = 0;
	this->spidev.spi_xf.length = 8;
	vListInitialiseItem(&(this->spidev.listitem)); 						//This initialize list item which is not of type MINI and set container to null ,it takes a pointer.
	this->spidev.spi_xf.pTxData = this->spi_tx_buf;
	this->spidev.spi_xf.pRxData = this->spi_rx_buf;
	bufferInit(this->spi_tx_buf,this->spi_rx_buf);
	this->spidev.spi_xfer_completed =0;
	vListInsertEnd(lists,&(this->spidev.listitem));						// This inserts item at list ends and both parameter are address of variables.
	listSET_LIST_ITEM_OWNER(&(this->spidev.listitem),&(this->spidev));
	assign_value_list_item(&(this->spidev.listitem));
	/*Chip_SPI_Int_FlushData(this->spidev->pSPI0);
	Chip_SPI_Int_RWFrames8Bits(this->spidev->pSPI0, this->spidev->spi_xf);
	Chip_SPI_Int_Enable(this->spidev->pSPI0);*/
}
