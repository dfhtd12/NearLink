# HopeRun IoT Development Kit--Timer

![hihope_illustration](../../Image/hihope_illustration.png)

An integrated software and hardware development kit based on WS63E solution, providing a comprehensive suite for embedded system development.

![wifi_iot](../../Image/HH-K01.png)

## 1. Timer API

| API              | **Description**                      |
| ---------------- | ------------------------------------ |
| osTimerNew       | Create and initialize a timer        |
| osTimerGetName   | Get the specified timer identifier   |
| osTimerStart     | Start or restart the specified timer |
| osTimerStop      | Stop the specified timer             |
| osTimerIsRunning | Check if a timer is active           |
| osTimerDelete    | Delete the timer                     |

### osTimerNew()

```c
osTimerId_t osTimerNew (osTimerFunc_t func, osTimerType_t type, void *argument, const osTimerAttr_t *attr)
```

**Parameter：**

| Name     | **Description**                                              |
| :------- | :----------------------------------------------------------- |
| func     | Timer callback function                                      |
| type     | Timer type，osTimerOnce: Single-execution timer，ostimer: Periodic timer |
| argument | Parameters of the callback function                          |
| attr     | Timer properties                                             |

## 2. Code

Callback Function of Timer 

```c
void cb_timeout_periodic(void *arg) {
    (void)arg;
    times++;
}
```

Create a periodic timer using `osTimerNew` that triggers the callback function `cb_timeout_periodic` every 100 clock cycles. On each timer tick, check if the global variable `times` is less than 3. If `times` is no longer less than 3, stop the timer.

```c
void timer_periodic(void) {
    osTimerId_t periodic_tid = osTimerNew(cb_timeout_periodic, osTimerPeriodic, NULL, NULL);
    if (periodic_tid == NULL) {
        printf("[Timer Test] osTimerNew(periodic timer) failed.\r\n");
        return;
    } else {
        printf("[Timer Test] osTimerNew(periodic timer) success, tid: %p.\r\n",periodic_tid);
    }
    osStatus_t status = osTimerStart(periodic_tid, 100);
    if (status != osOK) {
        printf("[Timer Test] osTimerStart(periodic timer) failed.\r\n");
        return;
    } else {
        printf("[Timer Test] osTimerStart(periodic timer) success, wait a while and stop.\r\n");
    }

    while(times < 3) {
        printf("[Timer Test] times:%d.\r\n",times);
        osDelay(100);
    }

    status = osTimerStop(periodic_tid);
    printf("[Timer Test] stop periodic timer, status :%d.\r\n", status);
    status = osTimerDelete(periodic_tid);
    printf("[Timer Test] kill periodic timer, status :%d.\r\n", status);
}

```



## 3. **Compile**

1. Copy the `01_timer` directory to the `applications\sample\wifi-iot\app` directory within the Oniro source code.
2. Modify the `BUILD.gn` in the `applications\sample\wifi-iot\app` directory of the Oniro source code by replacing the `features` variable with:

```
    features = [
        ...
        "01_timer:timer_demo",
        ...
    ]
```
3. In the file `config.py` located at `device\soc\hisilicon\ws63v100\sdk\build\config\target_config\ws63`, locate the section labeled `'ws63-liteos-app'`. Within this section, add the following code to the `'ram_component'` field:
```
"timer_demo"
```

4. In the file `ohos.cmake` located at `device\soc\hisilicon\ws63v100\sdk\libs_url\ws63\cmake`, locate the section labeled `"ws63-liteos-app"`. Within this section, find the `set(COMPONENT_LIST` statement and append the following code to its argument list:
```
"timer_demo"
```
5. Execute the following command in the root directory of the Oniro SDK: `rm -rf out && hb set -p nearlink_dk_3863 && hb build -f`

## 4. Result

Set the serial port tool's baud rate to 115200, reset the development board, and observe the printed output.

```
[Timer Test] osTimerNew(periodic timer) success, tid: 0xe9b4c.
[Timer Test] osTimerStart(periodic timer) success, wait a while and stop.
[Timer Test] times:0.
[Timer Test] times:1.
[Timer Test] times:2.
[Timer Test] stop periodic timer, status :0.
[Timer Test] kill periodic timer, status :0.
```

### 【Dev-kits】

##### 1. Online marketplaces  https://item.taobao.com/item.htm?abbucket=16&id=816685710481&ns=1&priceTId=214783b117346662457694855ed644&skuId=5533042544092&spm=a21n57.sem.item.49.46a639031zWytE&utparam=%7B%22aplus_abtest%22%3A%22b28048df8f009463834be6bdac2a3713%22%7D&xxc=taobaoSearch

##### 2. **Technical Documentation**

- **Gitee CodeCloud** (User Manuals, Specifications, Oniro Development Cases) **https://gitee.com/hihopeorg_group/near-link**
- **fbb_ws63 Repository** (SDK Packages, Technical Documentation Downloads)**https://gitee.com/HiSpark/fbb_ws63**

##### 3. **Interaction and Support**
- **Hisilicon Community - NearLink Zone Forum** **https://developer.hisilicon.com/forum/0133146886267870001**
