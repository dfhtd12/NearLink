# Serial Port Printing Experiment<a name="ZH-CN_TOPIC_0000001130176841"></a>


## 一、UART OH API

| API                                                          | Description         |
| ------------------------------------------------------------ | ------------------- |
| unsigned int IoTUartInit(unsigned int id, const IotUartAttribute *param); | Initiate UART       |
| int IoTUartWrite(unsigned int id, const unsigned char *data, unsigned int dataLen); | Send by serial port |


## 2. Compile

1. Copy the `demo_uart` directory to the `applications\sample\wifi-iot\app` directory in your local OpenHarmony source code.

2. Modify the `applications\sample\wifi-iot\app\BUILD.gn` file in the OpenHarmony source code:

    - Change the `features` to:

    ```
     features = [
         "demo_uart:demo_uart",
     ]
    ```

3. Modify the `device\soc\hisilicon\ws63v100\sdk\libs_url\ws63\cmake\ohos.cmake` file to add `"demo_uart"` as follows:
    ```
    elseif(${TARGET_COMMAND} MATCHES "ws63-liteos-app")
    set(COMPONENT_LIST "begetutil"   "hilog_lite_static" "samgr_adapter" "bootstrap" "fsmanager_static" "hal_update_static" "hilog_static" "inithook"   "samgr_source"
            "broadcast" "hal_file_static"   "init_log"  "native_file" "udidcomm"
            "cjson_static" "hal_sys_param" "hichainsdk" "hota" "init_utils"  "param_client_lite"
            "hiview_lite_static" "hal_sysparam" "hievent_lite_static" "huks_3.0_sdk"   "samgr" "blackbox_lite" "hal_iothardware" "wifiservice"
            "hidumper_mini" "demo_uart")
    endif()
    ```

4. Modify the `device\soc\hisilicon\ws63v100\sdk\build\config\target_config\ws63\config.py` file to add `"demo_uart"` to the `'ram_component': []` in the `'ws63-liteos-app'` section as follows:
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
5. Execute the following command in the root directory of the Oniro SDK: `rm -rf out && hb set -p nearlink_dk_3863 && hb build -f`



## 3. Result

After flashing the file, press the reset button. Set the baud rate to 115200, and Serial Port 1 and Serial Port 2 will print the data received via the serial ports.
