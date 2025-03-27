# 润和星闪派物联网开发套件--I2C（i2c_aht20）

![hihope_illustration](../../Image/hihope_illustration.png)

[润和星闪派物联网开发套件](https://item.taobao.com/item.htm?abbucket=16&id=816685710481&ns=1&priceTId=214783b117346662457694855ed644&skuId=5533042544092&spm=a21n57.sem.item.49.46a639031zWytE&utparam=%7B%22aplus_abtest%22%3A%22b28048df8f009463834be6bdac2a3713%22%7D&xxc=taobaoSearch) 基于海思WS63E解决方案的一套软硬件组合的综合性开发套件。

![wifi_iot](../../Image/HH-K01.png)

## 一、I2C API

| API名称                                                      | 说明                            |
| ------------------------------------------------------------ | ------------------------------- |
| I2cInit (WifiIotI2cIdx id, unsigned int baudrate)            | 用指定的波特速率初始化I2C设备   |
| I2cDeinit (WifiIotI2cIdx id)                                 | 取消初始化I2C设备               |
| I2cWrite (WifiIotI2cIdx id, unsigned short deviceAddr, const WifiIotI2cData *i2cData) | 将数据写入I2C设备               |
| I2cRead (WifiIotI2cIdx id, unsigned short deviceAddr, const WifiIotI2cData *i2cData) | 从I2C设备中读取数据             |



## 二、Aht20 API

本样例提供了一个鸿蒙AHT20数字温湿度传感器驱动库，其功能如下：

* 使用Harmony OS的IoT硬件接口;
* 接口简洁易于使用、易于移植;
* 内置了测试程序，可直接进行测试;

| API名称                                                   | 说明                    |
| --------------------------------------------------------- | ----------------------- |
| uint32_t AHT20_Calibrate(void)                            | 校准，成功返回0         |
| uint32_t AHT20_StartMeasure(void)                         | 触发测量，成功返回0     |
| uint32_t AHT20_GetMeasureResult(float* temp, float* humi) | 读取测量结果，成功返回0 |

## 三、如何编译

1. 将11_aht20目录复制到openharmony源码的`applications\sample\wifi-iot\app`目录下，
2. 修改openharmony源码的`applications\sample\wifi-iot\app\BUILD.gn`文件，将其中的 `features` 改为：

```
    features = [
        ...
        "11_aht20:i2c_demo",
        ...
    ]
```
3. 在`device\soc\hisilicon\ws63v100\sdk\build\config\target_config\ws63\config.py`文件中，找到`'ws63-liteos-app'`部分，在其`'ram_component'`中，添加以下代码：
```
"i2c_demo"
```

4. 在`device\soc\hisilicon\ws63v100\sdk\libs_url\ws63\cmake\ohos.cmake`文件中，找到`"ws63-liteos-app"`部分，在其`set(COMPONENT_LIST`部分，添加以下代码：
```
"i2c_demo"
```
5. 在openharmony sdk根目录目录执行：`rm -rf out && hb set -p nearlink_dk_3863 && hb build -f`


## 四、运行结果

烧录文件后，按下reset按键，程序开始运行，串口工具会输出测量后的温湿度数据



### 【套件支持】

##### 1. 套件购买  https://item.taobao.com/item.htm?abbucket=16&id=816685710481&ns=1&priceTId=214783b117346662457694855ed644&skuId=5533042544092&spm=a21n57.sem.item.49.46a639031zWytE&utparam=%7B%22aplus_abtest%22%3A%22b28048df8f009463834be6bdac2a3713%22%7D&xxc=taobaoSearch

##### 2. 技术资料

- Gitee码云网站（使用说明书、规格说明书、OpenHarmony开发案例等） **https://gitee.com/hihopeorg_group/near-link**
- fbb_ws63代码仓（SDK包、技术文档下载）**https://gitee.com/HiSpark/fbb_ws63**

##### 3. 互动交流
- 海思社区星闪专区-论坛 **https://developer.hisilicon.com/forum/0133146886267870001**

