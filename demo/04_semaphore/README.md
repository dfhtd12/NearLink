# HopeRun IoT Development Kit--Semaphore

![hihope_illustration](../../Image/hihope_illustration.png)

An integrated software and hardware development kit based on WS63E solution, providing a comprehensive suite for embedded system development.

![wifi_iot](../../Image/HH-K01.png)

## 1. Semaphore API

| API                 | **Description**                                              |
| ------------------- | ------------------------------------------------------------ |
| osSemaphoreNew      | Create and initialize a semaphore                            |
| osSemaphoreGetName  | Get the name of the semaphore                                |
| osSemaphoreAcquire  | Acquire a token from the semaphore; if a token cannot be acquired, it will return after a timeout |
| osSemaphoreRelease  | Release a token to the semaphore, but the number of tokens should not exceed the initial defined number of tokens |
| osSemaphoreGetCount | Get the current number of tokens in the semaphore            |
| osSemaphoreDelete   | Delete the semaphore                                         |

## 2. Code

`osSemaphoreAcquire` is used to acquire access to the shared resource; if the acquisition fails, it waits. After successful access, `osSemaphoreRelease` can be used to release the access to the shared resource.

This example illustrates the classic producer-consumer problem, where it is necessary to ensure that when the warehouse is full, the producers enter a waiting state, and when the products are consumed, the consumers enter a waiting state.

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

Since the consumption rate is higher than the production rate, three producers and two consumers are defined.

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



## 3. **Compile**

1.  the `04_semaphore`applications\sample\wifi-iot\app` directory within the Oniro source code.
2. Modify the `BUILD.gn` in the `applications\sample\wifi-iot\app` directory of the Oniro source code by replacing the `features` variable with:

```
    features = [
        ...
        "04_semaphore:semp_demo",
        ...
    ]
```
3. In the file `config.py` located at `device\soc\hisilicon\ws63v100\sdk\build\config\target_config\ws63`, locate the section labeled `'ws63-liteos-app'`. Within this section, add the following code to the `'ram_component'` field:
```
"semp_demo"
```

4. In the file `ohos.cmake` located at `device\soc\hisilicon\ws63v100\sdk\libs_url\ws63\cmake`, locate the section labeled `"ws63-liteos-app"`. Within this section, find the `set(COMPONENT_LIST` statement and append the following code to its argument list:
```
"semp_demo"
```
5. Execute the following command in the root directory of the Oniro SDK: `rm -rf out && hb set -p nearlink_dk_3863 && hb build -f`

## 4. Result

**Extract part of the runtime results**

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

### 【Dev-kits】

##### 1. Online marketplaces  https://item.taobao.com/item.htm?abbucket=16&id=816685710481&ns=1&priceTId=214783b117346662457694855ed644&skuId=5533042544092&spm=a21n57.sem.item.49.46a639031zWytE&utparam=%7B%22aplus_abtest%22%3A%22b28048df8f009463834be6bdac2a3713%22%7D&xxc=taobaoSearch

##### 2. **Technical Documentation**

- **Gitee CodeCloud** (User Manuals, Specifications, Oniro Development Cases) **https://gitee.com/hihopeorg_group/near-link**
- **fbb_ws63 Repository** (SDK Packages, Technical Documentation Downloads)**https://gitee.com/HiSpark/fbb_ws63**

##### 3. **Interaction and Support**

- **Hisilicon Community - NearLink Zone Forum** **https://developer.hisilicon.com/forum/0133146886267870001**

