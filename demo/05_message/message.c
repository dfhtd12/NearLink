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

#define QUEUE_SIZE 3
typedef struct{
    osThreadId_t tid;
    int count;
} message_entry;
osMessageQueueId_t qid;

void sender_thread(void *arg)
{
    static int count = 0;
    message_entry sentry;
    (void) arg;
    uint32_t counter  = 0;
    uint32_t running = 1;
    while (running) {
        sentry.tid = osThreadGetId();
        sentry.count = count;
        osal_printk("[Message Test] %s send %d to message queue.\r\n", osThreadGetName(osThreadGetId()), count);
        osMessageQueuePut(qid, (const void *) &sentry, 0, osWaitForever);
        count++;
        osDelay(5);
        counter++;
        if (counter >= 100) {
            running = 0;
        }
    }
}

void receiver_thread(void *arg)
{
    (void) arg;
    message_entry rentry;
    uint32_t counter = 0;
    uint32_t running = 1;

    while (running) {
        osMessageQueueGet(qid, (void *) &rentry, NULL, osWaitForever);
        osal_printk("[Message Test] %s get %d from %s by message queue.\r\n", osThreadGetName(osThreadGetId()),
                    rentry.count, osThreadGetName(rentry.tid));
        osDelay(3);
        counter++;
        if (counter >= 100) {
            running = 0;
        }
    }
}

osThreadId_t newThread(char *name, osThreadFunc_t func, void *arg)
{
    osThreadAttr_t attr = {
            name, 0, NULL, 0, NULL, 1024 * 2, osPriorityNormal, 0, 0
    };
    osThreadId_t tid = osThreadNew(func, arg, &attr);
    if (tid == NULL) {
        osal_printk("[Message Test] osThreadNew(%s) failed.\r\n", name);
    } else {
        osal_printk("[Message Test] osThreadNew(%s) success, thread id: %d.\r\n", name, tid);
    }
    return tid;
}

void rtosv2_msgq_main(void *arg)
{
    (void) arg;

    qid = osMessageQueueNew(QUEUE_SIZE, sizeof(message_entry), NULL);

    osThreadId_t ctid1 = newThread("recevier1", receiver_thread, NULL);
    osThreadId_t ctid2 = newThread("recevier2", receiver_thread, NULL);
    osThreadId_t ptid1 = newThread("sender1", sender_thread, NULL);
    osThreadId_t ptid2 = newThread("sender2", sender_thread, NULL);
    osThreadId_t ptid3 = newThread("sender3", sender_thread, NULL);

    osDelay(20);
    uint32_t cap = osMessageQueueGetCapacity(qid);
    osal_printk("[Message Test] osMessageQueueGetCapacity, capacity: %d.\r\n", cap);
    uint32_t msg_size = osMessageQueueGetMsgSize(qid);
    osal_printk("[Message Test] osMessageQueueGetMsgSize, size: %d.\r\n", msg_size);
    uint32_t count = osMessageQueueGetCount(qid);
    osal_printk("[Message Test] osMessageQueueGetCount, count: %d.\r\n", count);
    uint32_t space = osMessageQueueGetSpace(qid);
    osal_printk("[Message Test] osMessageQueueGetSpace, space: %d.\r\n", space);

    osDelay(80);
    osThreadTerminate(ctid1);
    osThreadTerminate(ctid2);
    osThreadTerminate(ptid1);
    osThreadTerminate(ptid2);
    osThreadTerminate(ptid3);
    osMessageQueueDelete(qid);
}

static void MessageTestTask(void)
{
    osThreadAttr_t attr;

    attr.name = "rtosv2_msgq_main";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = 0x1000;
    attr.priority = osPriorityNormal;

    if (osThreadNew((osThreadFunc_t) rtosv2_msgq_main, NULL, &attr) == NULL) {
        osal_printk("[MessageTestTask] Falied to create rtosv2_msgq_main!\n");
    }
}

APP_FEATURE_INIT(MessageTestTask);
