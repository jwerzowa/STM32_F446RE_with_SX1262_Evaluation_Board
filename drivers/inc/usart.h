#ifndef USART_H
#define USART_H

#include <stdint.h>
#include "gpio.h"

typedef struct {
    GPIO_TypeDef* USART_port;
    uint32_t CTS_pin;
    uint32_t RTS_pin;
    uint32_t TX_pin;
    uint32_t RX_pin;
    uint32_t CK_pin;
    uint32_t bauderate;
    uint8_t alternate_function;
} USART_Config;

typedef struct {
    volatile uint32_t USART_SR;
    volatile uint32_t USART_DR;
    volatile uint32_t USART_BRR;
    volatile uint32_t USART_CR1;
    volatile uint32_t USART_CR2;
    volatile uint32_t USART_CR3;
    volatile uint32_t USART_GTPR;
} USART_TypeDef;


void USART_init(USART_TypeDef* USARTx, USART_Config* config);
void USART_transmit(USART_TypeDef* USARTx, uint8_t* data, uint16_t size);
void USART_receive(USART_TypeDef* USARTx, uint8_t* data, uint16_t size);

#define USART1 ((USART_TypeDef*) 0x40011000)
#define USART2 ((USART_TypeDef*) 0x40004400)
#define USART3 ((USART_TypeDef*) 0x40004800)
#define UART4  ((USART_TypeDef*) 0x40004C00)
#define UART5  ((USART_TypeDef*) 0x40005000)
#define USART6 ((USART_TypeDef*) 0x40011400)

#endif // USART_H