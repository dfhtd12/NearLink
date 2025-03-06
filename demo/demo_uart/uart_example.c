/*
# Copyright (C) 2024 HiHope Open Source Organization .
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
*/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "ohos_init.h"
#include "cmsis_os2.h"
#include "iot_uart.h"
#include "pinctrl.h"
#include "uart.h"

static unsigned char uart2WriteBuff[] = "  uart2";
static unsigned char uart1WriteBuff[] = "  uart1";
static uint8_t uart2ReadBuff[16];
static uint8_t uart1ReadBuff[16];

static void uart1_rx_callback(const void *buffer, uint16_t length, bool error)
{
    IoTUartWrite(1, (const unsigned char *)buffer, (uint32_t)length);
}

static void uart1_config(){
    uapi_uart_deinit(1);
     IotUartAttribute g_uart_cfg = {115200, 8, 1, IOT_UART_PARITY_NONE, 100, 100, 0}; 
      IoTUartInit(1, &g_uart_cfg); 
    (void)uapi_uart_register_rx_callback(1, UART_RX_CONDITION_FULL_OR_SUFFICIENT_DATA_OR_IDLE,
                                        16, uart1_rx_callback);
}

static void uart2_rx_callback(const void *buffer, uint16_t length, bool error)
{
    IoTUartWrite(2, (const unsigned char *)buffer, (uint32_t)length);
}

void uart2_config(void)
{
     uapi_uart_deinit(2);
     IotUartAttribute g_uart_cfg = {115200, 8, 1, IOT_UART_PARITY_NONE, 100, 100, 0}; 
      IoTUartInit(2, &g_uart_cfg); 
    (void)uapi_uart_register_rx_callback(2, UART_RX_CONDITION_FULL_OR_SUFFICIENT_DATA_OR_IDLE,
                                         16, uart2_rx_callback);
}



//1.任务处理函数 
static void* UartDemo_Task(const char* arg) 
{ 
   (void)arg; 
   uart1_config();
   uart2_config();
   IoTUartWrite(1, uart1WriteBuff, sizeof(uart1WriteBuff));
   IoTUartWrite(2, uart2WriteBuff, sizeof(uart2WriteBuff));
   return NULL; 
} 

 
//2.任务入口函数 
static void UartDemo_Entry(void) 
{ 
    osThreadAttr_t attr = {0}; 
 
    printf("[UartDemo] UartDemo_Entry\n"); 
 
    attr.name = "UartDemo_Task"; 
    attr.attr_bits = 0U; 
    attr.cb_mem = NULL; 
    attr.cb_size = 0U; 
    attr.stack_mem = NULL; 
    attr.stack_size = 1024;//堆栈大小 
    attr.priority = osPriorityNormal;//优先级 
 
    if (osThreadNew((osThreadFunc_t)UartDemo_Task, NULL, &attr) == NULL) 
   { 
        printf("[UartDemo] Falied to create UartDemo_Task!\n"); 
    }else  printf("[[UartDemo] create UartDemo_Task]");

   
} 
 
//3.注册模块 
SYS_RUN(UartDemo_Entry); 
