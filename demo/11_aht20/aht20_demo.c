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
#include "iot_i2c.h"    
#include "iot_errno.h"  
#include "aht20.h"      
#define AHT20_BAUDRATE 400000
#define AHT20_I2C_IDX 1

static float humidity = 0;
static float temperature = 0;
static char tempData[15] = {0};
static char humiData[15] = {0};
static void AHT20_Task(void *arg)
{
    (void)arg;
    int c = 10000;
    // 初始化GPIO
    IoTGpioInit(1); 

    // 设置GPIO-15引脚功能为I2C1_SDA
    IoSetFunc(IOT_IO_NAME_GPIO_15, IOT_IO_FUNC_GPIO_15_I2C1_SDA);
    // 设置GPIO-16引脚功能为I2C1_SCL
    IoSetFunc(IOT_IO_NAME_GPIO_16, IOT_IO_FUNC_GPIO_16_I2C1_SCL);

    // 初始化I2C1
    IoTI2cInit(AHT20_I2C_IDX, AHT20_BAUDRATE);


    // 校准AHT20,需要等待一段时间
    while (IOT_SUCCESS != AHT20_Calibrate())
    {
        printf("AHT20 sensor init failed!\r\n");
       
    }

   
    while (c)
    {
        // 启动测量
        if (AHT20_StartMeasure() != IOT_SUCCESS)
        {
            printf("measure failed!\r\n");
        }
        //获取测量结果
        if (AHT20_GetMeasureResult(&temperature, &humidity) != IOT_SUCCESS)
        {
            printf("get data failed!\r\n");
        }
        //打印温湿度数据
        sprintf(tempData, "%.2f", temperature);
        sprintf(humiData, "%.2f", humidity);
        printf("temperature: %s, humidity: %s\r\n", tempData, humiData);

        osDelay(100);
    }
}


static void AHT20_Demo(void)
{
    
    osThreadAttr_t attr;
    attr.name = "AHT20_Task";
    attr.stack_size = 0x1500;  
    attr.priority = osPriorityNormal;
    if (osThreadNew(AHT20_Task, NULL, &attr) == NULL)
    {
        printf("[AHT20_Task]Falied to create AHT20_Task!\n");
    }
}


APP_FEATURE_INIT(AHT20_Demo);
