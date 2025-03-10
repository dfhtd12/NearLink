# 润和星闪派物联网开发套件--信号量（Semaphore）

![hihope_illustration](https://gitee.com/hihopeorg/hispark-hm-pegasus/raw/master/docs/figures/hihope_illustration.png)

[润和星闪派物联网开发套件](https://item.taobao.com/item.htm?abbucket=16&id=816685710481&ns=1&priceTId=214783b117346662457694855ed644&skuId=5533042544092&spm=a21n57.sem.item.49.46a639031zWytE&utparam=%7B%22aplus_abtest%22%3A%22b28048df8f009463834be6bdac2a3713%22%7D&xxc=taobaoSearch) 基于海思WS63E解决方案的一套软硬件组合的综合性开发套件。

![wifi_iot](https://img.alicdn.com/imgextra/i4/3583112207/O1CN01SvRG981SAr7bdEg3i_!!3583112207.png)

## 一、Semaphore API

| API名称             | 说明                                                         |
| ------------------- | ------------------------------------------------------------ |
| osSemaphoreNew      | 创建并初始化一个信号量                                       |
| osSemaphoreGetName  | 获取一个信号量的名字                                         |
| osSemaphoreAcquire  | 获取一个信号量的令牌，若获取不到，则会超时返回               |
| osSemaphoreRelease  | 释放一个信号量的令牌，但是令牌的数量不超过初始定义的的令牌数 |
| osSemaphoreGetCount | 获取当前的信号量令牌数                                       |
| osSemaphoreDelete   | 删除一个信号量                                               |

## 二、代码分析

`osSemaphoreAcquire`获取共享资源的访问权限，若获取失败，则等待；访问成功后，可以通过`osSemaphoreRelease`释放对共享资源的访问

本样例为经典的消费者与生产者问题，需要确保仓库满时，生产者需要进入等待状态，产品消费完时，消费者需要进入等待状态

```c
void producer_thread(void *arg) {
    (void)arg;
    empty_id = osSemaphoreNew(BUFFER_SIZE, BUFFER_SIZE, NULL);
    filled_id = osSemaphoreNew(BUFFER_SIZE, 0U, NULL);
    while(1) {
        osSemaphoreAcquire(empty_id, osWaitForever);
        product_number++;
        printf("[Semp Test]%s produces a product, now product number: %d.\r\n", osThreadGetName(osThreadGetId()), product_number);
        osDelay(4);
        osSemaphoreRelease(filled_id);
    }
}

void consumer_thread(void *arg) {
    (void)arg;
    while(1){
        osSemaphoreAcquire(filled_id, osWaitForever);
        product_number--;
        printf("[Semp Test]%s consumes a product, now product number: %d.\r\n", osThreadGetName(osThreadGetId()), product_number);
        osDelay(3);
        osSemaphoreRelease(empty_id);
    }
}
```

由于消费产品的速度大于生产速度，所以定义了三个生产者，两个消费者

```c
void rtosv2_semp_main(void *arg) {
    (void)arg;
    empty_id = osSemaphoreNew(BUFFER_SIZE, BUFFER_SIZE, NULL);
    filled_id = osSemaphoreNew(BUFFER_SIZE, 0U, NULL);
 
    osThreadId_t ptid1 = newThread("producer1", producer_thread, NULL);
    osThreadId_t ptid2 = newThread("producer2", producer_thread, NULL);
    osThreadId_t ptid3 = newThread("producer3", producer_thread, NULL);
    osThreadId_t ctid1 = newThread("consumer1", consumer_thread, NULL);
    osThreadId_t ctid2 = newThread("consumer2", consumer_thread, NULL);

    osDelay(50);

    osThreadTerminate(ptid1);
    osThreadTerminate(ptid2);
    osThreadTerminate(ptid3);
    osThreadTerminate(ctid1);
    osThreadTerminate(ctid2);

    osSemaphoreDelete(empty_id);
    osSemaphoreDelete(filled_id);
}
```



## 三、如何编译

1. 将00_thread目录复制到openharmony源码的`applications\sample\wifi-iot\app`目录下，
2. 修改openharmony源码的`applications\sample\wifi-iot\app\BUILD.gn`文件，将其中的 `features` 改为：

```
    features = [
        ...
        "04_semaphore:semp_demo",
        ...
    ]
```
3. 在`device\soc\hisilicon\ws63v100\sdk\build\config\target_config\ws63\config.py`文件中，找到`'ws63-liteos-app'`部分，在其`'ram_component'`中，添加以下代码：
```
"semp_demo"
```

4. 在`device\soc\hisilicon\ws63v100\sdk\libs_url\ws63\cmake\ohos.cmake`文件中，找到`"ws63-liteos-app"`部分，在其`set(COMPONENT_LIST`部分，添加以下代码：
```
"semp_demo"
```
5. 在openharmony sdk根目录目录执行：`rm -rf out && hb set -p nearlink_dk_3863 && hb build -f`

## 四、运行结果

截取部分运行结果

```
[Semp Test]osThreadNew(producer1) success, thread id: 10580248.
[Semp Test]osThreadNew(producer2) success, thread id: 10580356.
[Semp Test]osThreadNew(producer3) success, thread id: 10580464.
[Semp Test]producer1 produces a product, now product number: 1.
[Semp Test]producer2 produces a product, now product number: 2.
[Semp Test]producer3 produces a product, now product number: 3.
[Semp Test]osThreadNew(consumer1) success, thread id: 10580572.
[Semp Test]osThreadNew(consumer2) success, thread id: 10580680.
```

### 【套件支持】

##### 1. 套件购买  https://item.taobao.com/item.htm?abbucket=16&id=816685710481&ns=1&priceTId=214783b117346662457694855ed644&skuId=5533042544092&spm=a21n57.sem.item.49.46a639031zWytE&utparam=%7B%22aplus_abtest%22%3A%22b28048df8f009463834be6bdac2a3713%22%7D&xxc=taobaoSearch

##### 2. 技术资料

- Gitee码云网站（使用说明书、规格说明书、OpenHarmony开发案例等） **https://gitee.com/hihopeorg_group/near-link**
- fbb_ws63代码仓（SDK包、技术文档下载）**https://gitee.com/HiSpark/fbb_ws63**

##### 3. 互动交流
- 海思社区星闪专区-论坛 **https://developer.hisilicon.com/forum/0133146886267870001**

