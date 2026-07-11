#include "../inc/systick.h"
#include <stdint.h>

volatile uint32_t ticks;


void SysTick_Init(uint32_t cpu_freq) {
    SYSTICK_BASE->SYST_RVR = (cpu_freq / 1000) - 1; // Set reload value for 1ms tick
    SYSTICK_BASE->SYST_CVR = 0; // Clear current value
    SYSTICK_BASE->SYST_CSR = 0x07; // Enable SysTick, use processor clock, enable interrupt
}

uint32_t SysTick_GetTick(void){
    return ticks;
}

void SysTick_delay(uint32_t ms){
    volatile uint32_t start_tick = SysTick_GetTick();
    while ((SysTick_GetTick() - start_tick) < ms) {
        // Wait until the specified time has passed
    }    
}

void SysTick_Handler(void) {
    ticks++;
}
