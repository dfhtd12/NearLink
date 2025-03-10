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

#include "MQTTClient.h"     // MQTTClient-C库接口文件
#include "mqtt_ohos.h"      // OHOS适配接口文件


//要连接热点的名称，根据实际情况修改
#define SSID "HUAWEI_Gao"

//要连接热点的密码，根据实际情况修改
#define PSK "gao51920"

// MQTT服务器IP地址（默认是电脑的地址）,请根据实际情况修改
#define MQTT_SERVER_IP "192.168.8.31"

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

// 接收订阅消息的回调函数
static void OnMessageArrived(MessageData *data)
{

    printf("Message form \r\n%s\r\n",
    data->topicName->lenstring.data);

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
    const char *pass = NULL;                                        // MQTT服务器密码

    // 初始化MQTT连接信息
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    // 用于接收接口返回值
    int ret = NetworkConnect(&network, (char *)host, port);

    if(ret != 0){
        printf("TCP Connect failed!,ret = %d\r\n",ret);
        return;
    }

    // 设置用户名和密码
    if (username != NULL && pass != NULL)
    {
        connectData.username.cstring = (char *)username;
        connectData.password.cstring = (char *)pass;
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

    char *topic = "test_topic/a";            // 订阅主题:test_topic/a

    if ((ret = MQTTSubscribe(&client, topic, QOS2, OnMessageArrived)) != 0)
    {
        // 订阅失败
        printf("MQTTSubscribe failed: %d\r\n", ret);
        return;
    }else{
        // 输出订阅成功信息
        printf("MQTT Subscribe OK\r\n");
    }
    
    char *stopic = "test_topic/b"; // 主题:test_topic/b
    char *payload = "Hello!"; // 消息内容

    //消息质量为QOS2,只会收到一条消息
    message.qos = QOS2;

    
    /*
    message.retained = 1,MQTT服务器收到这样的MQTTMessage数据包以后，将保存这个消息，
     当有一个新的订阅者订阅相应主题的时候，MQTT服务器会马上将这个消息发送给订阅.
     message.retained = 0，订阅者连接到MQTT服务器时，不会接收该主题的最新消息.
     */
    message.retained = 0;

    // 设置消息的内容
    message.payload = payload;

    // 设置消息的长度
    message.payloadlen = strlen(payload);

    // 发布消息
    if ((ret = MQTTPublish(&client, stopic, &message)) != 0)
    {
        // 发布消息失败
        printf("MQTTPublish failed: %d\r\n", ret);
        return ;
    }else{
        // 发布成功
        printf("MQTT Publish OK\r\n"); 
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
