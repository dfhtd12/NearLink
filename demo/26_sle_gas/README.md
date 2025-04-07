# HopeRun IoT Development Kit--（SLE）WS63 transmits data of gas sensor by NearLink

![hihope_illustration](../../Image/hihope_illustration.png)

An integrated software and hardware development kit based on WS63E solution, providing a comprehensive suite for embedded system development.

![wifi_iot](../../Image/HH-K01.png)

## 1. OH API

### Device Discovery OH API

| API                                | Description                                              |
| ---------------------------------- | -------------------------------------------------------- |
| `EnableSle`                        | Enable SLE                                               |
| `SleSetLocalAddr`                  | Set local device address                                 |
| `SleSetAnnounceData`               | Set device advertisement data                            |
| `SleSetAnnounceParam`              | Set Announce Parameter                                   |
| `SleStartAnnounce`                 | Start Announce                                           |
| `SleSetSeekParam`                  | Set Seek Parameter                                       |
| `SleStartSeek`                     | Start Seek                                               |
| `SleStopSeek`                      | Stop Seek                                                |
| `SleAnnounceSeekRegisterCallbacks` | Register callback functions for device Announce and Seek |

### Connection Manager OH API

| API                              | Description                                |
| -------------------------------- | ------------------------------------------ |
| `SleConnectRemoteDevice`         | Initiate connection request to peer device |
| `SleConnectionRegisterCallbacks` | Register connection management callback    |
| `SlePairRemoteDevice`            | Pair with remote device                    |

### SSAP server OH API

| API                      | Description                                             |
| ------------------------ | ------------------------------------------------------- |
| `ssapsRegisterServer`    | Register SSAP server                                    |
| `SsapsAddDescriptorSync` | Add characteristic descriptor synchronization interface |
| `SsapsAddPropertySync`   | Add characteristic synchronization interface            |
| `SsapsAddServiceSync`    | Add service synchronization interface                   |
| `SsapsStartService`      | Start service                                           |
| `SsapsNotifyIndicate`    | Send a notification or indication to the peer device    |
| `SsapsRegisterCallbacks` | Register the server-side callback function              |

### SSAP client OH API

| API                   | Description            |
| --------------------- | ---------------------- |
| `SsapcRegisterClient` | Register SSAP client   |
| `SsapWriteReq`        | Initiate write request |


## 2. Correspondence between the Environment Board and the Main Chip Pins
1. The passive buzzer Beep pin corresponds to gpio9 (PWM1) on the core board.
1. The ADC pin of the MQ-2 combustible gas sensor corresponds to gpio11 (ADC4) on the core board.

## 3. **Compile**

1. The experiment requires two development boards and a environment board.
2. Copy the `26_sle_gas` directory to the `applications\sample\wifi-iot\app` directory within the Oniro source code.
3. Modify the `BUILD.gn` in the `applications\sample\wifi-iot\app` directory of the Oniro source code by replacing the `features` variable with:

```
    features = [
        ...
        "26_sle_gas:sle_gas_demo",
        ...
    ]
```
4. In the file `config.py` located at `device\soc\hisilicon\ws63v100\sdk\build\config\target_config\ws63`, locate the section labeled `'ws63-liteos-app'`. Within this section, add the following code to the `'ram_component'` field:
```
"sle_gas_demo"
```

5. In the file `ohos.cmake` located at `device\soc\hisilicon\ws63v100\sdk\libs_url\ws63\cmake`, locate the section labeled `"ws63-liteos-app"`. Within this section, find the `set(COMPONENT_LIST` statement and append the following code to its argument list:
```
"sle_gas_demo"
```
6. Modify the `BUILD.gn` in the `applications\sample\wifi-iot\app\14_easy_wifi\demo\BUILD.gn` directory of the Oniro source code by replacing the `sources` variable with:

```
  sources = [ 
    "sle_uart_client.c",
    #"sle_uart_server_adv.c",
    #"sle_uart_server.c",
    "hal_iot_gpio_ex.c",
  ]
```
or
```
  sources = [ 
    #"sle_uart_client.c",
    "sle_uart_server_adv.c",
    "sle_uart_server.c",
    "hal_iot_gpio_ex.c",
  ]
```

7. Execute the following command in the root directory of the Oniro SDK: `rm -rf out && hb set -p nearlink_dk_3863 && hb build -f`

8. If the compilation fails, you can use a serial port assistant tool with a baud rate set to 115200 to send the AT command (`AT+SYSINFO\n`) to the development board. If the returned SDK version is lower than **SDK Version: 1.10.102**, you will need to replace the driver files related to ADC. For detailed steps, refer to the `替换文件步骤.txt` in the `adc_driver` directory.

## 4. Result
The WS63 server will send gas sensor data to the WS63 client every 100ms, with a default baud rate of 115200. You can use a serial port tool to view the data received by the client. When the WS63 server reads an ADC voltage value greater than 1000 from the MQ-2 combustible gas sensor, the buzzer will sound an alarm. If the value is below 1000, the alarm will stop.

### 【Dev-kits】

##### 1. Online marketplaces  https://item.taobao.com/item.htm?abbucket=16&id=816685710481&ns=1&priceTId=214783b117346662457694855ed644&skuId=5533042544092&spm=a21n57.sem.item.49.46a639031zWytE&utparam=%7B%22aplus_abtest%22%3A%22b28048df8f009463834be6bdac2a3713%22%7D&xxc=taobaoSearch

##### 2. **Technical Documentation**

- **Gitee CodeCloud** (User Manuals, Specifications, Oniro Development Cases) **https://gitee.com/hihopeorg_group/near-link**
- **fbb_ws63 Repository** (SDK Packages, Technical Documentation Downloads)**https://gitee.com/HiSpark/fbb_ws63**

##### 3. **Interaction and Support**

- **Hisilicon Community - NearLink Zone Forum** **https://developer.hisilicon.com/forum/0133146886267870001**