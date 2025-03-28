# Led Experience<a name="ZH-CN_TOPIC_0000001130176841"></a>

## 一、GPIO API

| API                                                          | Description                                                  |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| unsigned int GpioInit(void);                                 | GPIO Init                                                    |
| unsigned int GpioSetDir(WifiIotGpioIdx id, WifiIotGpioDir dir); | Set the direction of the GPIO pin, where the `id` parameter specifies the pin and the `dir` parameter specifies whether it is an input or output. |
| unsigned int GpioSetOutputVal(WifiIotGpioIdx id, WifiIotGpioValue val); | Set the output state of the GPIO pin, where the `id` parameter specifies the pin and the `val` parameter specifies the high or low level. |
| unsigned int IoSetFunc(WifiIotIoName id, unsigned char val); | Set the pin function, where the `id` parameter specifies the pin and the `val` parameter specifies the pin function. |
| unsigned int GpioDeinit(void);                               | GPIO Deinit                                                  |

## 2. Compile

1. Copy the `button_example` directory to the `applications\sample\wifi-iot\app` directory in your local OpenHarmony source code.

2. Modify the `applications\sample\wifi-iot\app\BUILD.gn` file in the OpenHarmony source code:

    - Change the `features` to:

    ```
    features = [
    "button_example:buttonExample",
    ]
    ```

    

4. Modify the `device\soc\hisilicon\ws63v100\sdk\libs_url\ws63\cmake\ohos.cmake` file to add `"buttonExample"` as follows:

    ```
    elseif(${TARGET_COMMAND} MATCHES "ws63-liteos-app")
    set(COMPONENT_LIST "begetutil"   "hilog_lite_static" "samgr_adapter" "bootstrap" "fsmanager_static" "hal_update_static" "hilog_static" "inithook"   "samgr_source"
            "broadcast" "hal_file_static"   "init_log"  "native_file" "udidcomm"
            "cjson_static" "hal_sys_param" "hichainsdk" "hota" "init_utils"  "param_client_lite"
            "hiview_lite_static" "hal_sysparam" "hievent_lite_static" "huks_3.0_sdk"   "samgr" "blackbox_lite" "hal_iothardware" "wifiservice"
            "hidumper_mini" "buttonExample")
    endif()
    ```

5. Modify the `device\soc\hisilicon\ws63v100\sdk\build\config\target_config\ws63\config.py` file to add `"buttonExample"` to the `'ram_component': []` in the `'ws63-liteos-app'` section as follows:

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
            "buttonExample"
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

6. Execute the following command in the root directory of the Oniro SDK: `rm -rf out && hb set -p nearlink_dk_3863 && hb build -f`

## 3. Result

After flashing the file, press the reset button. Press the button on the traffic light board to switch the green light between the following states: always on, blinking, and off.
