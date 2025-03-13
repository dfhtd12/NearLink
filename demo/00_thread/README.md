# HopeRun IoT Development Kit--Thread

![hihope_illustration](https://github.com/HopeRunORG/NearLink/tree/main/Image/hihope_illustration.png)

An integrated software and hardware development kit based on WS63E solution, providing a comprehensive suite for embedded system development.

![wifi_iot](https://github.com/HopeRunORG/NearLink/tree/main/Image/O1CN01SvRG981SAr7bdEg3i_!!3583112207.png)

## 1. Thread API

| API                   | **Description**                                              |
| --------------------- | ------------------------------------------------------------ |
| osThreadNew           | Create a thread and add it to the active thread group        |
| osThreadGetName       | Return the name of the specified thread                      |
| osThreadGetId         | Return the thread ID of the currently running thread         |
| osThreadGetState      | Return the current state of the thread                       |
| osThreadSetPriority   | Set the priority of the specified thread                     |
| osThreadGetPriority   | Get the priority of the current thread                       |
| osThreadYield         | Yield control to the next ready thread                       |
| osThreadSuspend       | Suspend the specified thread                                 |
| osThreadResume        | Resume the specified thread                                  |
| osThreadDetach        | Detach the specified thread (releases memory upon termination) |
| osThreadJoin          | Wait for the specified thread to terminate                   |
| osThreadExit          | Terminate the current thread                                 |
| osThreadTerminate     | Terminate the specified thread                               |
| osThreadGetStackSize  | Get the stack size of the specified thread                   |
| osThreadGetStackSpace | Get the unused stack space of the specified thread           |
| osThreadGetCount      | Get the number of active threads                             |
| osThreadEnumerate     | Get the number of active threads in the thread group         |

### osThreadNew()

```
osThreadId_t osThreadNew(osThreadFunc_t	func, void *argument,const osThreadAttr_t *attr )	
```

> ***\*Note:\** This function must not be called from an interrupt service routine (ISR).**

**Parameter：**

| Name     | **Description**             |
| :------- | :-------------------------- |
| func     | Thread Function             |
| argument | Configure thread attributes |
| attr     | Thread Attributes           |

### osThreadTerminate()

```
osStatus_t osThreadTerminate (osThreadId_t thread_id)
```

| Name      | **Description**                                              |
| --------- | ------------------------------------------------------------ |
| thread_id | Specify the thread ID obtained via `osThreadCreate` or `osThreadIdGet` |

## 2. Code

Create a thread. If the creation succeeds, print the thread name and ID.

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

The function first prints its parameters, then performs an incremental operation on the global variable `count`, followed by printing the updated value of `count`.

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

The main program `rtosv2_thread_main` creates and runs the thread, uses the aforementioned APIs for operations, and finally terminates the created thread.

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

## 3. Compile

1. Copy the `00_thread` directory to the `applications\sample\wifi-iot\app` directory within the Oniro source code.
2. Modify the `BUILD.gn` in the `applications\sample\wifi-iot\app` directory of the Oniro source code by replacing the `features` variable with:

```
    features = [
         ...
        "00_thread:thread_demo",
         ...
    ]
```
3. In the file `config.py` located at `device\soc\hisilicon\ws63v100\sdk\build\config\target_config\ws63`, locate the section labeled `'ws63-liteos-app'`. Within this section, add the following code to the `'ram_component'` field:
```
"thread_demo"
```

4. In the file `ohos.cmake` located at `device\soc\hisilicon\ws63v100\sdk\libs_url\ws63\cmake`, locate the section labeled `"ws63-liteos-app"`. Within this section, find the `set(COMPONENT_LIST` statement and append the following code to its argument list:
```
"thread_demo"
```
5. Execute the following command in the root directory of the Oniro SDK：`rm -rf out && hb set -p nearlink_dk_3863 && hb build -f`

## 4. Result

Set the serial port tool's baud rate to 115200, reset the development board, and observe the printed output.

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

### 【Dev-kits】

##### 1. Online marketplaces  https://item.taobao.com/item.htm?abbucket=16&id=816685710481&ns=1&priceTId=214783b117346662457694855ed644&skuId=5533042544092&spm=a21n57.sem.item.49.46a639031zWytE&utparam=%7B%22aplus_abtest%22%3A%22b28048df8f009463834be6bdac2a3713%22%7D&xxc=taobaoSearch

##### 2. **Technical Documentation**

- **Gitee CodeCloud** (User Manuals, Specifications, Oniro Development Cases) **https://gitee.com/hihopeorg_group/near-link**
- **fbb_ws63 Repository** (SDK Packages, Technical Documentation Downloads)**https://gitee.com/HiSpark/fbb_ws63**

##### 3. **Interaction and Support**

- **Hisilicon Community - NearLink Zone Forum** **https://developer.hisilicon.com/forum/0133146886267870001**

