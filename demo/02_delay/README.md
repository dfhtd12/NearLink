# HopeRun IoT Development Kit--Delay

![hihope_illustration](../../Image/hihope_illustration.png)

An integrated software and hardware development kit based on WS63E solution, providing a comprehensive suite for embedded system development.

![wifi_iot](../../Image/HH-K01.png)

## 1. Delay API

| API          | **Description**                      |
| ------------ | ------------------------------------ |
| osDelay      | Wait for the specified ticks         |
| osDelayUntil | Wait until the specified clock cycle |

## 2. Code

Use `osDelay` to make the thread wait for 100 clock cycles. Then, use `osKernelGetTickCount` to get the current tick count. Add 100 clock cycles to this value, and then call `osDelayUntil` to make the thread wait until the tick count plus 100 clock cycles before resuming execution.

```
void rtosv2_delay_main(void *arg) {
    (void)arg;

    printf("[Delay Test] Current system tick: %d.\r\n", osKernelGetTickCount());
    osStatus_t status = osDelay(100);
    printf("[Delay Test] osDelay, status: %d.\r\n", status);
    printf("[Delay Test] Current system tick: %d.\r\n", osKernelGetTickCount());

    uint32_t tick = osKernelGetTickCount();
    tick += 100;
    status = osDelayUntil(tick);
    printf("[Delay Test] osDelayUntil, status: %d.\r\n", status);
    printf("[Delay Test] Current system tick: %d.\r\n", osKernelGetTickCount());
}
```



## 3. **Compile**

1. the `02_delayCopy` directory to the `applications\sample\wifi-iot\app` directory within the Oniro source code.
2. Modify the `BUILD.gn` in the `applications\sample\wifi-iot\app` directory of the Oniro source code by replacing the `features` variable with:

```
    features = [
        ...
        "02_delay:delay_demo",
        ...
    ]
```
3. In the file `config.py` located at `device\soc\hisilicon\ws63v100\sdk\build\config\target_config\ws63`, locate the section labeled `'ws63-liteos-app'`. Within this section, add the following code to the `'ram_component'` field:
```
"delay_demo"
```

4. In the file `ohos.cmake` located at `device\soc\hisilicon\ws63v100\sdk\libs_url\ws63\cmake`, locate the section labeled `"ws63-liteos-app"`. Within this section, find the `set(COMPONENT_LIST` statement and append the following code to its argument list:
```
"delay_demo"
```
5. Execute the following command in the root directory of the Oniro SDK: `rm -rf out && hb set -p nearlink_dk_3863 && hb build -f`

## 4. Result

Set the serial port tool's baud rate to 115200, reset the development board, and observe the printed output.

```
[Delay Test]  Current system tick: 248.
[Delay Test]  osDelay, status: 0.
[Delay Test]  Current system tick: 348.
[Delay Test]  osDelayUntil, status: 0.
[Delay Test]  Current system tick: 448.
```

### 【Dev-kits】

##### 1. Online marketplaces  https://item.taobao.com/item.htm?abbucket=16&id=816685710481&ns=1&priceTId=214783b117346662457694855ed644&skuId=5533042544092&spm=a21n57.sem.item.49.46a639031zWytE&utparam=%7B%22aplus_abtest%22%3A%22b28048df8f009463834be6bdac2a3713%22%7D&xxc=taobaoSearch

##### 2. **Technical Documentation**

- **Gitee CodeCloud** (User Manuals, Specifications, Oniro Development Cases) **https://gitee.com/hihopeorg_group/near-link**
- **fbb_ws63 Repository** (SDK Packages, Technical Documentation Downloads)**https://gitee.com/HiSpark/fbb_ws63**

##### 3. **Interaction and Support**

- **Hisilicon Community - NearLink Zone Forum** **https://developer.hisilicon.com/forum/0133146886267870001**

