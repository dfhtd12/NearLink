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
#include "lwip/sockets.h"  
#include "osal_debug.h"
#include "oled_ssd1306.h"       

static char request[50] = "test_data";
static char response[100] ;
static char display_data[11];

/* @brief TCP客户端测试函数
   @param1 host TCP服务端IP地址
   @param2 port TCP服务端端口
*/
void TcpClientTest(const char *host, unsigned short port)
{
    ssize_t ret = 0;
    int c = 1;
    // 创建一个TCP Socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // 设置服务端的地址信息
    struct sockaddr_in serverAddr = {0};

    // 设置服务端的信息
    serverAddr.sin_family = AF_INET;    // 使用IPv4协议
    serverAddr.sin_port = htons(port);  // 端口号

    // 将服务端IP地址转化为标准格式
    if (inet_pton(AF_INET, host, &serverAddr.sin_addr) <= 0)
    {
        // 转化失败
         printf("inet_pton failed!\r\n");   
         // 关闭连接
        closesocket(sockfd);
    }

    // 建立连接
    if (connect(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        // 连接失败
        printf("connect tcp server failed!\r\n");     
        OledShowString(0, 2, "               ", FONT6x8);  
        OledShowString(0, 2, "conn tcp failed", FONT6x8);   
        // 关闭连接
        closesocket(sockfd);
    }

    // 连接成功
    printf("connect to tcp server %s success!\r\n", host);
    // OLED显示服务端IP地址

    OledShowString(0, 2, "conn tcp success", FONT6x8);
    OledShowString(0, 4, "tcp server ip:", FONT6x8); 
    OledShowString(0, 5, host, FONT6x8);   

    // 发送数据
    ret = send(sockfd, request, sizeof(request), 0);

    // 检查接口返回值，小于0表示发送失败
    if (ret < 0)
    {
        // 发送失败
        printf("send request failed!\r\n");
    }

    // 发送成功
     printf("send request{%s} %ld to tcp server !\r\n", request, ret);

    while (c)
    {
    memset(display_data,0,strlen(display_data));
    memset(response,0,strlen(response));
    // 接收收据
    ret = recv(sockfd, response, sizeof(response), 0);

    // 接收失败或对方的通信端关闭
    if (ret <= 0) {
        printf(" recv_data failed or done, %ld!\r\n", ret);
    }

    // 接收成功，输出日志，OLED显示收到的收据,最多显示10个字符
    printf(" recv_data{%s} from client!\r\n", response);
    OledShowString(0, 7,"recv:", FONT6x8);
    OledShowString(40, 7,"          ", FONT6x8);
 
  
    memcpy(display_data,response,10);
    display_data[11] = '\0';
    
    OledShowString(40, 7,display_data, FONT6x8);
    osDelay(100); 

    }
   

    // 关闭socket
   // closesocket(sockfd);
}
