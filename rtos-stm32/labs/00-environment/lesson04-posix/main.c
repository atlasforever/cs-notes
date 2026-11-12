
#include <inttypes.h>
#include <stdatomic.h>
#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"

static atomic_uint_fast64_t g_worker_a_count;
static atomic_uint_fast64_t g_worker_b_count;

static void worker_a(void *arg)
{
    (void)arg;

    for (;;) {
        atomic_fetch_add_explicit(
            &g_worker_a_count, 1, memory_order_relaxed);
    }
}

static void worker_b(void *arg)
{
    (void)arg;

    for (;;) {
        atomic_fetch_add_explicit(
            &g_worker_b_count, 1, memory_order_relaxed);
    }
}

static void monitor_task(void *arg)
{
    (void)arg;

    printf("[monitor] started, tick=%lu\n",
           (unsigned long)xTaskGetTickCount());

    for (;;) {
        vTaskDelay(pdMS_TO_TICKS(1000));

        const uint_fast64_t count_a =
            atomic_load_explicit(&g_worker_a_count,
                                 memory_order_relaxed);

        const uint_fast64_t count_b =
            atomic_load_explicit(&g_worker_b_count,
                                 memory_order_relaxed);

        printf("[monitor] tick=%lu, A=%" PRIuFAST64
               ", B=%" PRIuFAST64 "\n",
               (unsigned long)xTaskGetTickCount(),
               count_a,
               count_b);
    }
}

int main(void)
{
    setbuf(stdout, NULL);

    if (xTaskCreate(monitor_task, "monitor",
                    configMINIMAL_STACK_SIZE,
                    NULL, 3, NULL) != pdPASS) {
        return 1;
    }

    if (xTaskCreate(worker_a, "worker_a",
                    configMINIMAL_STACK_SIZE,
                    NULL, 2, NULL) != pdPASS) {
        return 1;
    }

    if (xTaskCreate(worker_b, "worker_b",
                    configMINIMAL_STACK_SIZE,
                    NULL, 2, NULL) != pdPASS) {
        return 1;
    }

    vTaskStartScheduler();

    fprintf(stderr, "Scheduler returned unexpectedly\n");
    return 1;
}
