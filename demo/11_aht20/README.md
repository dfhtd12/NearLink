# HopeRun IoT Development Kit--I2C（i2c_aht20）

![hihope_illustration](../../Image/hihope_illustration.png)

An integrated software and hardware development kit based on WS63E solution, providing a comprehensive suite for embedded system development.

![wifi_iot](../../Image/HH-K01.png)

## 一、I2C API

| API                                                          | Description                                            |
| ------------------------------------------------------------ | ------------------------------------------------------ |
| I2cInit (WifiIotI2cIdx id, unsigned int baudrate)            | Initialize the I2C device with the specified baud rate |
| I2cDeinit (WifiIotI2cIdx id)                                 | Deinitialize the I2C device                            |
| I2cWrite (WifiIotI2cIdx id, unsigned short deviceAddr, const WifiIotI2cData *i2cData) | Write data to the I2C device                           |
| I2cRead (WifiIotI2cIdx id, unsigned short deviceAddr, const WifiIotI2cData *i2cData) | Read data from the I2C device                          |



## 二、Aht20 API

This example provides a driver library for the AHT20 digital temperature and humidity sensor for Harmony OS, with the following features:

* Uses Harmony OS's IoT hardware interfaces;
* Offers simple and easy-to-use interfaces that are also easy to port;
* Includes built-in test programs for direct testing;

| API                                                       | Description                                  |
| --------------------------------------------------------- | -------------------------------------------- |
| uint32_t AHT20_Calibrate(void)                            | Calibrate, return 0 if success               |
| uint32_t AHT20_StartMeasure(void)                         | Trigger measurement, return 0 if success     |
| uint32_t AHT20_GetMeasureResult(float* temp, float* humi) | Read measurement result, return 0 if success |

## 3. **Compile**

1. Copy the `11_aht20` directory to the `applications\sample\wifi-iot\app` directory within the Oniro source code
2. Modify the `BUILD.gn` in the `applications\sample\wifi-iot\app` directory of the Oniro source code by replacing the `features` variable with:

```
    features = [
        ...
        "11_aht20:i2c_demo",
        ...
    ]
```
3. In the file `config.py` located at `device\soc\hisilicon\ws63v100\sdk\build\config\target_config\ws63`, locate the section labeled `'ws63-liteos-app'`. Within this section, add the following code to the `'ram_component'` field:
```
"i2c_demo"
```

4. In the file `ohos.cmake` located at `device\soc\hisilicon\ws63v100\sdk\libs_url\ws63\cmake`, locate the section labeled `"ws63-liteos-app"`. Within this section, find the `set(COMPONENT_LIST` statement and append the following code to its argument list:
```
"i2c_demo"
```
5. Execute the following command in the root directory of the Oniro SDK: `rm -rf out && hb set -p nearlink_dk_3863 && hb build -f`


## 4. Result

After flashing the file, press the reset button. The series tool will output the data of humidity and temperature by sensors.



### 【Dev-kits】

##### 1. Online marketplaces  https://item.taobao.com/item.htm?abbucket=16&id=816685710481&ns=1&priceTId=214783b117346662457694855ed644&skuId=5533042544092&spm=a21n57.sem.item.49.46a639031zWytE&utparam=%7B%22aplus_abtest%22%3A%22b28048df8f009463834be6bdac2a3713%22%7D&xxc=taobaoSearch

##### 2. **Technical Documentation**

- **Gitee CodeCloud** (User Manuals, Specifications, Oniro Development Cases) **https://gitee.com/hihopeorg_group/near-link**
- **fbb_ws63 Repository** (SDK Packages, Technical Documentation Downloads)**https://gitee.com/HiSpark/fbb_ws63**

##### 3. **Interaction and Support**

- **Hisilicon Community - NearLink Zone Forum** **https://developer.hisilicon.com/forum/0133146886267870001**

