#ifndef SPI_H
#define SPI_H

#include <stdint.h>
#include "gpio.h"

// SPI register structure, used to access the SPI peripheral registers directly
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

// SPI configuration structure, used to more easily initialize the SPI peripheral with specific settings
typedef struct {
    GPIO_TypeDef* port;
    uint32_t sck_pin;
    uint32_t mosi_pin;
    uint32_t miso_pin;
    uint32_t nss_pin;
    uint32_t alternate_function;
    uint32_t baudrate_prescaler;
    uint32_t cpol;
    uint32_t cpha;
    uint32_t dff;
    uint32_t lsbfirst;
} SPI_Config;

//STM32 F446RE SPI base addresses
#define SPI1 ((SPI_TypeDef*) 0x40013000)
#define SPI2 ((SPI_TypeDef*) 0x40003800)
#define SPI3 ((SPI_TypeDef*) 0x40003C00)


void SPI_Init(SPI_TypeDef* SPIx, SPI_Config* config);
void SPI_transmit(SPI_TypeDef* SPIx, uint8_t* data, uint16_t size);
void SPI_receive(SPI_TypeDef* SPIx, uint8_t* data, uint8_t byte_out, uint16_t size);
uint8_t SPI_TransferByte(SPI_TypeDef* SPIx, uint8_t byte_out);
void CS_Select(GPIO_TypeDef* port, uint32_t pin);
void CS_Deselect(GPIO_TypeDef* port, uint32_t pin);


#endif  // SPI_H