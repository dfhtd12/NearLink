# Led experiment<a name="ZH-CN_TOPIC_0000001130176841"></a>

## 一、GPIO API

| API                                                          | Description                                                  |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| unsigned int GpioInit(void);                                 | Initialize the GPIO module                                   |
| unsigned int GpioSetDir(WifiIotGpioIdx id, WifiIotGpioDir dir); | Set the direction of the GPIO pin, where the `id` parameter specifies the pin and the `dir` parameter specifies whether it is an input or output |
| unsigned int GpioSetOutputVal(WifiIotGpioIdx id, WifiIotGpioValue val); | Set the output state of the GPIO pin, where the `id` parameter specifies the pin and the `val` parameter specifies the high or low level |
| unsigned int IoSetFunc(WifiIotIoName id, unsigned char val); | Set the pin function, where the `id` parameter specifies the pin and the `val` parameter specifies the pin function |
| unsigned int GpioDeinit(void);                               | Deinitialize the GPIO module                                 |

## 2. **Compile**

1. Copy the `led_demo` directory to the `applications\sample\wifi-iot\app` directory within the Oniro source code.

2. Modify the `applications\sample\wifi-iot\app\BUILD.gn` file in OpenHarmony:

   - Change the `features` to:

   ```
   features = [
     "led_demo:ledDemo",
    ]
   ```

   

3. Modify the `device\soc\hisilicon\ws63v100\sdk\libs_url\ws63\cmake\ohos.cmake` file to add `"ledDemo"` as follows:

    ```
    elseif(${TARGET_COMMAND} MATCHES "ws63-liteos-app")
    set(COMPONENT_LIST "begetutil"   "hilog_lite_static" "samgr_adapter" "bootstrap" "fsmanager_static" "hal_update_static" "hilog_static" "inithook"   "samgr_source"
            "broadcast" "hal_file_static"   "init_log"  "native_file" "udidcomm"
            "cjson_static" "hal_sys_param" "hichainsdk" "hota" "init_utils"  "param_client_lite"
            "hiview_lite_static" "hal_sysparam" "hievent_lite_static" "huks_3.0_sdk"   "samgr" "blackbox_lite" "hal_iothardware" "wifiservice"
            "hidumper_mini" "ledDemo")
    endif()
    ```

4. Modify the `device\soc\hisilicon\ws63v100\sdk\build\config\target_config\ws63\config.py` file to add `"ledDemo"` to the `'ram_component': []` in the `'ws63-liteos-app'` section as follows:

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

5. Execute the following command in the root directory of the Oniro SDK: `rm -rf out && hb set -p nearlink_dk_3863 && hb build -f`

## 3. Result

After flashing the file, press the reset button. You will find that the RGB LEDs on the smart (RGB) light board cycle through their colors (GPIO7, GPIO10, GPIO11).
