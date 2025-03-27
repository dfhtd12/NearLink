# WIFI连接&WIFI热点实验<a name="ZH-CN_TOPIC_0000001130176841"></a>


## OpenharmonySTA模式原始API接口

使用Openharmony原始WiFI API接口进行编程，STA模式需要使用原始STA接口以及一些DHCP客户端接口。

### STA模式

OpenharmonyWiFi STA模式的API接口有：

| API                                                          | 功能说明                                |
| ------------------------------------------------------------ | --------------------------------------- |
| `WifiErrorCode EnableWifi(void);`                            | 开启STA                                 |
| `WifiErrorCode DisableWifi(void);`                           | 关闭STA                                 |
| `int IsWifiActive(void);`                                    | 查询STA是否已开启                       |
| `WifiErrorCode Scan(void);`                                  | 触发扫描                                |
| `WifiErrorCode GetScanInfoList(WifiScanInfo* result, unsigned int* size);` | 获取扫描结果                            |
| `WifiErrorCode AddDeviceConfig(const WifiDeviceConfig* config, int* result);` | 添加热点配置，成功会通过result传出netId |
| `WifiErrorCode GetDeviceConfigs(WifiDeviceConfig* result, unsigned int* size);` | 获取本机所有热点配置                    |
| `WifiErrorCode RemoveDevice(int networkId);`                 | 删除热点配置                            |
| `WifiErrorCode ConnectTo(int networkId);`                    | 连接到热点                              |
| `WifiErrorCode Disconnect(void);`                            | 断开热点连接                            |
| `WifiErrorCode GetLinkedInfo(WifiLinkedInfo* result);`       | 获取当前连接热点信息                    |
| `WifiErrorCode RegisterWifiEvent(WifiEvent* event);`         | 注册事件监听                            |
| `WifiErrorCode UnRegisterWifiEvent(const WifiEvent* event);` | 解除事件监听                            |
| `WifiErrorCode GetDeviceMacAddress(unsigned char* result);`  | 获取Mac地址                             |
| `WifiErrorCode AdvanceScan(WifiScanParams *params);`         | 高级搜索                                |



#### DHCP 客户端接口

以及DHCP客户端接口：

| API                 | 描述               |
| ------------------- | ------------------ |
| netifapi_netif_find | 按名称查找网络接口 |
| netifapi_dhcp_start | 启动DHCP客户端     |
| netifapi_dhcp_stop  | 停止DHCP客户端     |



### AP模式

使用Openharmony原始WiFI API接口进行编程，AP模式需要使用原始AP模式接口以及一些DHCP服务端接口。

OpenharmonyWiFi STA模式的API接口有：

| API                                                          | 说明                 |
| ------------------------------------------------------------ | -------------------- |
| `WifiErrorCode EnableHotspot(void);`                         | 打开AP模式           |
| `WifiErrorCode DisableHotspot(void);`                        | 关闭AP模式           |
| `WifiErrorCode SetHotspotConfig(const HotspotConfig* config);` | 设置当前热点配置参数 |
| `WifiErrorCode GetHotspotConfig(HotspotConfig* result);`     | 获取当前热点配置参数 |
| `int IsHotspotActive(void);`                                 | 查询AP是否已开启     |
| `WifiErrorCode GetStationList(StationInfo* result, unsigned int* size);` | 获取接入的设备列表   |



#### DHCP服务端接口

以及一些DHCP服务端接口：

| API                     | 描述                             |
| ----------------------- | -------------------------------- |
| netifapi_netif_set_addr | 设置当前接口的IP、网关、子网掩码 |
| netifapi_dhcps_start    | 启动DHCP服务端                   |
| netifapi_dhcps_stop     | 停止DHCP服务端                   |


## 实验步骤



1. 实验需要两块开发板
2. 将easy_wifi目录复制到openharmony源码的`applications\sample\wifi-iot\app`目录下，
3. 修改openharmony源码的`applications\sample\wifi-iot\app\BUILD.gn`文件，将其中的 `features` 改为：

```
    features = [
        "easy_wifi/demo:wifi_demo",
        "easy_wifi/src:easy_wifi",
    ]
```
4. 在`device\soc\hisilicon\ws63v100\sdk\build\config\target_config\ws63\config.py`文件中，找到`'ws63-liteos-app'`部分，在其`'ram_component'`中，添加以下代码：
```
"wifi_demo","easy_wifi"
```

5. 在`device\soc\hisilicon\ws63v100\sdk\libs_url\ws63\cmake\ohos.cmake`文件中，找到`"ws63-liteos-app"`部分，在其`set(COMPONENT_LIST`部分，添加以下代码：
```
"wifi_demo" "easy_wifi"
```
6. 在openharmony sdk根目录目录执行：`rm -rf out && hb set -p nearlink_dk_3863 && hb build -f`

7. 给第一块开发板烧录

8. 修改源码中的`applications\sample\wifi-iot\app\easy_wifi\demo\BUILD.gn`文件，将其中的 `sources` 改为：

```
   sources = [
        #"wifi_connect_demo.c",
        "wifi_hotspot_demo.c",
    ]
```

9. 在openharmony sdk根目录目录执行：`rm -rf out && hb set -p nearlink_dk_3863 && hb build -f`

10. 给第二块开发板烧录
