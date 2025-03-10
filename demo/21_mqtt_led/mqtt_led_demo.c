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

//要连接热点的名称，根据实际情况修改
#define SSID "WIFI-0A02"

//要连接热点的密码，根据实际情况修改
#define PSK "1234567890"

// MQTT服务器IP地址（默认是电脑的地址）,请根据实际情况修改
#define MQTT_SERVER_IP "192.168.100.198"

// MQTT服务器端口,请根据实际情况修改
#define MQTT_SERVER_PORT "1888"

//交通板红色LED
#define RED_GPIO 7     

// MQTT客户端
static MQTTClient client = {0};

// MQTT网络连接
static Network network = {0};

// 发送和接收的数据缓冲区
static unsigned char sendbuf[100], readbuf[100];

//MQTT消息
MQTTMessage message;


/// 初始化交通灯板红色LED的引脚
void InitLED(void)
{
    usleep(1000000);
    // 初始化GPIO模块
    IoTGpioInit(RED_GPIO);     
    // 设置GPIO7为GPIO功能。
    IoSetFunc(RED_GPIO, IOT_IO_FUNC_GPIO_7_GPIO);
    // 设置GPIO7为输出模式（引脚方向为输出）。
    IoTGpioSetDir(RED_GPIO, IOT_GPIO_DIR_OUT);
}

// uint16_t state;
// uint16_t param;
// 接收订阅消息的回调函数
static void OnMessageArrived(MessageData *data)
{


    printf("Message form \r\n%s\r\n",
    data->topicName->lenstring.data);
    char* ret = strstr(data->message->payload, "state");
    char *ptr;
    uint8_t i = 0;
    if (ret != NULL){

    while (ret[i] != '1'&&ret[i] != '0')
    {
        i++;
        if (ret[i] == '1')
        {
            printf("LED_ON!!\r\n");
            IoTGpioSetOutputVal(RED_GPIO,1);
        }
        else if(ret[i] == '0'){
            printf("LED_OFF!!\r\n");
            IoTGpioSetOutputVal(RED_GPIO,0);
        }
        
    }
}
    else printf("can not find state\n");
 


}

static void MqttTask(void *arg)
{
    MQTTClient *c = (MQTTClient *)arg;
    while (c)
    {
        mqttMutexLock(&c->mutex);
        if (c->isconnected)
        {
            //接收订阅的数据
            MQTTYield(c, 1);
        }
        mqttMutexUnlock(&c->mutex);

        osDelay(100);
    }
}

static void mqttDemoTask(void *arg)
{
    (void)arg;

    InitLED();
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

    char *topic = "control_led_topic";            // 订阅主题

    if ((ret = MQTTSubscribe(&client, topic, QOS2, OnMessageArrived)) != 0)
    {
        // 订阅失败
        printf("MQTTSubscribe failed: %d\r\n", ret);
        return;
    }else{
        // 输出订阅成功信息
        printf("MQTT Subscribe OK\r\n");
    }
    
    // 创建MQTT任务线程,接收订阅主题的消息
     ret = ThreadStart(&client.thread, MqttTask, &client);
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
