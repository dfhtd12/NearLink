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
#include <stdlib.h>   
#include <string.h>    
#include "ohos_init.h" 
#include "cmsis_os2.h" 
#include "unistd.h" 

#include "MQTTClient.h"     // MQTTClient-C库接口文件
#include "mqtt_ohos.h"      // OHOS适配接口文件

#include "iot_gpio.h"   
#include "iot_gpio_ex.h"
#include "iot_errno.h" 

#include "aht20.h"     

#define AHT20_BAUDRATE 400000

#define AHT20_I2C_IDX 1

//要连接热点的名称，根据实际情况修改
#define SSID "WIFI-0A02"

//要连接热点的密码，根据实际情况修改
#define PSK "1234567890"

// MQTT服务器IP地址（默认是电脑的地址）,请根据实际情况修改
#define MQTT_SERVER_IP "192.168.100.198"

// MQTT服务器端口,请根据实际情况修改
#define MQTT_SERVER_PORT "1888"   

// MQTT客户端
static MQTTClient client = {0};

// MQTT网络连接
static Network network = {0};

// 发送和接收的数据缓冲区
static unsigned char sendbuf[100], readbuf[100];

//MQTT消息
MQTTMessage message;

//温湿度数据
float temperature;
float humidity;
//有效数据
char payload[100] = {0};                             
//初始化传感器
static void InitTempHumiSensor()
{

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
}
//获取温湿度
static uint16_t GetTempHumi(float *temperature, float *humidity){
    char tempData[15] = {0};
    char humiData[15] = {0};
    // 启动测量
    if (AHT20_StartMeasure() != IOT_SUCCESS)
    {
        printf("measure failed!\r\n");
        return IOT_FAILURE;
    }
    //获取测量结果
    if (AHT20_GetMeasureResult(temperature, humidity) != IOT_SUCCESS)
    {
        printf("get data failed!\r\n");
        return IOT_FAILURE;
    }
    //打印温湿度数据
    sprintf(tempData, "%.2f", *temperature);
    sprintf(humiData, "%.2f", *humidity);
    printf("temperature: %s, humidity: %s\r\n", tempData, humiData);

    return IOT_SUCCESS;
}

static void mqttDemoTask(void *arg)
{
    (void)arg;

    InitTempHumiSensor();
    // 连接到热点
    if (ConnectToHotspot(SSID, PSK) != 0)
    {
        // 连接到热点失败
        printf("Connect to AP failed\r\n"); 
        return;
    }

    // 等待TCP连接完成
    osDelay(100);

    //初始化MQTT相关的参数和回调
    NetworkInit(&network);
    // 初始化MQTT客户端
    MQTTClientInit(&client, &network, 200, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
  
    const char *host = MQTT_SERVER_IP;                               // MQTT服务器IP地址
    unsigned short port = atoi(MQTT_SERVER_PORT);                      // MQTT服务器端口
    const char *clientId = "client_test";                              // MQTT客户端ID
    const char *username = NULL;                                        // MQTT服务器用户名
    const char *passwords = NULL;                                        // MQTT服务器密码

    // 初始化MQTT连接信息
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    // 用于接收接口返回值
    int ret = NetworkConnect(&network, (char *)host, port);

    if(ret != 0){
        printf("TCP Connect failed!,ret = %d\r\n",ret);
        return;
    }

    // 设置用户名和密码
    if (username != NULL && passwords != NULL)
    {
        connectData.username.cstring = (char *)username;
        connectData.password.cstring = (char *)passwords;
    }

    /* 设置MQTT版本*/
    // 3 = 3.1
    // 4 = 3.1.1
    connectData.MQTTVersion = 3;

    // 设置MQTT客户端ID
    connectData.clientID.cstring = (char *)clientId;

    // 发送MQTT连接包
    if ((ret = MQTTConnect(&client, &connectData)) != 0)
    {
        // 连接失败
        printf("Connect MQTT Broker failed!,ret = %d\r\n",ret);
        return;
    }else{
       // 成功连接到MQTT服务器
        printf("MQTT Connected!\r\n");
    }

    //发布的主题
    char *topic = "device_sensor_data";         
    int c = 1;
    while (c)
    {
      uint16_t ret = GetTempHumi(&temperature, &humidity);
      if(ret == 0){
            sprintf(payload,
            "{\"device_name\":\"test_decice_01\", \"temp\":%.2f, \"humi\":%.2f}",
            temperature, humidity);         
            message.qos = QOS0;
            message.retained = 0;   
            message.payload = payload;
            message.payloadlen = strlen(payload);
            // 发布消息    
            ret = MQTTPublish(&client, topic, &message);         
            if (ret != 0)                                    
                printf("MQTT Publish failed!\r\n");
            else
                printf("MQTT Publish OK\r\n");
        }    
       
        osDelay(100);                                       
    }
    
    
}

// 入口函数
static void mqttDemoEntry(void)
{
    // 定义线程属性
    osThreadAttr_t attr;
    attr.name = "mqttDemoTask";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = 0x1000;
    attr.priority = osPriorityNormal;

    // 创建线程
    if (osThreadNew(mqttDemoTask, NULL, &attr) == NULL)
    {
        printf("[mqttDemoEntry] Falied to create mqttDemoTask!\n");
    }
}

// 运行入口函数
APP_FEATURE_INIT(mqttDemoEntry);
