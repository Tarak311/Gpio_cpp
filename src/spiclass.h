/*
 * spiclass.h
 *
 *  Created on: Jul 10, 2018
 *      Author: tarak
 */

#ifndef SPICLASS_H_
#define SPICLASS_H_

class spi_class {
public:
	spi_class();
	virtual ~spi_class();
private:
	uint8_t spi_tx_buf[BUFFER_SIZE];
	uint8_t spi_rx_buf[BUFFER_SIZE];
	SPI_CONFIG_FORMAT_T spi_format;
    SPI_DATA_SETUP_T spi_xf;
	int spi_xfer_completed;
};

#endif /* SPICLASS_H_ */
