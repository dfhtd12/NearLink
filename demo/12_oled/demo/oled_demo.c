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
#include "oled_ssd1306.h"


static void OledTask(void *arg)
{
    (void)arg;

    // 初始化SSD1306
    OledInit();
    // 全屏黑色
    OledFillScreen(0x00);
    // 显示字符串
    OledShowString(20, 2, "Hello,World!", FONT8x16);   

}


static void OledDemo(void)
{
    osThreadAttr_t attr;
    attr.name = "OledTask";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = 0x1000;   
    attr.priority = osPriorityNormal;
    if (osThreadNew(OledTask, NULL, &attr) == NULL) {
        printf("[OledDemo] Falied to create OledTask!\n");
    }
}


APP_FEATURE_INIT(OledDemo);
