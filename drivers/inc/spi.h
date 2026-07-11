#ifndef SPI_H
#define SPI_H

#include <stdint.h>
#include "gpio.h"


typedef struct {
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t SR;
    volatile uint32_t DR;
    volatile uint32_t CRCPR;
    volatile uint32_t RXCRCR;
    volatile uint32_t TXCRCR;
    volatile uint32_t I2SCFGR;
    volatile uint32_t I2SPR;
} SPI_TypeDef;

typedef struct {
    uint32_t baudrate_prescaler; // Baud rate prescaler
    uint32_t cpol;                // Clock polarity
    uint32_t cpha;                // Clock phase
    uint32_t dff;                 // Data frame format (8-bit or 16-bit)
    uint32_t lsbfirst;            // LSB first or MSB first
} SPI_Config;


#define SPI1 ((SPI_TypeDef*) 0x40013000)
#define SPI2 ((SPI_TypeDef*) 0x40003800)
#define SPI3 ((SPI_TypeDef*) 0x40003C00)


void SPI_Init(SPI_TypeDef* SPIx, SPI_Config* config);
void SPI_transmit(SPI_TypeDef* SPIx, uint8_t* data, uint16_t size);
void SPI_receive(SPI_TypeDef* SPIx, uint8_t* data, uint16_t size);
void CS_Select(GPIO_TypeDef* port, uint32_t pin);
void CS_Deselect(GPIO_TypeDef* port, uint32_t pin);


#endif  // SPI_H