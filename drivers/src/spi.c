#include "../inc/spi.h"
#include "../inc/rcc.h"
#include "../inc/gpio.h"

void SPI_Init(SPI_TypeDef* SPIx, SPI_Config* config) {
    RCC->RCC_APB2ENR |= (1 << 12); // Enable SPI1 clock

    GPIO_PinConfig sck = {
        .pin = config->sck_pin,
        .mode = GPIO_MODE_ALTERNATE,
        .output_type = GPIO_OUTPUT_PUSH_PULL,
        .speed = GPIO_SPEED_HIGH,
        .pull = GPIO_PULL_NONE,
        .af = config->af 
    };
    GPIO_PinConfig mosi = {
        .pin = config->mosi_pin,
        .mode = GPIO_MODE_ALTERNATE,
        .output_type = GPIO_OUTPUT_PUSH_PULL,
        .speed = GPIO_SPEED_HIGH,
        .pull = GPIO_PULL_NONE,
        .af = config->af
    };
    GPIO_PinConfig miso = {
        .pin = config->miso_pin,
        .mode = GPIO_MODE_ALTERNATE,
        .output_type = GPIO_OUTPUT_PUSH_PULL,
        .speed = GPIO_SPEED_HIGH,
        .pull = GPIO_PULL_NONE,
        .af = config->af
    };
    GPIO_PinConfig nss = {
        .pin = config->nss_pin,
        .mode = GPIO_MODE_OUTPUT,
        .output_type = GPIO_OUTPUT_PUSH_PULL,
        .speed = GPIO_SPEED_HIGH,
        .pull = GPIO_PULL_NONE,
        .af = config->af
    };

    GPIO_Init(config->port, &sck);
    GPIO_Init(config->port, &mosi);
    GPIO_Init(config->port, &miso);
    GPIO_Init(config->port, &nss);


    SPIx->CR1 = (config->cpol << 1) | 
             (config->cpha << 0) |
             (1 << 2) |                        // MSTR
             (config->baudrate_prescaler << 3) |
             (config->lsbfirst << 7) |
             (config->dff << 11);

    SPIx->CR1 |= (1 << 6); // SPE - enable SPI last
}

    



void SPI_transmit(SPI_TypeDef* SPIx, uint8_t* data, uint16_t size) {
    while (!(SPIx->SR & (1 << 1)));
    for (uint16_t i = 0; i < size; i++) {
        SPIx->DR = data[i]; // Write data to DR
        while (!(SPIx->SR & (1 << 1))); // Wait until TXE is set again
    }
    while((SPIx->SR & (1 << 7))); // Wait until BSY is cleared
    
}

void SPI_receive(SPI_TypeDef* SPIx, uint8_t* data, uint16_t size) {
    for (uint16_t i = 0; i < size; i++) {
        while (!(SPIx->SR & (1 << 1)));     // Wait for TXE
        SPIx->DR = 0x00;                     // Send dummy byte
        while (!(SPIx->SR & (1 << 0)));     // Wait for RXNE
        data[i] = SPIx->DR;                  // Read received byte
    }
    while (SPIx->SR & (1 << 7));            // Wait for BSY to clear
}

void CS_Select(GPIO_TypeDef* port, uint32_t pin) {
    GPIO_Write(port, pin, 0); // Pull low to select
}

void CS_Deselect(GPIO_TypeDef* port, uint32_t pin) {
    GPIO_Write(port, pin, 1); // Pull high to deselect
}
