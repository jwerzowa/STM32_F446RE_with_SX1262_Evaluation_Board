#include "../inc/rcc.h"
#include <stdint.h>

#define FLASH_ACR ((volatile uint32_t*)0x40023C00)

//Function to set the frequency of the clock
//Return enum RCC_OK = 0 if successful, RCC_Error = 1 if not 
RCC_Status RCC_Config(uint32_t frequency) {
    // Enable HSI
    RCC->RCC_CR |= (1<<0); 
    // Wait for the HSI to be ready (Per reference manual: internal high-speed clock ready flag Set by hardware to indicate that the HSI oscillator is stable. After the HSION bit is cleared, HSIRDY goes low after 6 HSI clock cycles)
    while (!(RCC->RCC_CR & (1<<1)));

    //Configure the PLL (Phase locked loop) to achieve the desired frequency
    // Clear and set M, N, P, PLLSRC
    RCC->RCC_PLLCFGR = 0; 
    //Division factor for the main PLL (PLL) input clock
    RCC->RCC_PLLCFGR |= (8 << 0);    // M = 8
    //Main PLL (PLL) multiplication factor for VCO
    RCC->RCC_PLLCFGR |= (180 << 6);  // N = 180
    //Main PLL (PLL) division factor for main system clock (0b00 : divide by 2, 0b01 : divide by 4, 0b10 : divide by 6, 0b11 : divide by 8)
    RCC->RCC_PLLCFGR |= (0b00 << 16);   // P = 0 means divide by 2


    // Enable instruction cache, data cache, prefetch, and set 5 wait states
    *FLASH_ACR = (1 << 10) | (1 << 9) | (1 << 8) | 5;


    //Now enable PLL (Phase Locked Loop) and wait for it to be ready
    RCC->RCC_CR |= (1<<24); // Set PLLON bit
    //wait for PLL to be ready
    while (!(RCC->RCC_CR & (1<<25)));

    //set the prescalers for the different buses (AHB, APB1, APB2)
    RCC->RCC_CFGR &= ~(0xF << 4);   // clear HPRE
    RCC->RCC_CFGR |=  (0 << 4);     // AHB /1

    RCC->RCC_CFGR &= ~(0x7 << 10);  // clear PPRE1
    RCC->RCC_CFGR |=  (5 << 10);    // APB1 /4

    RCC->RCC_CFGR &= ~(0x7 << 13);  // clear PPRE2
    RCC->RCC_CFGR |=  (4 << 13);    // APB2 /2

    // Now switch the system clock to PLL
    RCC->RCC_CFGR &= ~(3 << 0);
    RCC->RCC_CFGR |= (2 << 0);

    // Wait for the system clock to switch to PLL, take RCC_CFGR value and & it with (3 << 2) to get the SWS bits, check if they are equal to (2 << 2) which means PLL is selected as system clock
    while ((RCC->RCC_CFGR & (3 << 2)) != (2 << 2));

    return RCC_OK;
}

uint32_t RCC_GetSystemClockFrequency(void) {

    switch (RCC->RCC_CFGR & (3<<2) >> 2) {
        case 0: return 16000000;   // HSI
        case 1: return 8000000;    // HSE (external crystal, assume 8MHz)
        case 2: return 180000000;  // PLL
        default: return 0;
    }

}