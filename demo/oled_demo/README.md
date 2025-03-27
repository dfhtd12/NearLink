# OLED display hello world experiment<a name="ZH-CN_TOPIC_0000001130176841"></a>


## 1. I2C API

| API                                                          | Description                                             |
| ------------------------------------------------------------ | ------------------------------------------------------- |
| I2cInit (WifiIotI2cIdx id, unsigned int baudrate)            | Initialize the I2C device with the specified baud rate  |
| I2cDeinit (WifiIotI2cIdx id)                                 | Deinitialize the I2C device                             |
| I2cWrite (WifiIotI2cIdx id, unsigned short deviceAddr, const WifiIotI2cData *i2cData) | Write data to the I2C device                            |
| I2cRead (WifiIotI2cIdx id, unsigned short deviceAddr, const WifiIotI2cData *i2cData) | Read data from the I2C device                           |
| I2cWriteread (WifiIotI2cIdx id, unsigned short deviceAddr, const WifiIotI2cData *i2cData) | Send data to the I2C device and receive a data response |

## 2. ssd1306 API

* This example provides a driver library for the SSD1306 OLED display using HarmonyOS IoT hardware interfaces, with the following features:
  - Uses HarmonyOS's IoT hardware interfaces;
  - Offers simple and easy-to-use interfaces that are also easy to port;
  - Includes built-in test programs for direct testing;

| API                                                      | Description                   |
| ------------------------------------------------------------ | -------------------------------- |
| uint32_t OledInit(void);                                     | Initialize                |
| void OledFillScreen(uint8_t fillData);                       | Fill the screen         |
| void OledShowChar(uint8_t x, uint8_t y, uint8_t ch, Font font);               | Display a character |
| void void OledShowString(uint8_t x, uint8_t y, const char* str, Font font);   | Display a string |

## 3. Compile

1. Copy the `oled_demo` directory to the `applications\sample\wifi-iot\app` directory in your local OpenHarmony source code.
2. Modify the `applications\sample\wifi-iot\app\BUILD.gn` file in the OpenHarmony source code. Change the `features` to:


```
    features = [
        "oled_demo:oled_demo",
    ]
```
3. In the `device\soc\hisilicon\ws63v100\sdk\build\config\target_config\ws63\config.py` file, locate the `'ws63-liteos-app'` section. In its `'ram_component'`, add the following code:
```
"oled_demo"
```

4. In the `device\soc\hisilicon\ws63v100\sdk\libs_url\ws63\cmake\ohos.cmake` file, locate the `"ws63-liteos-app"` section. In its `set(COMPONENT_LIST` part, add the following code:
```
"oled_demo"
```
5. Execute the following command in the root directory of the Oniro SDK: `rm -rf out && hb set -p nearlink_dk_3863 && hb build -f`



## 4. Result


After flashing the file, press the reset button to start the program. The OLED screen will display "Hello, World!".



