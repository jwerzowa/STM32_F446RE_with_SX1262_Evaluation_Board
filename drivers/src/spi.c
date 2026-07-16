#include "../inc/spi.h"
#include "../inc/rcc.h"
#include "../inc/gpio.h"

void SPI_Init(SPI_TypeDef* SPIx, SPI_Config* config) {
    
    RCC->RCC_APB2ENR |= (1 << 12); // Enable SPI1 clock

    //Configure GPIO pins for SPI functionality using the provided configuration structure. Each pin (SCK, MOSI, MISO, NSS) is set up with the appropriate mode, output type, speed, pull-up/pull-down configuration, and alternate function given config struct
    GPIO_PinConfig sck = {
        .pin = config->sck_pin,
        .mode = GPIO_MODE_ALTERNATE,
        .output_type = GPIO_OUTPUT_PUSH_PULL,
        .speed = GPIO_SPEED_HIGH,
        .pull = GPIO_PULL_NONE,
        .alternate_function = config->alternate_function
    };
    GPIO_PinConfig mosi = {
        .pin = config->mosi_pin,
        .mode = GPIO_MODE_ALTERNATE,
        .output_type = GPIO_OUTPUT_PUSH_PULL,
        .speed = GPIO_SPEED_HIGH,
        .pull = GPIO_PULL_NONE,
        .alternate_function = config->alternate_function
    };
    GPIO_PinConfig miso = {
        .pin = config->miso_pin,
        .mode = GPIO_MODE_ALTERNATE,
        .output_type = GPIO_OUTPUT_PUSH_PULL,
        .speed = GPIO_SPEED_HIGH,
        .pull = GPIO_PULL_NONE,
        .alternate_function = config->alternate_function
    };
    GPIO_PinConfig nss = {
        .pin = config->nss_pin,
        .mode = GPIO_MODE_OUTPUT,
        .output_type = GPIO_OUTPUT_PUSH_PULL,
        .speed = GPIO_SPEED_HIGH,
        .pull = GPIO_PULL_NONE,
        .alternate_function = config->alternate_function
    };

    GPIO_Init(config->port, &sck);
    GPIO_Init(config->port, &mosi);
    GPIO_Init(config->port, &miso);
    GPIO_Init(config->port, &nss);

    //Configure the SPI peripheral. 
    SPIx->CR1 = (config->cpol << 1) |
             (config->cpha << 0) |
             (1 << 2) |                        // MSTR
             (config->baudrate_prescaler << 3) |
             (config->lsbfirst << 7) |
             (config->dff << 11) |
             (1 << 8) |                         // SSI - force internal NSS high
             (1 << 9);                          // SSM - software slave management (NSS is a plain GPIO here)

    SPIx->CR1 |= (1 << 6); // enable SPI last
}

    



void SPI_transmit(SPI_TypeDef* SPIx, uint8_t* data, uint16_t size) {            
    for (uint16_t i = 0; i < size; i++) {
        SPI_TransferByte(SPIx, data[i]); // Send byte and receive dummy byte
    }
    while((SPIx->SR & (1 << 7))); // Wait until BSY is cleared
    
}

void SPI_receive(SPI_TypeDef* SPIx, uint8_t* data, uint8_t byte_out, uint16_t size) {
    for (uint16_t i = 0; i < size; i++) {
        data[i] = SPI_TransferByte(SPIx, byte_out);                     // Send dummy byte
    }
    while (SPIx->SR & (1 << 7));            // Wait for BSY to clear
}


uint8_t SPI_TransferByte(SPI_TypeDef* SPIx, uint8_t byte_out){
    
    while (!(SPIx->SR & (1 << 1))); // Wait until TXE is set
    SPIx->DR = byte_out;            // Send byte
    while (!(SPIx->SR & (1 << 0))); // Wait until RXNE is set
    return SPIx->DR;                // Return received byte
};


void CS_Select(GPIO_TypeDef* port, uint32_t pin) {
    GPIO_Write(port, pin, 0); // Pull low to select 
}

void CS_Deselect(GPIO_TypeDef* port, uint32_t pin) {
    GPIO_Write(port, pin, 1); // Pull high to deselect
}
