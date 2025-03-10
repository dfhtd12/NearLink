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
#include <string.h>    
#include "ohos_init.h" 
#include "cmsis_os2.h" 
#include "wifi_connecter.h" 
#include "oled_ssd1306.h"      

//要连接热点的名称，根据实际情况修改
#define SSID "WIFI-0A02"

//要连接热点的密码，根据实际情况修改
#define PSK "1234567890"

//UDP服务器IP地址，根据实际情况修改
#define UDP_SERVER_IP "192.168.100.199"

//UDP服务器端口
#define UDP_SERVER_PORT 1234


static void TCPDemoTask(void *arg)
{
    (void)arg;

    // 连接热点
    if (ConnectToHotspot(SSID, PSK) != 0)
    {
        // 连接到热点失败
        printf("Connect to AP failed\n");                                   
        OledShowString(0, 2, "conn AP failed", FONT6x8);   
        return;
    }

    // 连接到热点成功
    OledShowString(0, 2,"                 ", FONT6x8);   
    OledShowString(0, 2,"conn AP success", FONT6x8);   
    osDelay(100);

    // 运行Udp客户端测试
    UdpClientTest(UDP_SERVER_IP, UDP_SERVER_PORT);
}

// 入口函数
static void TCPDemoEntry(void)
{
    // 初始化SSD1306
    OledInit();
    // 全屏黑色
    OledFillScreen(0x00);
    OledShowString(24, 0, "-UdpClient-", FONT6x8);   

    osThreadAttr_t attr;
    attr.name = "TCPDemoTask";
    attr.stack_size = 0x2000;
    attr.priority = osPriorityNormal;

    if (osThreadNew(TCPDemoTask, NULL, &attr) == NULL)
    {
         printf("[TCPDemoEntry] Falied to create TCPDemoTask!\n");
    }
}

// 运行入口函数
APP_FEATURE_INIT(TCPDemoEntry);
