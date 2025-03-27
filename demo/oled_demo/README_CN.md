# OLED显示hello world实验<a name="ZH-CN_TOPIC_0000001130176841"></a>


## 一、I2C API

| API名称                                                      | 说明                            |
| ------------------------------------------------------------ | ------------------------------- |
| I2cInit (WifiIotI2cIdx id, unsigned int baudrate)            | 用指定的波特速率初始化I2C设备   |
| I2cDeinit (WifiIotI2cIdx id)                                 | 取消初始化I2C设备               |
| I2cWrite (WifiIotI2cIdx id, unsigned short deviceAddr, const WifiIotI2cData *i2cData) | 将数据写入I2C设备               |
| I2cRead (WifiIotI2cIdx id, unsigned short deviceAddr, const WifiIotI2cData *i2cData) | 从I2C设备中读取数据             |
| I2cWriteread (WifiIotI2cIdx id, unsigned short deviceAddr, const WifiIotI2cData *i2cData) | 向I2C设备发送数据并接收数据响应 |

## 二、ssd1306 API

本样例提供了一个HarmonyOS IoT硬件接口的SSD1306 OLED屏驱动库，其特点如下：

* 使用Harmony OS的IoT硬件接口;
* 接口简洁易于使用、易于移植;
* 内置了测试程序，可直接进行测试;

| API名称                                                      | 说明                              |
| ------------------------------------------------------------ | -------------------------------- |
| uint32_t OledInit(void);                                     | 初始化                            |
| void OledFillScreen(uint8_t fillData);                       | 填充屏幕                          |
| void OledShowChar(uint8_t x, uint8_t y, uint8_t ch, Font font);               | 显示一个字符     |
| void void OledShowString(uint8_t x, uint8_t y, const char* str, Font font);   | 显示字符串        |
              |


## 三、如何编译

1. 将oled_demo目录复制到openharmony源码的`applications\sample\wifi-iot\app`目录下，
2. 修改openharmony源码的`applications\sample\wifi-iot\app\BUILD.gn`文件，将其中的 `features` 改为：

```
    features = [
        "oled_demo:oled_demo",
    ]
```
3. 在`device\soc\hisilicon\ws63v100\sdk\build\config\target_config\ws63\config.py`文件中，找到`'ws63-liteos-app'`部分，在其`'ram_component'`中，添加以下代码：
```
"oled_demo"
```

4. 在`device\soc\hisilicon\ws63v100\sdk\libs_url\ws63\cmake\ohos.cmake`文件中，找到`"ws63-liteos-app"`部分，在其`set(COMPONENT_LIST`部分，添加以下代码：
```
"oled_demo"
```
5. 在openharmony sdk根目录目录执行：`rm -rf out && hb set -p nearlink_dk_3863 && hb build -f`



## 四、效果演示


烧录文件后，按下reset按键，程序开始运行，OLED屏幕会显示Hello,World!



