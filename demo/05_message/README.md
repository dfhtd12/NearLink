# HopeRun IoT Development Kit--MessageQueue

![hihope_illustration](../../Image/hihope_illustration.png)

An integrated software and hardware development kit based on WS63E solution, providing a comprehensive suite for embedded system development.

![wifi_iot](../../Image/HH-K01.png)

## 一、MessageQueue API

| API                       | Description                                                  |
| ------------------------- | ------------------------------------------------------------ |
| osMessageQueueNew         | Create and initialize a message queue                        |
| osMessageQueueGetName     | Get the name of the specified message queue                  |
| osMessageQueuePut         | Send one message to the specified message queue; if the message queue is full, return a timeout |
| osMessageQueueGet         | Receive one message from the specified message queue; if the message queue is empty, return a timeout |
| osMessageQueueGetCapacity | Get the capacity of the specified message queue              |
| osMessageQueueGetMsgSize  | Get the maximum message size that can be stored in the specified message queue |
| osMessageQueueGetCount    | Get the current number of messages in the specified message queue |
| osMessageQueueGetSpace    | Get the number of additional messages that can be stored in the specified message queue |
| osMessageQueueReset       | Reset the specified message queue to its initial state       |
| osMessageQueueDelete      | Delete the specified message queue                           |

## 2. Code

`osMessageQueueNew` creates a message queue

The sender sends its `count` value and the thread ID each time, and then increments `count` by 1

The receiver retrieves a message from the message queue and then prints it out

```c
void sender_thread(void *arg) {
    static int count=0;
    message_entry sentry;
    (void)arg;
    while(1) {
        sentry.tid = osThreadGetId();
        sentry.count = count;
        printf("[Message Test] %s send %d to message queue.\r\n", osThreadGetName(osThreadGetId()), count);
        osMessageQueuePut(qid, (const void *)&sentry, 0, osWaitForever);
        count++;
        osDelay(5);
    }
}

void receiver_thread(void *arg) {
    (void)arg;
    message_entry rentry;
    while(1) {
        osMessageQueueGet(qid, (void *)&rentry, NULL, osWaitForever);
        printf("[Message Test] %s get %d from %s by message queue.\r\n", osThreadGetName(osThreadGetId()), rentry.count, osThreadGetName(rentry.tid));
        osDelay(3);
    }
}
```

The main program creates three message senders and two message receivers, and then calls the relevant APIs to confirm the filling of the message queue

```c
void rtosv2_msgq_main(void *arg) {
    (void)arg;

    qid = osMessageQueueNew(QUEUE_SIZE, sizeof(message_entry), NULL);

    osThreadId_t ctid1 = newThread("recevier1", receiver_thread, NULL);
    osThreadId_t ctid2 = newThread("recevier2", receiver_thread, NULL);
    osThreadId_t ptid1 = newThread("sender1", sender_thread, NULL);
    osThreadId_t ptid2 = newThread("sender2", sender_thread, NULL);
    osThreadId_t ptid3 = newThread("sender3", sender_thread, NULL);

    osDelay(20);
    uint32_t cap = osMessageQueueGetCapacity(qid);
    printf("[Message Test] osMessageQueueGetCapacity, capacity: %d.\r\n",cap);
    uint32_t msg_size =  osMessageQueueGetMsgSize(qid);
    printf("[Message Test] osMessageQueueGetMsgSize, size: %d.\r\n",msg_size);
    uint32_t count = osMessageQueueGetCount(qid);
    printf("[Message Test] osMessageQueueGetCount, count: %d.\r\n",count);
    uint32_t space = osMessageQueueGetSpace(qid);
    printf("[Message Test] osMessageQueueGetSpace, space: %d.\r\n",space);

    osDelay(80);
    osThreadTerminate(ctid1);
    osThreadTerminate(ctid2);
    osThreadTerminate(ptid1);
    osThreadTerminate(ptid2);
    osThreadTerminate(ptid3);
    osMessageQueueDelete(qid);
}
```

## 3. **Compile**

1. Copy the `05_message` directory to the `applications\sample\wifi-iot\app` directory within the Oniro source code.
2. Modify the `BUILD.gn` in the `applications\sample\wifi-iot\app` directory of the Oniro source code by replacing the `features` variable with:

```
    features = [
        ...
        "05_message:message_demo",
        ...
    ]
```
3. In the file `config.py` located at `device\soc\hisilicon\ws63v100\sdk\build\config\target_config\ws63`, locate the section labeled `'ws63-liteos-app'`. Within this section, add the following code to the `'ram_component'` field:
```
"message_demo"
```

4. In the file `ohos.cmake` located at `device\soc\hisilicon\ws63v100\sdk\libs_url\ws63\cmake`, locate the section labeled `"ws63-liteos-app"`. Within this section, find the `set(COMPONENT_LIST` statement and append the following code to its argument list:
```
"message_demo"
```
5. Execute the following command in the root directory of the Oniro SDK: `rm -rf out && hb set -p nearlink_dk_3863 && hb build -f`

## 4. Result

**Extract part of the runtime results**

```
[Message Test] recevier1 get 4 from sender2 by message queue.
[Message Test] sender1 send 6 to message queue.
[Message Test] recevier2 get 5 from sender3 by message queue.
[Message Test] sender2 send 7 to message queue.
[Message Test] sender3 send 8 to message queue.
[Message Test] recevier1 get 6 from sender1 by message queue.
[Message Test] recevier2 get 7 from sender2 by message queue.
[Message Test] sender1 send 9 to message queue.
[Message Test] recevier1 get 8 from sender3 by message queue.
[Message Test] sender2 send 10 to message queue.
[Message Test] sender3 send 11 to message queue.
[Message Test] recevier2 get 9 from sender1 by message queue.
[Message Test] recevier1 get 10 from sender2 by message queue.
[Message Test] recevier2 get 11 from sender3 by message queue.
[Message Test] osMessageQueueGetCapacity, capacity: 3.
[Message Test] osMessageQueueGetMsgSize, size: 8.
[Message Test] osMessageQueueGetCount, count: 0.
[Message Test] osMessageQueueGetSpace, space: 3.
[Message Test] sender1 send 12 to message queue.
[Message Test] sender2 send 13 to message queue.
[Message Test] sender3 send 13 to message queue.
[Message Test] recevier1 get 12 from sender1 by message queue.
[Message Test] recevier2 get 13 from sender2 by message queue.
```

### 【Dev-kits】

##### 1. Online marketplaces  https://item.taobao.com/item.htm?abbucket=16&id=816685710481&ns=1&priceTId=214783b117346662457694855ed644&skuId=5533042544092&spm=a21n57.sem.item.49.46a639031zWytE&utparam=%7B%22aplus_abtest%22%3A%22b28048df8f009463834be6bdac2a3713%22%7D&xxc=taobaoSearch

##### 2. **Technical Documentation**

- **Gitee CodeCloud** (User Manuals, Specifications, Oniro Development Cases) **https://gitee.com/hihopeorg_group/near-link**
- **fbb_ws63 Repository** (SDK Packages, Technical Documentation Downloads)**https://gitee.com/HiSpark/fbb_ws63**

##### 3. **Interaction and Support**

- **Hisilicon Community - NearLink Zone Forum** **https://developer.hisilicon.com/forum/0133146886267870001**
