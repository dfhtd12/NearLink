# 串口打印实验<a name="ZH-CN_TOPIC_0000001130176841"></a>


## 一、UART OH API

| API名称                                                      | 说明                                      |
| ------------------------------------------------------------ | ---------------------------------------- |
| unsigned int IoTUartInit(unsigned int id, const IotUartAttribute *param);                 | UART初始化   |
| int IoTUartWrite(unsigned int id, const unsigned char *data, unsigned int dataLen);       | 串口发送     |


## 二、如何编译

1. 将demo_uart目录复制到本地openharmony源码的applications\sample\wifi-iot\app目录下

2. 修改openharmony的`applications\sample\wifi-iot\app\BUILD.gn`文件：
   * 将其中的 features 改为：
   ```
    features = [
        "demo_uart:demo_uart",
    ]
   ```

3. 修改`device\soc\hisilicon\ws63v100\sdk\libs_url\ws63\cmake\ohos.cmake`文件添加 `"demo_uart"`，如下：
    ```
    elseif(${TARGET_COMMAND} MATCHES "ws63-liteos-app")
    set(COMPONENT_LIST "begetutil"   "hilog_lite_static" "samgr_adapter" "bootstrap" "fsmanager_static" "hal_update_static" "hilog_static" "inithook"   "samgr_source"
            "broadcast" "hal_file_static"   "init_log"  "native_file" "udidcomm"
            "cjson_static" "hal_sys_param" "hichainsdk" "hota" "init_utils"  "param_client_lite"
            "hiview_lite_static" "hal_sysparam" "hievent_lite_static" "huks_3.0_sdk"   "samgr" "blackbox_lite" "hal_iothardware" "wifiservice"
            "hidumper_mini" "demo_uart")
    endif()
    ```

4. 修改`device\soc\hisilicon\ws63v100\sdk\build\config\target_config\ws63\config.py`文件在`'ws63-liteos-app'`中`'ram_component': []'`添加 `"demo_uart"`，如下：
   ```
       'ws63-liteos-app': {
        'base_target_name': 'target_ws63_app_rom_template',
        'os': 'liteos',
        'defines': [
            ......
        ],
        'ram_component': [
            .......
            'xo_trim_port',
            "mqtt",
            "demo_uart"
        ],
        'ccflags': [
            "-DBOARD_ASIC", '-DPRE_ASIC',
        ],
        'application': 'application',
        'bin_name': 'ws63-liteos-app',
        'smaller': True,
        'hso_enable_bt': True,
        'hso_enable': True,
        'codesize_statistic': True,
        'nv_update':True,
        'generate_efuse_bin': True,
        'copy_files_to_interim': True
    },
   ```
5. 在openharmony源码目录下执行：`rm -rf out && hb set -p nearlink_dk_3863 && hb build -f`



## 三、运行结果

烧录文件后，按下reset按键，波特率为115200，串口1和串口2会打印串口接收到的数据。
