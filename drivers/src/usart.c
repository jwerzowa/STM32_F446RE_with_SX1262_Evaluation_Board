#include "../inc/usart.h"
#include "../inc/rcc.h"
#include "../inc/gpio.h"


void USART_init(USART_TypeDef* USARTx, USART_Config* config) {
    
    if(USARTx == USART1) {
        RCC->RCC_APB2ENR |= (1 << 4); // Enable clock for USART1
    } else if(USARTx == USART2) {
        RCC->RCC_APB1ENR |= (1 << 17); // Enable clock for USART2
    } else if(USARTx == USART3) {
        RCC->RCC_APB1ENR |= (1 << 18); // Enable clock for USART3
    } else if(USARTx == UART4) {
        RCC->RCC_APB1ENR |= (1 << 19); // Enable clock for UART4
    } else if(USARTx == UART5) {
        RCC->RCC_APB1ENR |= (1 << 20); // Enable clock for UART5
    } else if(USARTx == USART6) {
        RCC->RCC_APB2ENR |= (1 << 5); // Enable clock for USART6
    }

    RCC->RCC_AHB1ENR |= (1 << ((uint32_t)config->USART_port - (uint32_t)GPIOA) / 0x400); // Enable clock for GPIO port

    GPIO_PinConfig tx_config = {
        .pin = config->TX_pin,
        .mode = GPIO_MODE_ALTERNATE,
        .output_type = GPIO_OUTPUT_PUSH_PULL,
        .speed = GPIO_SPEED_HIGH,
        .pull = GPIO_PULL_NONE,
        .alternate_function = config->alternate_function
    };

    GPIO_PinConfig rx_config = {
        .pin = config->RX_pin,
        .mode = GPIO_MODE_ALTERNATE,
        .output_type = GPIO_OUTPUT_PUSH_PULL,
        .speed = GPIO_SPEED_HIGH,
        .pull = GPIO_PULL_UP,
        .alternate_function = config->alternate_function
    };

    GPIO_Init(config->USART_port, &tx_config);
    GPIO_Init(config->USART_port, &rx_config);
    
    uint32_t clock;

    if(USARTx == USART1) {
        clock = PCLK2_HZ;
    } else if(USARTx == USART2) {
        clock = PCLK1_HZ;
    } else if(USARTx == USART3) {
        clock = PCLK1_HZ;
    } else if(USARTx == UART4) {
        clock = PCLK1_HZ;
    } else if(USARTx == UART5) {
        clock = PCLK1_HZ;
    } else if(USARTx == USART6) {
        clock = PCLK2_HZ;
    }

    uint32_t BRR = clock / config->bauderate;

    USARTx->USART_BRR = BRR;

    USARTx->USART_CR1 |= (1 << 13) | (1 << 3) | (1 << 2); // Enable UE, TE, RE

    USARTx->USART_CR1 &= ~((1<<12)|(1<<10)); // Disable M and PCE for 8N1 configuration

 
}

void USART_transmit(USART_TypeDef* USARTx, uint8_t* data, uint16_t size) {

    for(uint16_t i = 0; i < size; i++) {
        while(!(USARTx->USART_SR & (1 << 7))); // Wait until TXE is set
        USARTx->USART_DR = data[i]; // Send data
    }

}

void USART_receive(USART_TypeDef* USARTx, uint8_t* data, uint16_t size) {

    for(uint16_t i = 0; i < size; i++) {
        while(!(USARTx->USART_SR & (1<<5)));
        data[i] = USARTx->USART_DR; // Read data
    }
}