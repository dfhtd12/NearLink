# 润和星闪派物联网开发套件--WiFi

![hihope_illustration](../../Image/hihope_illustration.png)

[润和星闪派物联网开发套件](https://item.taobao.com/item.htm?abbucket=16&id=816685710481&ns=1&priceTId=214783b117346662457694855ed644&skuId=5533042544092&spm=a21n57.sem.item.49.46a639031zWytE&utparam=%7B%22aplus_abtest%22%3A%22b28048df8f009463834be6bdac2a3713%22%7D&xxc=taobaoSearch) 基于海思WS63E解决方案的一套软硬件组合的综合性开发套件。

![wifi_iot](../../Image/HH-K01.png)



## 一、如何编译

1. 实验需要两块开发板
2. 将14_easy_wifi目录复制到openharmony源码的`applications\sample\wifi-iot\app`目录下，
3. 修改openharmony源码的`applications\sample\wifi-iot\app\BUILD.gn`文件，将其中的 `features` 改为：

```
    features = [
        ...
        "14_easy_wifi/demo:wifi_demo",
        "14_easy_wifi/src:easy_wifi",
        ...
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

8. 修改源码中的`applications\sample\wifi-iot\app\14_easy_wifi\demo\BUILD.gn`文件，将其中的 `sources` 改为：

```
   sources = [
        #"wifi_connect_demo.c",
        "wifi_hotspot_demo.c",
    ]
```

9. 在openharmony sdk根目录目录执行：`rm -rf out && hb set -p nearlink_dk_3863 && hb build -f`

10. 给第二块开发板烧录

## 二、运行结果

复位两块开发板，作为STA端的开发板会与作为AP端的开发板建立连接。成功连接，STA端会有如下打印
```
[16:05:57.546]收←◆STA try connect.
[16:05:57.645]收←◆drv_soc_ioctl ioctl_cmd->cmd=47.
drv_soc_ioctl ioctl_cmd->cmd=47.
[osal_event_read:61]:LOS_EventRead failed! ret=0. mask=0x7. mode=0x2
drv_soc_ioctl ioctl_cmd->cmd=47.
drv_soc_ioctl ioctl_cmd->cmd=16.
drv_soc_ioctl ioctl_cmd->cmd=47.
drv_soc_ioctl ioctl_cmd->cmd=6.
drv_soc_ioctl ioctl_cmd->cmd=5.
drv_soc_ioctl ioctl_cmd->cmd=6.
drv_soc_ioctl ioctl_cmd->cmd=6.
drv_soc_ioctl ioctl_cmd->cmd=5.
drv_soc_ioctl ioctl_cmd->cmd=1.
drv_soc_ioctl ioctl_cmd->cmd=3.
drv_soc_ioctl ioctl_cmd->cmd=1.
+NOTICE:CONNECTED
```



### 【套件支持】

##### 1. 套件购买  https://item.taobao.com/item.htm?abbucket=16&id=816685710481&ns=1&priceTId=214783b117346662457694855ed644&skuId=5533042544092&spm=a21n57.sem.item.49.46a639031zWytE&utparam=%7B%22aplus_abtest%22%3A%22b28048df8f009463834be6bdac2a3713%22%7D&xxc=taobaoSearch

##### 2. 技术资料

- Gitee码云网站（使用说明书、规格说明书、OpenHarmony开发案例等） **https://gitee.com/hihopeorg_group/near-link**
- fbb_ws63代码仓（SDK包、技术文档下载）**https://gitee.com/HiSpark/fbb_ws63**

##### 3. 互动交流
- 海思社区星闪专区-论坛 **https://developer.hisilicon.com/forum/0133146886267870001**

