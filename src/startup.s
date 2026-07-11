    .syntax unified
    .cpu cortex-m4
    .fpu softvfp
    .thumb

    .section .isr_vector, "a", %progbits
    .word _estack
    .word Reset_Handler
    .word NMI_Handler
    .word HardFault_Handler
    .word MemManage_Handler
    .word BusFault_Handler
    .word UsageFault_Handler
    .word 0
    .word 0
    .word 0
    .word 0
    .word SVC_Handler
    .word DebugMon_Handler
    .word 0
    .word PendSV_Handler
    .word SysTick_Handler

    .section .text.Reset_Handler
    .weak Reset_Handler
    .type Reset_Handler, %function
    Reset_Handler:
        ldr r0, =_estack
        mov sp, r0

        ldr r0, =_sdata
        ldr r1, =_edata
        ldr r2, =_sidata
        movs r3, #0
        b LoopCopyDataInit

    CopyDataInit:
        ldr r4, [r2, r3]
        str r4, [r0, r3]
        adds r3, r3, #4

    LoopCopyDataInit:
        adds r4, r0, r3
        cmp r4, r1
        bcc CopyDataInit


        ldr r0, =_sbss
        ldr r1, =_ebss
        movs r2, #0
        b LoopZeroBss

    ZeroBss:
        str r2, [r0]
        adds r0, r0, #4

    LoopZeroBss:
        cmp r0, r1
        bcc ZeroBss
        bl main

    LoopForever:
        b LoopForever


    .weak NMI_Handler
    .thumb_func
    NMI_Handler:
    .weak HardFault_Handler
    .thumb_func
    HardFault_Handler:
    .weak MemManage_Handler
    .thumb_func
    MemManage_Handler:
    .weak BusFault_Handler
    .thumb_func
    BusFault_Handler:
    .weak UsageFault_Handler
    .thumb_func
    UsageFault_Handler:
    .weak SVC_Handler
    .thumb_func
    SVC_Handler:
    .weak DebugMon_Handler
    .thumb_func
    DebugMon_Handler:
    .weak PendSV_Handler
    .thumb_func
    PendSV_Handler:
    .weak SysTick_Handler
    .thumb_func
    SysTick_Handler:
        b .