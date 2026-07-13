
#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>


#define GPIOA ((GPIO_TypeDef*) 0x40020000)
#define GPIOB ((GPIO_TypeDef*) 0x40020400)
#define GPIOC ((GPIO_TypeDef*) 0x40020800)
#define GPIOD ((GPIO_TypeDef*) 0x40020C00)
#define GPIOE ((GPIO_TypeDef*) 0x40021000)
#define GPIOF ((GPIO_TypeDef*) 0x40021400)
#define GPIOG ((GPIO_TypeDef*) 0x40021800)
#define GPIOH ((GPIO_TypeDef*) 0x40021C00)


typedef enum {
    GPIO_MODE_INPUT = 0x00,
    GPIO_MODE_OUTPUT = 0x01,
    GPIO_MODE_ALTERNATE = 0x02,
    GPIO_MODE_ANALOG = 0x03
} GPIO_Mode;

typedef enum {
    GPIO_OUTPUT_PUSH_PULL = 0x00,
    GPIO_OUTPUT_OPEN_DRAIN = 0x01
}  GPIO_OutputType;

typedef enum {
    GPIO_SPEED_LOW = 0x00,
    GPIO_SPEED_MEDIUM = 0x01,
    GPIO_SPEED_HIGH = 0x02,
    GPIO_SPEED_VERY_HIGH = 0x03
} GPIO_Speed;

typedef enum {
    GPIO_PULL_NONE = 0x00,
    GPIO_PULL_UP   = 0x01,
    GPIO_PULL_DOWN = 0x02
} GPIO_Pull;

typedef struct {
    uint32_t pin;
    GPIO_Mode mode;
    GPIO_OutputType output_type;
    GPIO_Speed speed;
    GPIO_Pull pull;
    uint32_t        alternate_function;
} GPIO_PinConfig;

typedef struct {
    volatile uint32_t MODER;
    volatile uint32_t OTYPER;
    volatile uint32_t OSPEEDR;
    volatile uint32_t PUPDR;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t LCKR;
    volatile uint32_t AFRL;
    volatile uint32_t AFRH;

} GPIO_TypeDef;



//Basic Functions for GPIO
void GPIO_Init(GPIO_TypeDef *port, GPIO_PinConfig *config);
void GPIO_Write(GPIO_TypeDef *port, uint32_t pin, uint8_t value);
void GPIO_Toggle(GPIO_TypeDef *port, uint32_t pin);
uint8_t GPIO_Read(GPIO_TypeDef *port, uint32_t pin);

#endif // GPIO_H