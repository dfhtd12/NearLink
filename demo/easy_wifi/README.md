# Wi-Fi Connection & Wi-Fi Hotspot Experiment<a name="ZH-CN_TOPIC_0000001130176841"></a>


## Oniro raw Wi-Fi API interfaces of STA mode.

Use the OpenHarmony raw Wi-Fi API interfaces for programming. In STA mode, you need to use the raw STA interfaces and some DHCP client interfaces.

### STA mode

API of Oniro WiFi STA：

| API                                                          | Description                                                  |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| `WifiErrorCode EnableWifi(viod);                             | Enable STA                                                   |
| `WifiErrorCode DisableWifi(void);`                           | Disable STA                                                  |
| `int IsWifiActive(void);`                                    | Check if STA mode is enabled                                 |
| `WifiErrorCode Scan(void);`                                  | Trigger a scan                                               |
| `WifiErrorCode GetScanInfoList(WifiScanInfo* result, unsigned int* size);` | Get the scan results                                         |
| `WifiErrorCode AddDeviceConfig(const WifiDeviceConfig* config, int* result);` | Add a hotspot configuration; if successful, the `netid` will be returned via the `result` |
| `WifiErrorCode GetDeviceConfigs(WifiDeviceConfig* result, unsigned int* size);` | Get all hotspot configurations on the device                 |
| `WifiErrorCode RemoveDevice(int networkId);`                 | Delete hotspot configuration                                 |
| `WifiErrorCode ConnectTo(int networkId);`                    | Connect to a hotspot                                         |
| `WifiErrorCode Disconnect(void);`                            | Disconnect from the hotspot                                  |
| `WifiErrorCode GetLinkedInfo(WifiLinkedInfo* result);`       | Get the information of the currently connected hotspot       |
| `WifiErrorCode RegisterWifiEvent(WifiEvent* event);          | Register event listener                                      |
| `WifiErrorCode UnRegisterWifiEvent(const WifiEvent* event);` | Unregister event listener                                    |
| `WifiErrorCode GetDeviceMacAddress(unsigned char* result);`  | Get MAC address                                              |
| `WifiErrorCode AdvanceScan(WifiScanParams *params);`         | Advanced search                                              |



#### DHCP Client Interfaces

| API                 | Description                        |
| ------------------- | ---------------------------------- |
| netifapi_netif_find | Find the network interface by name |
| netifapi_dhcp_start | Start the DHCP client              |
| netifapi_dhcp_stop  | Stop the DHCP client               |



### AP mode

Use the Oniro raw Wi-Fi API interfaces for programming. In AP mode, you need to use the raw AP mode interfaces and some DHCP server interfaces.

The Oniro Wi-Fi STA mode API interfaces include:

| API                                                          | Description                                      |
| ------------------------------------------------------------ | ------------------------------------------------ |
| `WifiErrorCode EnableHotspot(void);`                         | Enable AP mode                                   |
| `WifiErrorCode DisableHotspot(void);`                        | Disable AP mode                                  |
| `WifiErrorCode SetHotspotConfig(const HotspotConfig* config);` | Get the current hotspot configuration parameters |
| `WifiErrorCode GetHotspotConfig(HotspotConfig* result);`     | Get the current hotspot configuration parameters |
| `int IsHotspotActive(void);`                                 | Check if the AP is enabled                       |
| `WifiErrorCode GetStationList(StationInfo* result, unsigned int* size);` | Get the list of connected devices                |



#### DHCP Server Interfaces

some DHCP server interfaces:

| API                     | Description                                                  |
| ----------------------- | ------------------------------------------------------------ |
| netifapi_netif_set_addr | Set the IP, gateway, and subnet mask for the current interface |
| netifapi_dhcps_start    | Start the DHCP server                                        |
| netifapi_dhcps_stop     | Stop the DHCP server                                         |


## Step



1. The experiment requires two development boards.
2. Copy the `easy_wifi` directory to the `applications\sample\wifi-iot\app` directory within the Oniro source code.，
3. Modify the `BUILD.gn` in the `applications\sample\wifi-iot\app` directory of the Oniro source code by replacing the `features` variable with:

```
    features = [
        "easy_wifi/demo:wifi_demo",
        "easy_wifi/src:easy_wifi",
    ]
```
4. In the file `config.py` located at `device\soc\hisilicon\ws63v100\sdk\build\config\target_config\ws63`, locate the section labeled `'ws63-liteos-app'`. Within this section, add the following code to the `'ram_component'` field:
```
"wifi_demo","easy_wifi"
```

5. In the file `ohos.cmake` located at `device\soc\hisilicon\ws63v100\sdk\libs_url\ws63\cmake`, locate the section labeled `"ws63-liteos-app"`. Within this section, find the `set(COMPONENT_LIST` statement and append the following code to its argument list:
```
"wifi_demo" "easy_wifi"
```
6. Execute the following command in the root directory of the Oniro SDK: `rm -rf out && hb set -p nearlink_dk_3863 && hb build -f`

7. Flash the first development board.

8. Modify the `BUILD.gn` in the `applications\sample\wifi-iot\app\14_easy_wifi\demo\BUILD.gn` directory of the Oniro source code by replacing the `sources` variable with:

```
   sources = [
        #"wifi_connect_demo.c",
        "wifi_hotspot_demo.c",
    ]
```

9. Execute the following command in the root directory of the Oniro SDK: `rm -rf out && hb set -p nearlink_dk_3863 && hb build -f`

10. Flash the second development board.
