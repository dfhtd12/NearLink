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

#include "ohos_init.h"
#include "cmsis_os2.h"
#include "iot_gpio.h"
#include "iot_gpio_ex.h"

#define LED_TASK_GPIO 10
#define IOT_GPIO_KEY 13
#define LED_INTERVAL_TIME 30
#define LED_TASK_STACK_SIZE 0x1000
#define LED_TASK_PRIO 25

enum LedState{
    LED_ON = 0,
    LED_OFF,
    LED_SPARK,
    LED_EXIT,
};

enum LedState g_ledState = LED_SPARK;

static void *LedTask(const char *arg)
{
    (void) arg;
    uint32_t running = 1;
    while (running) {
        switch (g_ledState) {
            case LED_ON:
                IoTGpioSetOutputVal(LED_TASK_GPIO, 0);
                osDelay(LED_INTERVAL_TIME);
                break;
            case LED_OFF:
                IoTGpioSetOutputVal(LED_TASK_GPIO, 1);
                osDelay(LED_INTERVAL_TIME);
                break;
            case LED_SPARK:
                IoTGpioSetOutputVal(LED_TASK_GPIO, 0);
                osDelay(LED_INTERVAL_TIME);
                IoTGpioSetOutputVal(LED_TASK_GPIO, 1);
                osDelay(LED_INTERVAL_TIME);
                break;
            case LED_EXIT:
                running = 0;
                break;
            default:
                osDelay(LED_INTERVAL_TIME);
                return ;
        }
    }

    return NULL;
}

static void OnButtonPressed(char *arg)
{
    (void) arg;

    enum LedState nextState = LED_SPARK;
    switch (g_ledState) {
        case LED_ON:
            nextState = LED_SPARK;
            break;
        case LED_OFF:
            nextState = LED_ON;
            break;
        case LED_SPARK:
            nextState = LED_OFF;
            break;
        default:
            break;
    }

    g_ledState = nextState;
}

static void LedExampleEntry(void)
{
    osThreadAttr_t attr;

    IoTGpioInit(LED_TASK_GPIO);
    IoSetFunc(LED_TASK_GPIO, 0);
    IoTGpioSetDir(LED_TASK_GPIO, IOT_GPIO_DIR_OUT);


    IoTGpioInit(IOT_GPIO_KEY);
    IoSetFunc(IOT_GPIO_KEY, 0);
    IoTGpioSetDir(IOT_GPIO_KEY, IOT_GPIO_DIR_IN);
    IoSetPull(IOT_GPIO_KEY, IOT_IO_PULL_UP);

    IoTGpioRegisterIsrFunc(IOT_GPIO_KEY, IOT_INT_TYPE_EDGE, IOT_GPIO_EDGE_FALL_LEVEL_LOW,
                           OnButtonPressed, NULL);

    attr.name = "LedTask";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = LED_TASK_STACK_SIZE;
    attr.priority = LED_TASK_PRIO;

    if (osThreadNew((osThreadFunc_t) LedTask, NULL, &attr) == NULL) {
        printf("[LedExample] Falied to create LedTask!\n");
    }
}

SYS_RUN(LedExampleEntry);