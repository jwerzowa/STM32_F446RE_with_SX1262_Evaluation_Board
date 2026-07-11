
#ifndef RCC_H
#define RCC_H

#include <stdint.h>

//RCC base Address
#define RCC ((RCC_TypeDef*) 0x40023800)

//Structure to make register access easier, give offsets are consistently 32 bits (4 bytes) apart.
typedef struct {
    volatile uint32_t RCC_CR;
    volatile uint32_t RCC_PLLCFGR;
    volatile uint32_t RCC_CFGR;
    volatile uint32_t RCC_CIR;
    volatile uint32_t RCC_AHB1RSTR;
    volatile uint32_t RCC_AHB2RSTR;
    volatile uint32_t RCC_AHB3RSTR;
    volatile uint32_t reserved0;
    volatile uint32_t RCC_APB1RSTR;
    volatile uint32_t RCC_APB2RSTR;
    volatile uint32_t reserved1;
    volatile uint32_t reserved2;
    volatile uint32_t RCC_AHB1ENR;
    volatile uint32_t RCC_AHB2ENR;
    volatile uint32_t RCC_AHB3ENR;
    volatile uint32_t reserved3;
    volatile uint32_t RCC_APB1ENR;
    volatile uint32_t RCC_APB2ENR;
    volatile uint32_t reserved4;
    volatile uint32_t reserved5;
    volatile uint32_t RCC_AHB1LPENR;
    volatile uint32_t RCC_AHB2LPENR;
    volatile uint32_t RCC_AHB3LPENR;
    volatile uint32_t reserved6;
    volatile uint32_t RCC_APB1LPENR;
    volatile uint32_t RCC_APB2LPENR;
    volatile uint32_t reserved7;
    volatile uint32_t reserved8;
    volatile uint32_t RCC_BDCR;
    volatile uint32_t RCC_CSR;
    volatile uint32_t reserved9;
    volatile uint32_t reserved10;
    volatile uint32_t RCC_SSCGR;
    volatile uint32_t RCC_PLLI2SCFGR;
    volatile uint32_t RCC_PLLSAICFGR;
    volatile uint32_t RCC_DCKCFGR;
    volatile uint32_t RCC_CKGATENR;
    volatile uint32_t RCC_DCKCFGR2;
} RCC_TypeDef;

typedef enum {
    RCC_OK = 0,
    RCC_Error = 1
} RCC_Status;



//function declarations
RCC_Status RCC_Config(uint32_t frequency);
uint32_t RCC_GetSystemClockFrequency(void);


#endif // RCC_H