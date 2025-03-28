# 润和星闪派物联网开发套件--延时（Delay）

![hihope_illustration](../../Image/hihope_illustration.png)

[润和星闪派物联网开发套件](https://item.taobao.com/item.htm?abbucket=16&id=816685710481&ns=1&priceTId=214783b117346662457694855ed644&skuId=5533042544092&spm=a21n57.sem.item.49.46a639031zWytE&utparam=%7B%22aplus_abtest%22%3A%22b28048df8f009463834be6bdac2a3713%22%7D&xxc=taobaoSearch) 基于海思WS63E解决方案的一套软硬件组合的综合性开发套件。

![wifi_iot](../../Image/HH-K01.png)

## 一、Delay API

| API名称      | 说明                 |
| ------------ | -------------------- |
| osDelay      | 等待指定的ticks      |
| osDelayUntil | 等待到指定的时钟周期 |

## 二、代码分析

使用`osDelay`让线程等待100个时钟周期，然后通过`osKernelGetTickCount`获取当前的时钟周期tick, 在此基础上增加100个时钟周期，之后调用`osDelayUntil`让线程等待tick + 100个时钟周期后恢复运行

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



## 三、如何编译

1. 将02_delay目录复制到openharmony源码的`applications\sample\wifi-iot\app`目录下，
2. 修改openharmony源码的`applications\sample\wifi-iot\app\BUILD.gn`文件，将其中的 `features` 改为：

```
    features = [
        ...
        "02_delay:delay_demo",
        ...
    ]
```
3. 在`device\soc\hisilicon\ws63v100\sdk\build\config\target_config\ws63\config.py`文件中，找到`'ws63-liteos-app'`部分，在其`'ram_component'`中，添加以下代码：
```
"delay_demo"
```

4. 在`device\soc\hisilicon\ws63v100\sdk\libs_url\ws63\cmake\ohos.cmake`文件中，找到`"ws63-liteos-app"`部分，在其`set(COMPONENT_LIST`部分，添加以下代码：
```
"delay_demo"
```
5. 在openharmony sdk根目录目录执行：`rm -rf out && hb set -p nearlink_dk_3863 && hb build -f`

## 四、运行结果

设置串口工具波特率为115200，复位开发板，查看打印结果

```
[Delay Test]  Current system tick: 248.
[Delay Test]  osDelay, status: 0.
[Delay Test]  Current system tick: 348.
[Delay Test]  osDelayUntil, status: 0.
[Delay Test]  Current system tick: 448.
```

### 【套件支持】

##### 1. 套件购买  https://item.taobao.com/item.htm?abbucket=16&id=816685710481&ns=1&priceTId=214783b117346662457694855ed644&skuId=5533042544092&spm=a21n57.sem.item.49.46a639031zWytE&utparam=%7B%22aplus_abtest%22%3A%22b28048df8f009463834be6bdac2a3713%22%7D&xxc=taobaoSearch

##### 2. 技术资料

- Gitee码云网站（使用说明书、规格说明书、OpenHarmony开发案例等） **https://gitee.com/hihopeorg_group/near-link**
- fbb_ws63代码仓（SDK包、技术文档下载）**https://gitee.com/HiSpark/fbb_ws63**

##### 3. 互动交流
- 海思社区星闪专区-论坛 **https://developer.hisilicon.com/forum/0133146886267870001**

