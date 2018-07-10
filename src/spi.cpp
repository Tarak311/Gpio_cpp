/*
 * spi.cpp
 *
 *  Created on: Jul 10, 2018
 *      Author: tarak
 */


#include "../inc/spi.h"

//using namespace spi;

using namespace spi;
void bufferInit(uint8_t *tx_buf, uint8_t *rx_buf)
				{
					uint16_t i;


					for (i = 0; i < BUFFER_SIZE; i++) {
						tx_buf[i] = i;
						rx_buf[i] = 0xBB;
					}
				}



void appSPIRun(void)
				{
					spi_xf.cnt = 0;
					spi_xf.length = BUFFER_SIZE;
					spi_xf.pTxData = spi_tx_buf;
					spi_xf.pRxData = spi_rx_buf;
					bufferInit(spi_tx_buf, spi_rx_buf);

				#if (SPI_TRANSFER_MODE_SEL == SPI_POLLING_SEL)
					Chip_SPI_RWFrames_Blocking(LPC_SPI, &spi_xf);
				#else
					spi_xfer_completed = 0;

					Chip_SPI_Int_FlushData(LPC_SPI);		/* flush dummy data from SPI FiFO */
					Chip_SPI_Int_RWFrames8Bits(LPC_SPI, &spi_xf);
					Chip_SPI_Int_Enable(LPC_SPI);			/* enable interrupt */
				#endif
				}

void SPI_IRQHandler(void)
			{
				Chip_SPI_Int_Disable(LPC_SPI);	/* Disable all interrupt */
				Chip_SPI_Int_RWFrames8Bits(LPC_SPI, &spi_xf);

				if (spi_xf.cnt < spi_xf.length) {
					Chip_SPI_Int_Enable(LPC_SPI);	/* enable all interrupts */
				}
				else {
					spi_xfer_completed = 1;
				}
			}




void program_init(void)
			{
				#if (SPI_MODE_SEL == SPI_MASTER_MODE_SEL)
					Board_SPI_Init(true);
				#else
					Board_SPI_Init(false);
				#endif

					/* SPI initialization */
					Chip_SPI_Init(LPC_SPI);
					spi_format.bits = SPI_BITS_8;
					spi_format.clockMode = SPI_CLOCK_MODE0;
					spi_format.dataOrder = SPI_DATA_MSB_FIRST;
					Chip_SPI_SetFormat(LPC_SPI, &spi_format);


					//Chip_GPDMA_Init(LPC_GPDMA);

				#if (SPI_MODE_SEL == SPI_MASTER_MODE_SEL)
					spi_xf.fnBefFrame =  Board_SPI_AssertSSEL;
					spi_xf.fnAftFrame =  Board_SPI_DeassertSSEL;
					spi_xf.fnBefTransfer = NULL;
					spi_xf.fnAftTransfer = NULL;
					Chip_SPI_SetMode(LPC_SPI, SPI_MODE_MASTER);
				#else
					spi_xf.fnBefFrame =  NULL;
					spi_xf.fnAftFrame =  NULL;
					spi_xf.fnBefTransfer = NULL;
					spi_xf.fnAftTransfer = NULL;
					Chip_SPI_SetMode(LPC_SPI, SPI_MODE_SLAVE);

				#endif

				#if (SPI_TRANSFER_MODE_SEL == SPI_INTERRUPT_SEL)
					/* Setting SPI interrupt */
					NVIC_EnableIRQ(SPI_IRQn);
				#endif
			}
