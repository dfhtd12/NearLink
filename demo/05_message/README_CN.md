# 润和星闪派物联网开发套件--消息队列（MessageQueue）

![hihope_illustration](../../Image/hihope_illustration.png)

[润和星闪派物联网开发套件](https://item.taobao.com/item.htm?abbucket=16&id=816685710481&ns=1&priceTId=214783b117346662457694855ed644&skuId=5533042544092&spm=a21n57.sem.item.49.46a639031zWytE&utparam=%7B%22aplus_abtest%22%3A%22b28048df8f009463834be6bdac2a3713%22%7D&xxc=taobaoSearch) 基于海思WS63E解决方案的一套软硬件组合的综合性开发套件。

![wifi_iot](../../Image/HH-K01.png)

## 一、MessageQueue API

| API名称                   | 说明                                                         |
| ------------------------- | ------------------------------------------------------------ |
| osMessageQueueNew         | 创建和初始化一个消息队列                                     |
| osMessageQueueGetName     | 返回指定的消息队列的名字                                     |
| osMessageQueuePut         | 向指定的消息队列存放1条消息，如果消息队列满了，那么返回超时  |
| osMessageQueueGet         | 从指定的消息队列中取得1条消息，如果消息队列为空，那么返回超时 |
| osMessageQueueGetCapacity | 获得指定的消息队列的消息容量                                 |
| osMessageQueueGetMsgSize  | 获得指定的消息队列中可以存放的最大消息的大小                 |
| osMessageQueueGetCount    | 获得指定的消息队列中当前的消息数                             |
| osMessageQueueGetSpace    | 获得指定的消息队列中还可以存放的消息数                       |
| osMessageQueueReset       | 将指定的消息队列重置为初始状态                               |
| osMessageQueueDelete      | 删除指定的消息队列                                           |

## 二、代码分析

`osMessageQueueNew`创建一个消息队列

发送者每次将自己`count`的值与线程ID发送，并将`count`加1；

接受者从消息队列中获取一条信息，然后将其打印输出

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

主程序创建了三个消息发送者和两个消息接收者，然后调用相关的API确认消息队列的装填

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



## 三、如何编译

1. 将05_message目录复制到openharmony源码的`applications\sample\wifi-iot\app`目录下，
2. 修改openharmony源码的`applications\sample\wifi-iot\app\BUILD.gn`文件，将其中的 `features` 改为：

```
    features = [
        ...
        "05_message:message_demo",
        ...
    ]
```
3. 在`device\soc\hisilicon\ws63v100\sdk\build\config\target_config\ws63\config.py`文件中，找到`'ws63-liteos-app'`部分，在其`'ram_component'`中，添加以下代码：
```
"message_demo"
```

4. 在`device\soc\hisilicon\ws63v100\sdk\libs_url\ws63\cmake\ohos.cmake`文件中，找到`"ws63-liteos-app"`部分，在其`set(COMPONENT_LIST`部分，添加以下代码：
```
"message_demo"
```
5. 在openharmony sdk根目录目录执行：`rm -rf out && hb set -p nearlink_dk_3863 && hb build -f`

## 四、运行结果

截取部分运行结果

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

### 【套件支持】

##### 1. 套件购买  https://item.taobao.com/item.htm?abbucket=16&id=816685710481&ns=1&priceTId=214783b117346662457694855ed644&skuId=5533042544092&spm=a21n57.sem.item.49.46a639031zWytE&utparam=%7B%22aplus_abtest%22%3A%22b28048df8f009463834be6bdac2a3713%22%7D&xxc=taobaoSearch

##### 2. 技术资料

- Gitee码云网站（使用说明书、规格说明书、OpenHarmony开发案例等） **https://gitee.com/hihopeorg_group/near-link**
- fbb_ws63代码仓（SDK包、技术文档下载）**https://gitee.com/HiSpark/fbb_ws63**

##### 3. 互动交流
- 海思社区星闪专区-论坛 **https://developer.hisilicon.com/forum/0133146886267870001**
