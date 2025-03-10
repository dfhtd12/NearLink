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
#include "pinctrl.h"
#include "adc.h"
#include "adc_porting.h"
#include "common_def.h"
#include "soc_osal.h"
#include "tcxo.h"
#include "ohos_init.h"  
#include "cmsis_os2.h"  
 
#define DELAY_10000MS                   10000
#define CYCLES                          10
#define ADC_TASK_PRIO                   26
#define ADC_TASK_STACK_SIZE             0x1000
 
static void *adc_task(const char *arg)
{
    unused(arg);
    printf("start adc sample\r\n");
    uapi_adc_init(ADC_CLOCK_NONE);
    uint8_t adc_channel = 2;
    uint16_t voltage = 0;
    int c = 100000;
    while (c) {
        adc_port_read(adc_channel, &voltage);
        printf("voltage: %d mv\r\n", voltage); 
        osal_msleep(100);
    }
    /* 当前测量的电压值和实际值可能有较大差别，请确认是否有分压电阻，如果有分压电阻，则差别符合预期 */
    uapi_adc_deinit();

    return NULL;
}

static void adc_entry(void)
{
  
    // 定义线程属性
    osThreadAttr_t attr;
    attr.name = "AdcTask";
    attr.stack_size = 0x1000;  
    attr.priority = osPriorityNormal;

    // 创建线程
    if (osThreadNew(adc_task, NULL, &attr) == NULL)
    {
        printf("[AdcExample] Falied to create AdcTask!\n");
    }


}

// 运行入口函数
SYS_RUN(adc_entry);