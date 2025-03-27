# 润和星闪派物联网开发套件--线程（Thread）

![hihope_illustration](../../Image/hihope_illustration.png)

[润和星闪派物联网开发套件](https://item.taobao.com/item.htm?abbucket=16&id=816685710481&ns=1&priceTId=214783b117346662457694855ed644&skuId=5533042544092&spm=a21n57.sem.item.49.46a639031zWytE&utparam=%7B%22aplus_abtest%22%3A%22b28048df8f009463834be6bdac2a3713%22%7D&xxc=taobaoSearch) 基于海思WS63E解决方案的一套软硬件组合的综合性开发套件。

![wifi_iot](../../Image/HH-K01.png)

## 一、Thread API

| API名称               | 说明                                                   |
| --------------------- | ------------------------------------------------------ |
| osThreadNew           | 创建一个线程并将其加入活跃线程组中                     |
| osThreadGetName       | 返回指定线程的名字                                     |
| osThreadGetId         | 返回当前运行线程的线程ID                               |
| osThreadGetState      | 返回当前线程的状态                                     |
| osThreadSetPriority   | 设置指定线程的优先级                                   |
| osThreadGetPriority   | 获取当前线程的优先级                                   |
| osThreadYield         | 将运行控制转交给下一个处于READY状态的线程              |
| osThreadSuspend       | 挂起指定线程的运行                                     |
| osThreadResume        | 恢复指定线程的运行                                     |
| osThreadDetach        | 分离指定的线程（当线程终止运行时，线程存储可以被回收） |
| osThreadJoin          | 等待指定线程终止运行                                   |
| osThreadExit          | 终止当前线程的运行                                     |
| osThreadTerminate     | 终止指定线程的运行                                     |
| osThreadGetStackSize  | 获取指定线程的栈空间大小                               |
| osThreadGetStackSpace | 获取指定线程的未使用的栈空间大小                       |
| osThreadGetCount      | 获取活跃线程数                                         |
| osThreadEnumerate     | 获取线程组中的活跃线程数                               |

### osThreadNew()

```
osThreadId_t osThreadNew(osThreadFunc_t	func, void *argument,const osThreadAttr_t *attr )	
```

> **注意** :不能在中断服务调用该函数

**参数：**

| 名字     | 描述                             |
| :------- | :------------------------------- |
| func     | 线程函数.                        |
| argument | 作为启动参数传递给线程函数的指针 |
| attr     | 线程属性                         |

### osThreadTerminate()

```
osStatus_t osThreadTerminate (osThreadId_t thread_id)
```

| 名字      | 描述                                                 |
| --------- | ---------------------------------------------------- |
| thread_id | 指定线程id，该id是由osThreadNew或者osThreadGetId获得 |

## 二、代码分析

创建线程，创建成功则打印线程名字和线程ID

```
osThreadId_t newThread(char *name, osThreadFunc_t func, void *arg) {
    osThreadAttr_t attr = {
        name, 0, NULL, 0, NULL, 1024*2, osPriorityNormal, 0, 0
    };
    osThreadId_t tid = osThreadNew(func, arg, &attr);
    if (tid == NULL) {
        printf("osThreadNew(%s) failed.\r\n", name);
    } else {
        printf("osThreadNew(%s) success, thread id: %d.\r\n", name, tid);
    }
    return tid;
}
```

该函数首先会打印自己的参数，然后对全局变量count进行循环+1操作，之后会打印count的值

```
void threadTest(void *arg) {
    static int count = 0;
    printf("%s\r\n",(char *)arg);
    osThreadId_t tid = osThreadGetId();
    printf("threadTest osThreadGetId, thread id:%p\r\n", tid);
    while (1) {
        count++;
        printf("threadTest, count: %d.\r\n", count);
        osDelay(20);
    }
}
```

主程序rtosv2_thread_main创建线程并运行，并使用上述API进行相关操作，最后终止所创建的线程。

```
void rtosv2_thread_main(void *arg) {
    (void)arg;
    osThreadId_t tid=newThread("test_thread", threadTest, "This is a test thread.");

    const char *t_name = osThreadGetName(tid);
    printf("[Thread Test]osThreadGetName, thread name: %s.\r\n", t_name);

    osThreadState_t state = osThreadGetState(tid);
    printf("[Thread Test]osThreadGetState, state :%d.\r\n", state);

    osStatus_t status = osThreadSetPriority(tid, osPriorityNormal4);
    printf("[Thread Test]osThreadSetPriority, status: %d.\r\n", status);

    osPriority_t pri = osThreadGetPriority (tid);   
    printf("[Thread Test]osThreadGetPriority, priority: %d.\r\n", pri);

    status = osThreadSuspend(tid);
    printf("[Thread Test]osThreadSuspend, status: %d.\r\n", status);  

    status = osThreadResume(tid);
    printf("[Thread Test]osThreadResume, status: %d.\r\n", status);

    uint32_t stacksize = osThreadGetStackSize(tid);
    printf("[Thread Test]osThreadGetStackSize, stacksize: %d.\r\n", stacksize);

    uint32_t stackspace = osThreadGetStackSpace(tid);
    printf("[Thread Test]osThreadGetStackSpace, stackspace: %d.\r\n", stackspace);

    uint32_t t_count = osThreadGetCount();
    printf("[Thread Test]osThreadGetCount, count: %d.\r\n", t_count);  

    osDelay(100);
    status = osThreadTerminate(tid);
    printf("[Thread Test]osThreadTerminate, status: %d.\r\n", status);
}
```

## 三、如何编译

1. 将00_thread目录复制到openharmony源码的`applications\sample\wifi-iot\app`目录下，
2. 修改openharmony源码的`applications\sample\wifi-iot\app\BUILD.gn`文件，将其中的 `features` 改为：

```
    features = [
         ...
        "00_thread:thread_demo",
         ...
    ]
```
3. 在`device\soc\hisilicon\ws63v100\sdk\build\config\target_config\ws63\config.py`文件中，找到`'ws63-liteos-app'`部分，在其`'ram_component'`中，添加以下代码：
```
"thread_demo"
```

4. 在`device\soc\hisilicon\ws63v100\sdk\libs_url\ws63\cmake\ohos.cmake`文件中，找到`"ws63-liteos-app"`部分，在其`set(COMPONENT_LIST`部分，添加以下代码：
```
"thread_demo"
```
5. 在openharmony sdk根目录目录执行：`rm -rf out && hb set -p nearlink_dk_3863 && hb build -f`

## 四、运行结果

设置串口工具波特率为115200，复位开发板，查看打印结果

```
[Thread Test] osThreadNew(test_thread) success.
[Thread Test] osThreadGetName, thread name: test_thread.
[Thread Test] osThreadGetState, state :1.
[Thread Test] This is a test thread.  <-testThread log
[Thread Test] threadTest osThreadGetId, thread id:0xe8544
[Thread Test] threadTest, count: 1.   <-testThread log
[Thread Test] osThreadSetPriority, status: 0.
[Thread Test] osThreadGetPriority, priority: 28.
[Thread Test] osThreadSuspend, status: 0.
[Thread Test] osThreadResume, status: 0.
[Thread Test] osThreadGetStackSize, stacksize: 2048.
[Thread Test] osThreadGetStackSpace, stackspace: 1144.
[Thread Test] osThreadGetCount, count: 12.
[Thread Test] threadTest, count: 2.   <-testThread log
[Thread Test] threadTest, count: 3.   <-testThread log
[Thread Test] threadTest, count: 4.   <-testThread log
[Thread Test] threadTest, count: 5.   <-testThread log
[Thread Test] threadTest, count: 6.   <-testThread log
[Thread Test] osThreadTerminate, status: 0.
```

### 【套件支持】

##### 1. 套件购买  https://item.taobao.com/item.htm?abbucket=16&id=816685710481&ns=1&priceTId=214783b117346662457694855ed644&skuId=5533042544092&spm=a21n57.sem.item.49.46a639031zWytE&utparam=%7B%22aplus_abtest%22%3A%22b28048df8f009463834be6bdac2a3713%22%7D&xxc=taobaoSearch

##### 2. 技术资料

- Gitee码云网站（使用说明书、规格说明书、OpenHarmony开发案例等） **https://gitee.com/hihopeorg_group/near-link**
- fbb_ws63代码仓（SDK包、技术文档下载）**https://gitee.com/HiSpark/fbb_ws63**

##### 3. 互动交流
- 海思社区星闪专区-论坛 **https://developer.hisilicon.com/forum/0133146886267870001**

