/*
 * Copyright (c) 2022 HiSilicon (Shanghai) Technologies CO., LIMITED.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <unistd.h>
#include "ohos_init.h"
#include "cmsis_os2.h"
#include "iot_gpio.h"
#include "iot_watchdog.h"

#if CHIP_WS63
#include "iot_gpio_ex.h"
#else
#include "hi_io.h"
#endif

#include "iot_ssl_gpio.h"

enum LedState {
    LED_ON = 0,
    LED_OFF,
    LED_SPARK,
};

enum LedState g_ledState = LED_SPARK;

static void OnButtonPressed(char *arg)
{
    (void) arg;

    enum LedState nextState = LED_SPARK;

    switch (g_ledState) {
        case LED_ON:
            nextState = LED_OFF;
            break;
        case LED_OFF:
            nextState = LED_SPARK;
            break;
        case LED_SPARK:
            nextState = LED_ON;
            break;
        default:
            break;
    }

    g_ledState = nextState;
}

static void *ButtonGPIODemo(const char *arg)
{
    (void)arg;
    (void)osal_msleep(1000);
    printf("AppTask start\r\n");

    // 初始化GPIO
    IoTGpioInit(IOT_GPIO_IDX_GREEN_LED);
    // 设置引脚功能为MODE_0(GPIO)
    IoSetFunc(IOT_GPIO_IDX_SWITCH, IOT_IO_FUNC_GPIO_14_GPIO);
    IoSetFunc(IOT_GPIO_IDX_GREEN_LED, IOT_IO_FUNC_GPIO_11_GPIO);
    // 设置GPIO为输出方向
    IoTGpioSetDir(IOT_GPIO_IDX_SWITCH, IOT_GPIO_DIR_IN);
    IoTGpioSetDir(IOT_GPIO_IDX_GREEN_LED, IOT_GPIO_DIR_OUT);
    // 配置GPIO引脚号和输出值
    IoTGpioSetOutputVal(IOT_GPIO_IDX_GREEN_LED, IOT_GPIO_VALUE0);

    // 注册按钮事件
    IoTGpioRegisterIsrFunc(IOT_GPIO_IDX_SWITCH, IOT_INT_TYPE_EDGE, IOT_GPIO_EDGE_FALL_LEVEL_LOW,
        OnButtonPressed, NULL);

    while (1) {
        switch (g_ledState) {
            case LED_ON:
                IoTGpioSetOutputVal(IOT_GPIO_IDX_GREEN_LED, IOT_GPIO_VALUE0);
                 osDelay(100);
              
                break;
            case LED_OFF:
                IoTGpioSetOutputVal(IOT_GPIO_IDX_GREEN_LED, IOT_GPIO_VALUE1);
               
                 osDelay(100);
                break;
            case LED_SPARK:
                IoTGpioSetOutputVal(IOT_GPIO_IDX_GREEN_LED, IOT_GPIO_VALUE0);
             
                 osDelay(20);
                IoTGpioSetOutputVal(IOT_GPIO_IDX_GREEN_LED, IOT_GPIO_VALUE1);
             
                 osDelay(20);
                break;
            default:

                 osDelay(100);
                break;
        }

    }
}

static void AppTask(void)
{
    osThreadAttr_t attr;

    attr.name = "ButtonGPIODemo";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = 2048; 
    attr.priority = osPriorityNormal;

    // 报错
    if (osThreadNew((osThreadFunc_t)ButtonGPIODemo, NULL, &attr) == NULL) {
        printf("[Example] Failed to create AppTask!\n");
    }
}

SYS_RUN(AppTask);
