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
#include <errno.h>          
#include <string.h>        
#include "lwip/sockets.h"   // lwIP TCP/IP协议栈
#include "oled_ssd1306.h"  

//收发数据
static char send_data[10] = "test_data";
static char recv_data[100] = "";
static char display_data[11] = "";

int c = 1;
void UdpServerTest(unsigned short port)
{

    ssize_t ret = 0;

    // 创建一个UDP Socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // 客户端的IP地址和端口号
    struct sockaddr_in clientAddr = {0};

    // 客户端地址的长度
    socklen_t clientAddrLen = sizeof(clientAddr);

    // 服务端的地址信息
    struct sockaddr_in serverAddr = {0};

    // 配置服务端的信息
  
    serverAddr.sin_family = AF_INET;  // 使用IPv4协议

    serverAddr.sin_port = htons(port);//端口号，从主机字节序转为网络字节序

    // 允许任意主机接入
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    // 将sockfd和本服务器IP、端口号绑定
    ret = bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

    if (ret < 0)
    {
        // 绑定失败
       printf("bind failed, %ld!\r\n", ret);                      
        // 关闭socket
       lwip_close(sockfd);                                            
    }

    // 绑定成功
    printf("bind to port %d success!\r\n", port);

    // OLED显示端口号
    char strPort[5] = {0};
    snprintf(strPort, sizeof(strPort), "%d", port);
    OledShowString(0, 2, "                 ", FONT6x8);
    OledShowString(0, 2, "udp server port:", FONT6x8);   
    OledShowString(0, 3, strPort, FONT6x8);   
    
    // 向客户端的IP地址和端口号发送消息
    ret = sendto(sockfd, send_data, strlen(send_data), 0, (struct sockaddr *)&clientAddr, sizeof(clientAddr));

    // 发送失败
    if (ret < 0)
    {
       printf("send failed, %ld!\r\n", ret);
    }

    // 发送成功
    printf("send_data {%s} %ld done!\r\n", send_data, ret);

    while (c)
    {
        memset(display_data,0,strlen(display_data));
        memset(recv_data,0,strlen(recv_data));
        // 接收来自客户端的消息，并用客户端的IP地址和端口号填充clientAddr
        ret = recvfrom(sockfd, recv_data, sizeof(recv_data), 0, (struct sockaddr *)&clientAddr, &clientAddrLen);

        // 接收失败
        if (ret < 0)
        {
        printf("recvfrom failed, %ld!\r\n", ret);
        }

        // 接收成功，输出日志
        printf("recv recv_data {%s} %ld done!\r\n", recv_data, ret);

        // 显示发送方的地址信息
        printf("peer info: ipaddr = %s, port = %d\r\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

        // OLED显示收到的数据,最多显示10个字节
        OledShowString(0, 4, "recv:", FONT6x8);   
        OledShowString(40, 4,"          ", FONT6x8);
        memcpy(display_data,recv_data,10);
        display_data[11] = '\0';
        OledShowString(40,4,display_data, FONT6x8);
        osDelay(100);  
    }
    

        // 关闭socket
        //lwip_close(sockfd);
}
