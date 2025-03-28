# 润和星闪派物联网开发套件--软件定时器（Timer）

![hihope_illustration](../../Image/hihope_illustration.png)

[润和星闪派物联网开发套件](https://item.taobao.com/item.htm?abbucket=16&id=816685710481&ns=1&priceTId=214783b117346662457694855ed644&skuId=5533042544092&spm=a21n57.sem.item.49.46a639031zWytE&utparam=%7B%22aplus_abtest%22%3A%22b28048df8f009463834be6bdac2a3713%22%7D&xxc=taobaoSearch) 基于海思WS63E解决方案的一套软硬件组合的综合性开发套件。

![wifi_iot](../../Image/HH-K01.png)

## 一、Timer API

| API名称          | 说明                     |
| ---------------- | ------------------------ |
| osTimerNew       | 创建和初始化定时器       |
| osTimerGetName   | 获取指定的定时器名字     |
| osTimerStart     | 启动或者重启指定的定时器 |
| osTimerStop      | 停止指定的定时器         |
| osTimerIsRunning | 检查一个定时器是否在运行 |
| osTimerDelete    | 删除定时器               |

### osTimerNew()

```c
osTimerId_t osTimerNew (osTimerFunc_t func, osTimerType_t type, void *argument, const osTimerAttr_t *attr)
```

**参数：**

| 名字     | 描述                                                         |
| :------- | :----------------------------------------------------------- |
| func     | 定时器回调函数.                                              |
| type     | 定时器类型，osTimerOnce表示单次定时器，ostimer周期表示周期性定时器. |
| argument | 定时器回调函数的参数                                         |
| attr     | 定时器属性                                                   |

## 二、代码分析

定时器的回调函数

```c
void cb_timeout_periodic(void *arg) {
    (void)arg;
    times++;
}
```

使用osTimerNew创建一个100个时钟周期调用一次回调函数cb_timeout_periodic定时器，每隔100个时钟周期检查一下全局变量times是否小于3，若不小于3则停止时钟周期

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



## 三、如何编译

1. 将01_timer目录复制到openharmony源码的`applications\sample\wifi-iot\app`目录下，
2. 修改openharmony源码的`applications\sample\wifi-iot\app\BUILD.gn`文件，将其中的 `features` 改为：

```
    features = [
        ...
        "01_timer:timer_demo",
        ...
    ]
```
3. 在`device\soc\hisilicon\ws63v100\sdk\build\config\target_config\ws63\config.py`文件中，找到`'ws63-liteos-app'`部分，在其`'ram_component'`中，添加以下代码：
```
"timer_demo"
```

4. 在`device\soc\hisilicon\ws63v100\sdk\libs_url\ws63\cmake\ohos.cmake`文件中，找到`"ws63-liteos-app"`部分，在其`set(COMPONENT_LIST`部分，添加以下代码：
```
"timer_demo"
```
5. 在openharmony sdk根目录目录执行：`rm -rf out && hb set -p nearlink_dk_3863 && hb build -f`

## 四、运行结果

设置串口工具波特率为115200，复位开发板，查看打印结果

```
[Timer Test] osTimerNew(periodic timer) success, tid: 0xe9b4c.
[Timer Test] osTimerStart(periodic timer) success, wait a while and stop.
[Timer Test] times:0.
[Timer Test] times:1.
[Timer Test] times:2.
[Timer Test] stop periodic timer, status :0.
[Timer Test] kill periodic timer, status :0.
```

### 【套件支持】

##### 1. 套件购买  https://item.taobao.com/item.htm?abbucket=16&id=816685710481&ns=1&priceTId=214783b117346662457694855ed644&skuId=5533042544092&spm=a21n57.sem.item.49.46a639031zWytE&utparam=%7B%22aplus_abtest%22%3A%22b28048df8f009463834be6bdac2a3713%22%7D&xxc=taobaoSearch

##### 2. 技术资料

- Gitee码云网站（使用说明书、规格说明书、OpenHarmony开发案例等） **https://gitee.com/hihopeorg_group/near-link**
- fbb_ws63代码仓（SDK包、技术文档下载）**https://gitee.com/HiSpark/fbb_ws63**

##### 3. 互动交流
- 海思社区星闪专区-论坛 **https://developer.hisilicon.com/forum/0133146886267870001**
