# HopeRun IoT Development Kit--IO Control（gpio_button）

![hihope_illustration](../../Image/hihope_illustration.png)

An integrated software and hardware development kit based on WS63E solution, providing a comprehensive suite for embedded system development.

![wifi_iot](../../Image/HH-K01.png)

## 一、GPIO API

| API                                                          | Description                                                  |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| unsigned int GpioGetInputVal(WifiIotGpioIdx id, WifiIotGpioValue *val); | Get the GPIO pin state, where the `id` parameter specifies the pin and the `val` parameter receives the state of the GPIO pin |
| unsigned int GpioRegisterIsrFunc(WifiIotGpioIdx id, WifiIotGpioIntType intType, WifiIotGpioIntPolarity intPolarity, GpioIsrCallbackFunc func, char *arg); | Register a GPIO pin interrupt, where the `id` parameter specifies the pin, the `intType` parameter specifies the interrupt trigger type (edge-triggered or level-triggered), the `intPolarity` parameter specifies the specific edge type (falling or rising edge) or level type (high or low level), the `func` parameter specifies the interrupt handler function, and the `arg` parameter specifies additional arguments for the interrupt handler function |
| typedef void (*GpioIsrCallbackFunc) (char *arg);             | Interrupt handler function prototype, where the `arg` parameter is an additional argument that can be unused (set to NULL) or can point to a user-defined type |
| unsigned int GpioUnregisterIsrFunc(WifiIotGpioIdx id);       | Unregister a GPIO pin interrupt, where the `id` parameter specifies the pin |

## 二、**Correspondence Between User button and Pegasus Chip Pins**

- **USER Button:** GPIO5 / Button Interrupt to Toggle LED State


## 3. **Compile**

1. Copy the `07_gpiobutton` directory to the `applications\sample\wifi-iot\app` directory within the Oniro source code.
2. Modify the `BUILD.gn` in the `applications\sample\wifi-iot\app` directory of the Oniro source code by replacing the `features` variable with:

```
    features = [
        ...
        "07_gpiobutton:button_example",
        ...
    ]
```
3. In the file `config.py` located at `device\soc\hisilicon\ws63v100\sdk\build\config\target_config\ws63`, locate the section labeled `'ws63-liteos-app'`. Within this section, add the following code to the `'ram_component'` field:
```
"button_example"
```

4. In the file `ohos.cmake` located at `device\soc\hisilicon\ws63v100\sdk\libs_url\ws63\cmake`, locate the section labeled `"ws63-liteos-app"`. Within this section, find the `set(COMPONENT_LIST` statement and append the following code to its argument list:
```
"button_example"
```
5. Execute the following command in the root directory of the Oniro SDK: `rm -rf out && hb set -p nearlink_dk_3863 && hb build -f`

## 4. Result

After flashing the file, press the reset button to start the program. The yellow LED on the red-green LED board (corresponding to GPIO10 on the core board) will first flash. When the USER button on the core board is pressed, the LED will turn off. Pressing the USER button again will turn the LED back on.

### 【Dev-kits】

##### 1. Online marketplaces  https://item.taobao.com/item.htm?abbucket=16&id=816685710481&ns=1&priceTId=214783b117346662457694855ed644&skuId=5533042544092&spm=a21n57.sem.item.49.46a639031zWytE&utparam=%7B%22aplus_abtest%22%3A%22b28048df8f009463834be6bdac2a3713%22%7D&xxc=taobaoSearch

##### 2. **Technical Documentation**

- **Gitee CodeCloud** (User Manuals, Specifications, Oniro Development Cases) **https://gitee.com/hihopeorg_group/near-link**
- **fbb_ws63 Repository** (SDK Packages, Technical Documentation Downloads)**https://gitee.com/HiSpark/fbb_ws63**

##### 3. **Interaction and Support**

- **Hisilicon Community - NearLink Zone Forum** **https://developer.hisilicon.com/forum/0133146886267870001**

