#include "../drivers/inc/rcc.h"
#include "../drivers/inc/gpio.h"
#include "../drivers/inc/systick.h"
#include "../drivers/inc/spi.h"
#include "../drivers/inc/usart.h"
#include <stdio.h>

int main(void) {
    RCC_Config(180000000);
    SysTick_Init(180000000);

    USART_Config usart2_config = {
        .USART_port = GPIOA,
        .CTS_pin = 0, // Not used
        .RTS_pin = 0, // Not used
        .TX_pin = 2,  // PA2
        .RX_pin = 3,  // PA3
        .CK_pin = 0,  // Not used
        .bauderate = 115200,
        .alternate_function = 7 // AF7 for USART2
    };

    USART_init(USART2, &usart2_config);


    GPIO_PinConfig BUSY_pin_config = {
        .pin = 3,
        .mode = GPIO_MODE_INPUT,
        .output_type = GPIO_OUTPUT_PUSH_PULL,
        .speed = GPIO_SPEED_HIGH,
        .pull = GPIO_PULL_NONE,
        .alternate_function = 0 // Not used for output mode
    };

    GPIO_Init(GPIOB, &BUSY_pin_config);

    GPIO_PinConfig D101_pin_config = {
        .pin = 4,
        .mode = GPIO_MODE_INPUT,
        .output_type = GPIO_OUTPUT_PUSH_PULL,
        .speed = GPIO_SPEED_HIGH,
        .pull = GPIO_PULL_NONE,
        .alternate_function = 0
    };

    GPIO_Init(GPIOB, &D101_pin_config);

    GPIO_PinConfig NRESET_pin_config = {
        .pin = 0,
        .mode = GPIO_MODE_OUTPUT,
        .output_type = GPIO_OUTPUT_PUSH_PULL,
        .speed = GPIO_SPEED_HIGH,
        .pull = GPIO_PULL_NONE,
        .alternate_function = 0 // Not used for output mode
    };

    GPIO_Init(GPIOA, &NRESET_pin_config);

    GPIO_Write(GPIOA, 0, 0); // assert NRESET (active low)
    SysTick_delay(1);        // hold low, datasheet requires >=100us
    GPIO_Write(GPIOA, 0, 1); // release NRESET
    SysTick_delay(10);       // allow the SX1262 to boot before polling BUSY

    GPIO_PinConfig ANT_SW_pin_config = {
        .pin = 9,
        .mode = GPIO_MODE_OUTPUT,
        .output_type = GPIO_OUTPUT_PUSH_PULL,
        .speed = GPIO_SPEED_HIGH,
        .pull = GPIO_PULL_NONE,
        .alternate_function = 0 // Not used for output mode
    };

    GPIO_Init(GPIOA, &ANT_SW_pin_config);

    SPI_Config spi1_config = {
        .port = GPIOA,
        .sck_pin = 5,
        .mosi_pin = 7,
        .miso_pin = 6,
        .nss_pin = 8,
        .alternate_function = 5, // AF5 for SPI1
        .baudrate_prescaler = 0b011, // fPCLK/16
        .cpol = 0,
        .cpha = 0,
        .dff = 0, // 8-bit data frame
        .lsbfirst = 0 // MSB first
    };


    SPI_Init(SPI1, &(spi1_config));

    //CS_Select(GPIOA, 8);
    //SPI_transmit(SPI1, &tx_data, 1);
    //SPI_receive(SPI1, &rx_data, 1);
    //CS_Deselect(GPIOA, 8);
    USART_transmit(USART2, (uint8_t*)"Starting main loop...\r\n", 23);

    while(1) {
        USART_transmit(USART2, (uint8_t*)"Waiting for BUSY low...\r\n", 25);
        while(GPIO_Read(GPIOB, 3) == 1); // wait BUSY low
        GPIO_Write(GPIOA, 8, 0);          // NSS low


        uint8_t opcode = 0xC0;
        uint8_t status;

        SPI_transmit(SPI1, &opcode, 1);   // send opcode, discard the garbage byte back
        SPI_receive(SPI1, &status, 0x00, 1); // send dummy 0x00, capture the real status
        GPIO_Write(GPIOA, 8, 1);          // NSS high

        USART_transmit(USART2, &status, 1);
        USART_transmit(USART2, (uint8_t*)"\r\n", 2);


        SysTick_delay(500);
    }
}




/*
SCK—D13 PA5 
(SPI1_SCK)MISO— D12 - PA6(SPI1_MISO)
MOSI— D11 - PA7(SPI1_MOSI)
NSS(CS)—D7 - PA8
BUSY— D3 - PB3 
(DIO1)(IRQ)—D5 - PB4
NRESET— A0 - PA0
ANT_SW— D8 - PA9
*/