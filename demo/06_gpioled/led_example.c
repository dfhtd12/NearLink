/*
Copyright (C) 2024 HiHope Open Source Organization .
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include <stdio.h>

#include <unistd.h>

#include "cmsis_os2.h"
#include "ohos_init.h"
#include "iot_gpio.h"

#define LED_TASK_GPIO 10
#define LED_TASK_STACK_SIZE 0x1000
#define LED_TASK_PRIO 25

static void *GpioTask(const char *arg)
{
    (void) arg;

    IoTGpioInit(LED_TASK_GPIO);
    IoTGpioSetDir(LED_TASK_GPIO, IOT_GPIO_DIR_OUT);
    uint32_t counter  = 0;
    uint32_t running = 1;
    while (running) {
        printf(" LED_SPARK! \n");
        IoTGpioSetOutputVal(LED_TASK_GPIO, 0);
        osDelay(50);
        IoTGpioSetOutputVal(LED_TASK_GPIO, 1);
        osDelay(50);

        counter++;
        if (counter >= 100) {
            running = 0;
        }
    }
    return NULL;
}

static void GpioExampleEntry(void)
{
    osThreadAttr_t attr;

    attr.name = "GpioTask";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = LED_TASK_STACK_SIZE;
    attr.priority = LED_TASK_PRIO;

    if (osThreadNew((osThreadFunc_t) GpioTask, NULL, &attr) == NULL) {
        printf("[GpioExample] Falied to create GpioTask!\n");
    }
}

SYS_RUN(GpioExampleEntry); // if test add it
