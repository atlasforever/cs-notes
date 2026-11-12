.syntax unified
.cpu cortex-m3
.thumb

.section .isr_vector, "a", %progbits
.word __stack_top
.word Reset_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word 0
.word 0
.word 0
.word 0
.word vPortSVCHandler
.word Default_Handler
.word 0
.word xPortPendSVHandler
.word xPortSysTickHandler

.section .text.Reset_Handler, "ax", %progbits
.global Reset_Handler
.thumb_func

Reset_Handler:
    /* Copy .data into RAM */
    ldr r0, =__data_start__
    ldr r1, =__data_end__
    ldr r2, =__data_load__

1:
    cmp r0, r1
    bhs 2f
    ldr r3, [r2], #4
    str r3, [r0], #4
    b 1b

2:
    /* Zero .bss */
    ldr r0, =__bss_start__
    ldr r1, =__bss_end__
    movs r2, #0

3:
    cmp r0, r1
    bhs 4f
    str r2, [r0], #4
    b 3b

4:
    bl main

5:
    b 5b

.section .text.Default_Handler, "ax", %progbits
.thumb_func
Default_Handler:
    b .