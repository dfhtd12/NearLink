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
#include <stddef.h>        
#include <errno.h>          
#include <string.h>        
#include "lwip/sockets.h" 
#include "oled_ssd1306.h"        

static char send_data[10] = "test_data";
static char recv_data[100] = "";
static char display_data[11] = "";

/*TCP服务端测试函数*/
void TcpServerTest(unsigned short port)
{
    ssize_t ret = 0;
    int c = 1;
    // 最大等待队列长度
    int backlog = 1;

    // 创建一个TCP Socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    int connfd = -1;

    // 客户端的IP地址和端口号
    struct sockaddr_in clientAddr = {0};

    // clientAddr的长度
    socklen_t clientAddrLen = sizeof(clientAddr);

    // 服务端的地址信息
    struct sockaddr_in serverAddr = {0};

    /* 配置服务端的信息 */
    
    serverAddr.sin_family = AF_INET;//使用IPv4协议

    serverAddr.sin_port = htons(port);//端口号，从主机字节序转为网络字节序

    // 允许任意主机接入
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    // 将sockfd和本服务器IP、端口号绑定
    ret = bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

    if (ret < 0) {
        // 绑定失败
        printf("bind port failed, %ld!\r\n", ret);                        
        OledShowString(0, 2,"                ", FONT6x8); 
        OledShowString(0, 2,"bind port failed", FONT6x8);   
        lwip_close(sockfd);                                           

       
    }

    // 绑定成功
    printf("bind to port %d success!\r\n", port);

    // OLED显示端口号
    OledShowString(0, 1,"                 ", FONT6x8);
    OledShowString(0, 1,"serverPort:", FONT6x8);
    OledShowString(0, 2,"                 ", FONT6x8);
    char strPort[5] = {0};
    snprintf(strPort, sizeof(strPort), "%d", port);
    OledShowString(90, 1,strPort, FONT6x8);


    // 开始监听，最大等待队列长度为backlog
    // 使用listen接口将sockfd设置为监听模式，在指定的IP和端口上监听客户端发起的连接请求。
    ret = listen(sockfd, backlog);

    // 检查接口返回值，小于0表示监听失败
    if (ret < 0) {
        // 监听失败
        printf("listen fail!\r\n");                               
        lwip_close(sockfd);    
    }

    // 监听成功
    printf("listen with %d backlog success!\r\n", backlog);


    /* 使用accept接口阻塞式等待客户端连接，创建一个新socket与客户端建立连接。
    */
    connfd = accept(sockfd, (struct sockaddr *)&clientAddr, &clientAddrLen);

    if (connfd < 0) {
        // 建立连接失败
        printf("accept failed, %d, %d\r\n", connfd, errno);     
        lwip_close(sockfd);                                
    }

    // 建立连接成功
    printf("accept success, connfd = %d!\r\n", connfd);
    printf("client addr info: host = %s, port = %d\r\n",
    inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

    // OLED显示客户端IP地址
    OledShowString(0, 3,"udp client ip:", FONT6x8);
    OledShowString(0, 4,inet_ntoa(clientAddr.sin_addr), FONT6x8);

    // 发送数据
    ret = send(connfd, send_data, strlen(send_data), 0);

    // 发送失败
    if (ret < 0) {
        printf("send response failed, %ld!\r\n", ret);
    }

    // 发送成功，输出日志，OLED显示发送的数据
    printf("send response{%s} to client done!\r\n", send_data);
    while (c)
    {
        memset(display_data,0,strlen(display_data));
        memset(recv_data,0,strlen(recv_data));
        // 接收收据
        ret = recv(connfd, recv_data, sizeof(recv_data), 0);

        // 接收失败或对方的通信端关闭
        if (ret <= 0) {
            printf(" recv_data failed or done, %ld!\r\n", ret);
        }

        // 接收成功，输出日志，OLED显示收到的收据,最多显示10个字符
        printf(" recv_data{%s} from client!\r\n", recv_data);
        OledShowString(0, 7,"recv:", FONT6x8);
        OledShowString(40, 7,"          ", FONT6x8);
    
    
        memcpy(display_data,recv_data,10);
        display_data[11] = '\0';
        
        OledShowString(40, 7,display_data, FONT6x8);
        osDelay(100); 

    }
    
     

    //关闭监听socket
    //lwip_close(sockfd);
    //关闭连接
    //lwip_close(connfd);

   
}
