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

#if CHIP_WS63
#define IOT_GPIO_IDX_RED_LED 7
#define IOT_GPIO_IDX_YELLOW_LED 10
#define IOT_GPIO_IDX_GREEN_LED 11
#else
// for hispark_pegasus
#define IOT_GPIO_IDX_RED_LED 10
#define IOT_GPIO_IDX_YELLOW_LED 12
#define IOT_GPIO_IDX_GREEN_LED 11
#endif

#if CHIP_WS63
static void *LedCntrolDemo(const char *arg)
{
    (void)arg;

    TaskMsleep(1000);//等待uart2（gpio7,gpio8）初始化完成后，再设置GPIO7为gpio功能
    printf("LedTask start\r\n");

    // 初始化GPIO
    IoTGpioInit(IOT_GPIO_IDX_RED_LED);
    IoTGpioInit(IOT_GPIO_IDX_YELLOW_LED);
    IoTGpioInit(IOT_GPIO_IDX_GREEN_LED);
    // 设置引脚功能为MODE_0(GPIO)
    
    IoSetFunc(IOT_GPIO_IDX_RED_LED, IOT_IO_FUNC_GPIO_7_GPIO);
    IoSetFunc(IOT_GPIO_IDX_YELLOW_LED, IOT_IO_FUNC_GPIO_10_GPIO);
    IoSetFunc(IOT_GPIO_IDX_GREEN_LED, IOT_IO_FUNC_GPIO_11_GPIO);
    // 设置GPIO为输出方向
    IoTGpioSetDir(IOT_GPIO_IDX_RED_LED, IOT_GPIO_DIR_OUT);
    IoTGpioSetDir(IOT_GPIO_IDX_YELLOW_LED, IOT_GPIO_DIR_OUT);
    IoTGpioSetDir(IOT_GPIO_IDX_GREEN_LED, IOT_GPIO_DIR_OUT);
    // 配置GPIO引脚号和输出值
    IoTGpioSetOutputVal(IOT_GPIO_IDX_RED_LED, IOT_GPIO_VALUE0);
    IoTGpioSetOutputVal(IOT_GPIO_IDX_YELLOW_LED, IOT_GPIO_VALUE0);
    IoTGpioSetOutputVal(IOT_GPIO_IDX_GREEN_LED, IOT_GPIO_VALUE0);

    while(1) {
        //亮红灯
        IoTGpioSetOutputVal(IOT_GPIO_IDX_GREEN_LED, IOT_GPIO_VALUE0);
        IoTGpioSetOutputVal(IOT_GPIO_IDX_RED_LED, IOT_GPIO_VALUE1);
        TaskMsleep(1000);
        //亮黄灯
        IoTGpioSetOutputVal(IOT_GPIO_IDX_RED_LED, IOT_GPIO_VALUE0);
        IoTGpioSetOutputVal(IOT_GPIO_IDX_YELLOW_LED, IOT_GPIO_VALUE1);
        TaskMsleep(1000);
        //亮绿灯
        IoTGpioSetOutputVal(IOT_GPIO_IDX_YELLOW_LED, IOT_GPIO_VALUE0);
        IoTGpioSetOutputVal(IOT_GPIO_IDX_GREEN_LED, IOT_GPIO_VALUE1);
        TaskMsleep(1000);
    }


    return NULL;
}
#else
static void *LedCntrolDemo(const char *arg)
{
    (void)arg;

    printf("LedTask start\r\n");

    // 初始化GPIO
    IoTGpioInit(IOT_GPIO_IDX_RED_LED);
    // 设置GPIO为输出方向
    IoTGpioSetDir(IOT_GPIO_IDX_RED_LED, IOT_GPIO_DIR_OUT);
    // 配置GPIO引脚号和输出值
    IoTGpioSetOutputVal(IOT_GPIO_IDX_RED_LED, IOT_GPIO_VALUE1);

    return NULL;
}
#endif

static void LedControlTask(void)
{
    osThreadAttr_t attr;

    attr.name = "LedCntrolDemo";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = 1024; /* 堆栈大小为1024 */
    attr.priority = osPriorityNormal;

    if (osThreadNew((osThreadFunc_t)LedCntrolDemo, NULL, &attr) == NULL) {
        printf("[LedExample] Failed to create LedTask!\n");
    }
}

SYS_RUN(LedControlTask);
