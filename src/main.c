#include "../drivers/inc/rcc.h"
#include "../drivers/inc/gpio.h"
#include "../drivers/inc/systick.h"
#include "../drivers/inc/spi.h"

int main(void) {
    RCC_Config(180000000);
    SysTick_Init(180000000);


    SPI_Config spi1_config = {
        .port = GPIOA,
        .sck_pin = 5,
        .mosi_pin = 7,
        .miso_pin = 6,
        .nss_pin = 8,
        .af = 5, // AF5 for SPI1
        .baudrate_prescaler = 0b011, // fPCLK/16
        .cpol = 0,
        .cpha = 0,
        .dff = 0, // 8-bit data frame
        .lsbfirst = 0 // MSB first
    };


    SPI_Init(SPI1, &(spi1_config));

    uint8_t tx_data = 0xC0;
    uint8_t rx_data = 0x00;

    CS_Select(GPIOA, 8);
    SPI_transmit(SPI1, &tx_data, 1);
    SPI_receive(SPI1, &rx_data, 1);
    CS_Deselect(GPIOA, 8);
}