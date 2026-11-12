#include <stdint.h>

#include "FreeRTOS.h"
#include "task.h"

#define UART0_BASE    0x40004000UL

#define UART_DATA     (*(volatile uint32_t *)(UART0_BASE + 0x00))
#define UART_STATE    (*(volatile uint32_t *)(UART0_BASE + 0x04))
#define UART_CTRL     (*(volatile uint32_t *)(UART0_BASE + 0x08))
#define UART_BAUDDIV  (*(volatile uint32_t *)(UART0_BASE + 0x10))

static void uart_init(void)
{
    UART_BAUDDIV = 217;
    UART_CTRL = 1U;
}

static void uart_putc(char c)
{
    while (UART_STATE & 1U) {
    }

    UART_DATA = (uint32_t)c;
}

static void uart_puts(const char *s)
{
    while (*s != '\0') {
        uart_putc(*s++);
    }
}

static void demo_task(void *arg)
{
    (void)arg;

    for (;;) {
        uart_puts("FreeRTOS task running\r\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

int main(void)
{
    uart_init();
    uart_puts("Cortex-M3 boot\r\n");

    if (xTaskCreate(demo_task, "demo", 256,
                    NULL, 2, NULL) != pdPASS) {
        uart_puts("Task creation failed\r\n");
        for (;;) {}
    }

    uart_puts("Starting scheduler\r\n");
    vTaskStartScheduler();

    for (;;) {}
}