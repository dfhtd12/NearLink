# Led实验<a name="ZH-CN_TOPIC_0000001130176841"></a>

## 一、GPIO API

| API名称                                                      | 说明                                                         |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| unsigned int GpioInit(void);                                 | GPIO模块初始化                                               |
| unsigned int GpioSetDir(WifiIotGpioIdx id, WifiIotGpioDir dir); | 设置GPIO引脚方向，id参数用于指定引脚，dir参数用于指定输入或输出 |
| unsigned int GpioSetOutputVal(WifiIotGpioIdx id, WifiIotGpioValue val); | 设置GPIO引脚的输出状态，id参数用于指定引脚，val参数用于指定高电平或低电平 |
| unsigned int IoSetFunc(WifiIotIoName id, unsigned char val); | 设置引脚功能，id参数用于指定引脚，val用于指定引脚功能        |
| unsigned int GpioDeinit(void);                               | 解除GPIO模块初始化           

## 二、如何编译

1. 将led_demo目录复制到本地openharmony源码的applications\sample\wifi-iot\app目录下

2. 修改openharmony的`applications\sample\wifi-iot\app\BUILD.gn`文件：
   * 将其中的 features 改为：
    features = [
        "led_demo:ledDemo",
    ]

3. 修改`device\soc\hisilicon\ws63v100\sdk\libs_url\ws63\cmake\ohos.cmake`文件添加 `"ledDemo"`，如下：

    ```
    elseif(${TARGET_COMMAND} MATCHES "ws63-liteos-app")
    set(COMPONENT_LIST "begetutil"   "hilog_lite_static" "samgr_adapter" "bootstrap" "fsmanager_static" "hal_update_static" "hilog_static" "inithook"   "samgr_source"
            "broadcast" "hal_file_static"   "init_log"  "native_file" "udidcomm"
            "cjson_static" "hal_sys_param" "hichainsdk" "hota" "init_utils"  "param_client_lite"
            "hiview_lite_static" "hal_sysparam" "hievent_lite_static" "huks_3.0_sdk"   "samgr" "blackbox_lite" "hal_iothardware" "wifiservice"
            "hidumper_mini" "ledDemo")
    endif()
    ```

4. 修改`device\soc\hisilicon\ws63v100\sdk\build\config\target_config\ws63\config.py`文件在`'ws63-liteos-app'`中`'ram_component': []'`添加 `"ledDemo"`，如下：

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
            "ledDemo"
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

烧录文件后，按下reset按键，会发现智能（炫彩）灯板RGB灯循环亮起（GPIO7\GPIO10\GPIO11）。
