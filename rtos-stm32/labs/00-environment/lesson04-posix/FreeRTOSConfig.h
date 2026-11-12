#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include <assert.h>

#define configUSE_PREEMPTION             1
#define configUSE_TIME_SLICING           1
#define configCPU_CLOCK_HZ               1000000UL
#define configTICK_RATE_HZ               100
#define configTICK_TYPE_WIDTH_IN_BITS TICK_TYPE_WIDTH_32_BITS
#define configMAX_PRIORITIES             5

#define configMINIMAL_STACK_SIZE         256
#define configMAX_TASK_NAME_LEN          16
#define configTOTAL_HEAP_SIZE            (64 * 1024)

#define configSUPPORT_DYNAMIC_ALLOCATION 1
#define configSUPPORT_STATIC_ALLOCATION  0

#define configUSE_IDLE_HOOK              0
#define configUSE_TICK_HOOK              0
#define configUSE_TIMERS                 0
#define configCHECK_FOR_STACK_OVERFLOW   0
#define configUSE_MALLOC_FAILED_HOOK     0

#define INCLUDE_vTaskDelay              1

#define configASSERT(x) assert(x)

#endif