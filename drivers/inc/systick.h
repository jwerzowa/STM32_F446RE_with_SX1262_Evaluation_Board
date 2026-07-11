#ifndef SYSTICK_H
#define SYSTICK_H

#include <stdint.h>

typedef struct {
    volatile uint32_t SYST_CSR ; // SysTick Control and Status Register
    volatile uint32_t SYST_RVR ; // SysTick Reload Value Register
    volatile uint32_t SYST_CVR ; // SysTick Current Value Register
    volatile uint32_t SYST_CALIB ; // SysTick Calibration Value Register

} Systick_TypeDef;

#define SYSTICK_BASE ((Systick_TypeDef*) 0xE000E010) 


void SysTick_Init(uint32_t cpu_freq);
uint32_t SysTick_GetTick(void);
void SysTick_delay(uint32_t ms);


#endif // SYSTICK_H