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
#include "ohos_init.h"
#include "cmsis_os2.h"

#include "iot_gpio.h"
#include "iot_gpio_ex.h"
#include "iot_pwm.h"

#define PWM_GPIO IOT_IO_NAME_GPIO_10    // PWM对应的GPIO引脚
#define PWM_PORT 2                      // PWM端口
#define Frequency 1230   //pwm接口不能输出低于1230hz的pwm,只能通过IO模拟的方式输出

static void PwmTask(void *arg)
{
    (void) arg;

    // 初始化GPIO模块
    IoTGpioInit(PWM_GPIO);
    // 设置GPIO10为PWM模式
    uint32_t ret = IoSetFunc(PWM_GPIO, 1);
    printf("[PwmExample] IoSetFunc ret: %d\r\n", ret);
    // 初始化PWM模块
    ret = IoTPwmInit(PWM_PORT);
    printf("[PwmExample] IoTPwmInit ret: %d\r\n", ret);
    uint32_t counter  = 0;
    uint32_t running = 1;

    while (running) {
        for (int i = 0; i < 100; i++) {
            IoTPwmStart(PWM_PORT, i, Frequency);
            osDelay(5);
            uapi_pwm_clear_group(PWM_PORT);
        }
        counter++;
        if (counter >= 100) {
            running = 0;
        }
    }
}

// 入口函数
static void PwmEntry(void)
{
    // 定义线程属性
    osThreadAttr_t attr;
    attr.name = "PwmTask";
    attr.stack_size = 2048;
    attr.priority = osPriorityNormal;
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;

    // 创建线程
    if (osThreadNew(PwmTask, NULL, &attr) == NULL) {
        printf("[PwmExample] Falied to create PwmTask!\n");
    }
}

// 运行入口函数
SYS_RUN(PwmEntry);