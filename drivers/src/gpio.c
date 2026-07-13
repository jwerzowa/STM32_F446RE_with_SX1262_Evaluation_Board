
#include "../inc/gpio.h"
#include "../inc/rcc.h"
#include <stdint.h>

void GPIO_Init(GPIO_TypeDef* port, GPIO_PinConfig* config) {
    
    //Enable the clock for the GPIO port, set the appropriate bit based on port address, given the offsets are consistently 0x400 apart, we can calculate the bit position by subtracting the base address of GPIOA from the port address and dividing by 0x400.
    RCC->RCC_AHB1ENR |= (1 << ((uint32_t)port - (uint32_t)GPIOA) / 0x400);
    
    //force the CPU to wait for the bus to complete the previous write before continuing
    volatile uint32_t dummy = RCC->RCC_AHB1ENR;
    (void)dummy;


    //Set the mode, output type, speed, and pull-up or pull-down configuration for the specified pin, and alternate function
    port->MODER &= ~(3 << config->pin *2);
    port->MODER |= (config->mode << config->pin *2);

    port->OTYPER &= ~(1 << config->pin);
    port->OTYPER |= (config->output_type << config->pin);

    port->OSPEEDR &= ~(3 << config->pin *2);
    port->OSPEEDR |= (config->speed << config->pin *2);

    port->PUPDR &= ~(3 << config->pin *2);
    port->PUPDR |= (config->pull << config->pin *2);
    
    //Set the alternate function, depending on the pin, since the alternate function registers are split into two 32-bit registers, AFRL for pins 0-7 and AFRH for pins 8-15. First, see if the mode is set to alternate function, then check if the pin is less than 8, if so, set the appropriate bits in AFRL, otherwise set the appropriate bits in AFRH.
    if (config->mode == GPIO_MODE_ALTERNATE) {
        if (config->pin < 8) {
            port->AFRL &= ~(0xF << (config->pin * 4));
            port->AFRL |=  (config->alternate_function << (config->pin * 4));
        } else {
            port->AFRH &= ~(0xF << ((config->pin - 8) * 4));
            port->AFRH |=  (config->alternate_function << ((config->pin - 8) * 4));
        }
    }

}

void GPIO_Write(GPIO_TypeDef* port, uint32_t pin, uint8_t value) {

    if(value){
        port->BSRR = (1 << pin); //Set the pin
    } else {
        port->BSRR = (1 << (pin + 16)); //Reset the pin
    }
}

void GPIO_Toggle(GPIO_TypeDef* port, uint32_t pin) {
    port->ODR ^= (1 << pin); //Toggle the pin
}

uint8_t GPIO_Read(GPIO_TypeDef* port, uint32_t pin) {
    return port->IDR & (1 << pin);
}