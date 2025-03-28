# 客户端和服务端使用星闪功能转发串口数据<a name="ZH-CN_TOPIC_0000001130176841"></a>

### Device Discovery OH接口

| API                                                          | 功能说明                                |
| ------------------------------------------------------------ | --------------------------------------- |
| `EnableSle`                                                  | 开启SLE |
| `SleSetLocalAddr`                                            | 设置本地设备地址 |
| `SleSetAnnounceData`                                         | 设置设备公开数据 |                                 
| `SleSetAnnounceParam`                                        | 设置设备公开参数 |
| `SleStartAnnounce`                                           | 开始广播 |
| `SleSetSeekParam`                                            | 设置扫描参数 |
| `SleStartSeek`                                               | 开始扫描 |
| `SleStopSeek`                                                | 停止扫描 |
| `SleAnnounceSeekRegisterCallbacks`                           | 注册设备公开和设备发现回调函数 |

### Connection Manager OH接口

| API                                                          | 功能说明               |
| ------------------------------------------------------------ | -----------------------|
| `SleConnectRemoteDevice`                                     | 向对端设备发起连接请求 |
| `SleConnectionRegisterCallbacks`                             | 注册连接管理回调函数 |

### SSAP server OH接口

| API                                                          | 功能说明             |
| ------------------------------------------------------------ | -------------------- |
| `ssapsRegisterServer`                                        | 注册 SSAP server |
| `SsapsAddDescriptorSync`                                     | 添加特征描述符同步接口 |
| `SsapsAddPropertySync`                                       | 添加特征同步接口 |
| `SsapsAddServiceSync`                                        | 添加服务同步接口 |
| `SsapsStartService`                                          | 启动服务 |
| `SsapsNotifyIndicate`                                        | 给对端发送通知或指示 |
| `SsapsRegisterCallbacks`                                     | 注册服务端回调函数 |

### SSAP client OH接口

| API                                                          | 功能说明             |
| ------------------------------------------------------------ | -------------------- |
| `SsapcRegisterClient`                                              | 注册 SSAP client |
| `SsapWriteReq`                                               | 发起写请求 |

### SSAP client 原生SDK接口

| API                                                          | 功能说明             |
| ------------------------------------------------------------ | -------------------- |
| `ssapc_register_callbacks`                                   | 注册客户端回调函数 |


### UART 原生SDK接口

| API                                                          | 功能说明             |
| ------------------------------------------------------------ | -------------------- |
| `uapi_uart_register_rx_callback`                             | 注册串口接收回调函数 |





## 如何编译

本项目下有两个示例代码，需要运行在两块WS63开发板上，一块作为服务端，一块作为客户端，SLE服务端需要注释掉`"sle_uart_client.c"`行，放开`"sle_uart_server_adv.c","sle_uart_server.c"`行，SLE客户端需要注释掉`"sle_uart_server_adv.c","sle_uart_server.c"`行，放开`"sle_uart_client.c"`行

1. 将sle_uart_demo文件夹克隆到本地openharmony源码的applications\sample\wifi-iot\app目录下；
2. 修改openharmony的`applications\sample\wifi-iot\app\BUILD.gn`文件：
   * 将其中的 features 改为：
    features = [
        "sle_uart_demo:sle_uart_demo",
    ]
3. 在device/soc/hisilicon/ws63v100/sdkv100/build/config/target_config/ws63/config.py文件的`'ram_component': []`中添加`"sle_uart_demo"`，在device/soc/hisilicon/ws63v100/sdkv100/libs_url/ws63cmake/ohos.cmake文件的`set(COMPONENT_LIST)`中添加`"sle_uart_demo"`；
4. 执行编译命令：`rm -rf out && hb set -p nearlink_dk_3863 && hb build -f`
5. 运行结果:两块ws63开发板可以通过星闪功能转发串口数据，波特率默认为115200，服务端在接收到客户端发送的数据`"123"`后会打印`"client_send_data: 123"`；