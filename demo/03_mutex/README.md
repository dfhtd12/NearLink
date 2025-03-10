# 润和星闪派物联网开发套件--互斥锁（Mutex）

![hihope_illustration](https://gitee.com/hihopeorg/hispark-hm-pegasus/raw/master/docs/figures/hihope_illustration.png)

[润和星闪派物联网开发套件](https://item.taobao.com/item.htm?abbucket=16&id=816685710481&ns=1&priceTId=214783b117346662457694855ed644&skuId=5533042544092&spm=a21n57.sem.item.49.46a639031zWytE&utparam=%7B%22aplus_abtest%22%3A%22b28048df8f009463834be6bdac2a3713%22%7D&xxc=taobaoSearch) 基于海思WS63E解决方案的一套软硬件组合的综合性开发套件。

![wifi_iot](https://img.alicdn.com/imgextra/i4/3583112207/O1CN01SvRG981SAr7bdEg3i_!!3583112207.png)

## 一、Mutex API

| API名称         | 说明                                                     |
| --------------- | -------------------------------------------------------- |
| osMutexNew      | 创建并初始化一个互斥锁                                   |
| osMutexGetName  | 获得指定互斥锁的名字                                     |
| osMutexAcquire  | 获得指定的互斥锁的访问权限，若互斥锁已经被锁，则返回超时 |
| osMutexRelease  | 释放指定的互斥锁                                         |
| osMutexGetOwner | 获得指定互斥锁的所有者线程                               |
| osMutexDelete   | 删除指定的互斥锁                                         |

## 二、代码分析

全局变量`g_test_value`若同时被多个线程访问，会将其加1，然后判断其奇偶性，并输出日志，如果没有互斥锁保护，线程会被中断导致错误，所以需要创建互斥锁来保护多线程共享区域

```c
void number_thread(void *arg) {
    osMutexId_t *mid = (osMutexId_t *)arg;
    while(1) {
        if (osMutexAcquire(*mid, 100) == osOK) {
            g_test_value++;
            if (g_test_value % 2 == 0) {
                printf("[Mutex Test] %s gets an even value %d.\r\n", osThreadGetName(osThreadGetId()), g_test_value);
            } else {
                printf("[Mutex Test] %s gets an odd value %d.\r\n",  osThreadGetName(osThreadGetId()), g_test_value);
            }
            osMutexRelease(*mid);
            osDelay(5);
        }
    }
}
```

创建三个线程访问全局变量`g_test_value` ，同时创建一个互斥锁共所有线程使用

```c
void rtosv2_mutex_main(void *arg) {
    (void)arg;
    osMutexAttr_t attr = {0};

    osMutexId_t mid = osMutexNew(&attr);
    if (mid == NULL) {
        printf("[Mutex Test] osMutexNew, create mutex failed.\r\n");
    } else {
        printf("[Mutex Test] osMutexNew, create mutex success.\r\n");
    }

    osThreadId_t tid1 = newThread("Thread_1", number_thread, &mid);
    osThreadId_t tid2 = newThread("Thread_2", number_thread, &mid);
    osThreadId_t tid3 = newThread("Thread_3", number_thread, &mid);

    osDelay(13);
    osThreadId_t tid = osMutexGetOwner(mid);
    printf("[Mutex Test] osMutexGetOwner, thread id: %p, thread name: %s.\r\n", tid, osThreadGetName(tid));
    osDelay(17);

    osThreadTerminate(tid1);
    osThreadTerminate(tid2);
    osThreadTerminate(tid3);
    osMutexDelete(mid);
}
```



## 三、如何编译

1. 将00_thread目录复制到openharmony源码的`applications\sample\wifi-iot\app`目录下，
2. 修改openharmony源码的`applications\sample\wifi-iot\app\BUILD.gn`文件，将其中的 `features` 改为：

```
    features = [
        ...
        "03_mutex:mutex_demo",
        ...
    ]
```
3. 在`device\soc\hisilicon\ws63v100\sdk\build\config\target_config\ws63\config.py`文件中，找到`'ws63-liteos-app'`部分，在其`'ram_component'`中，添加以下代码：
```
"mutex_demo"
```

4. 在`device\soc\hisilicon\ws63v100\sdk\libs_url\ws63\cmake\ohos.cmake`文件中，找到`"ws63-liteos-app"`部分，在其`set(COMPONENT_LIST`部分，添加以下代码：
```
"mutex_demo"
```
5. 在openharmony sdk根目录目录执行：`rm -rf out && hb set -p nearlink_dk_3863 && hb build -f`

## 四、运行结果

设置串口工具波特率为115200，复位开发板，查看打印结果

```
[Mutex Test]Thread_1 gets an even value 4.
[Mutex Test]Thread_2 gets an odd value 5.
[Mutex Test]Thread_3 gets an even value 6.

[17:45:54.066]收←◆[Mutex Test]Thread_1 gets an odd value 7.
[Mutex Test]Thread_2 gets an even value 8.
[Mutex Test]Thread_3 gets an odd value 9.
[Mutex Test]osMutexGetOwner, thread id: (nil), thread name: (null).

[17:45:54.115]收←◆[Mutex Test]Thread_1 gets an even value 10.
[Mutex Test]Thread_2 gets an odd value 11.
[Mutex Test]Thread_3 gets an even value 12.

[17:45:54.165]收←◆[Mutex Test]Thread_1 gets an odd value 13.
[Mutex Test]Thread_2 gets an even value 14.
[Mutex Test]Thread_3 gets an odd value 15.

[17:45:54.215]收←◆[Mutex Test]Thread_1 gets an even value 16.
[Mutex Test]Thread_2 gets an odd value 17.
[Mutex Test]Thread_3 gets an even value 18.

```

### 【套件支持】

##### 1. 套件购买  https://item.taobao.com/item.htm?abbucket=16&id=816685710481&ns=1&priceTId=214783b117346662457694855ed644&skuId=5533042544092&spm=a21n57.sem.item.49.46a639031zWytE&utparam=%7B%22aplus_abtest%22%3A%22b28048df8f009463834be6bdac2a3713%22%7D&xxc=taobaoSearch

##### 2. 技术资料

- Gitee码云网站（使用说明书、规格说明书、OpenHarmony开发案例等） **https://gitee.com/hihopeorg_group/near-link**
- fbb_ws63代码仓（SDK包、技术文档下载）**https://gitee.com/HiSpark/fbb_ws63**

##### 3. 互动交流
- 海思社区星闪专区-论坛 **https://developer.hisilicon.com/forum/0133146886267870001**

