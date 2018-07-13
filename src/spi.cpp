/*
 * spi.cpp
 *
 *  Created on: Jul 10, 2018
 *      Author: Tarak Patel
 */


#include "../inc/spi.hh"
#include "../freeRTOS/inc/list.h"
#include "../src/spiclass.h"




void bufferInit(uint8_t *tx_buf, uint8_t *rx_buf)
{
	uint16_t i;


	for (i = 0; i < BUFFER_SIZE; i++) {
		tx_buf[i] = i;
		rx_buf[i] = 0xBB;
	}
}



static int list_number = 0;
void assign_value_list_item(xListItem* item)
{
	listSET_LIST_ITEM_VALUE(item,list_number++);
}

