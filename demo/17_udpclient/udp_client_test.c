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

// 发送的数据
static char request[] = "send_data";
// 接收的数据
static char response[100] = "";
// OLED显示的数据
static char display_data[11] = "";
int c = 1;

/* @brief UDP客户端测试函数
   @param1 host UDP服务端IP地址
   @param2 port UDP服务端端口
*/
void UdpClientTest(const char *host, unsigned short port)
{
    ssize_t ret = 0;

    // 创建一个UDP Socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // 用于设置服务端的地址信息
    struct sockaddr_in toAddr = {0};

    // 使用IPv4协议
    toAddr.sin_family = AF_INET;

    // 端口号，从主机字节序转为网络字节序
    toAddr.sin_port = htons(port);

    // 将服务端IP地址转化为标准格式
    if (inet_pton(AF_INET, host, &toAddr.sin_addr) <= 0)
    {
        // 转化失败
        printf("inet_pton failed!\r\n");  
        // 关闭socket
        lwip_close(sockfd);
    }

    // 发送数据API
    // UDP socket每次发送都必须先指定目标主机和端口，主机可以是多播地址,发送数据的时候，使用本地随机端口 
    /*
    -参数-
    s：socket文件描述符
    dataptr：要发送的数据
    size：要发送的数据的长度，最大65332字节
    flags：消息传输标志位
    to：目标的地址信息
    tolen：目标的地址信息长度
    -返回值-
     发送的字节数，如果出错，返回-1
     */
    ret = sendto(sockfd, request, sizeof(request), 0, (struct sockaddr *)&toAddr, sizeof(toAddr));
    if (ret < 0)
    {
        // 发送失败
        printf("sendto failed!\r\n");   
    }

    // 发送成功
    printf("send UDP message {%s} %ld done!\r\n", request, ret);
    // 发送方的地址信息
    struct sockaddr_in fromAddr = {0};

    //地址信息长度
    socklen_t fromLen = sizeof(fromAddr);


    while (c)
    {
        memset(display_data,0,strlen(display_data));
        memset(response,0,strlen(response));    

        /*在本地随机端口上面接收数据,可以通过fromAddr参数可以得到发送方的信息（主机、端口号）*/
        /*
         -参数-
         s：socket文件描述符
         buffer：接收数据的缓冲区的地址
         length：接收数据的缓冲区的长度
         flags：消息接收标志位
         address：发送方的地址信息
         address_len：发送方的地址信息长度
        
         -返回值-
         接收的字节数，如果出错，返回-1
        */
        ret = recvfrom(sockfd, &response, sizeof(response), 0, (struct sockaddr *)&fromAddr, &fromLen);

        // 检查接口返回值，小于0表示接收失败
        if (ret <= 0)
        {
            // 接收失败，或者收到0长度的数据
            printf("recvfrom failed or abort, %ld, %d!\r\n", ret, errno);  
        }

        response[ret] = '\0';

        // 显示发送方的地址信息
        printf("peer info: ipaddr = %s, port = %d\r\n", inet_ntoa(fromAddr.sin_addr), ntohs(fromAddr.sin_port));
        // 接收成功，输出日志，OLED显示收到的收据,最多显示10个字符
        printf(" recv_data{%s} from server!\r\n", response);
        OledShowString(0, 5,"recv:", FONT6x8);
        OledShowString(40, 5,"          ", FONT6x8);
        memcpy(display_data,response,10);
        display_data[11] = '\0';
        OledShowString(40, 5,display_data, FONT6x8);

        osDelay(100); 

    }
   
    // 关闭socket
   // lwip_close(sockfd);
}
