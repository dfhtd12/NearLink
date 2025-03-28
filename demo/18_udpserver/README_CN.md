# 润和星闪派物联网开发套件--WS63作为udp服务端与远端进行通信

![hihope_illustration](../../Image/hihope_illustration.png)

[润和星闪派物联网开发套件](https://item.taobao.com/item.htm?abbucket=16&id=816685710481&ns=1&priceTId=214783b117346662457694855ed644&skuId=5533042544092&spm=a21n57.sem.item.49.46a639031zWytE&utparam=%7B%22aplus_abtest%22%3A%22b28048df8f009463834be6bdac2a3713%22%7D&xxc=taobaoSearch) 基于海思WS63E解决方案的一套软硬件组合的综合性开发套件。

![wifi_iot](../../Image/HH-K01.png)



## 一、如何编译

1. 将12_oled、14_easy_wifi、18_udpserver目录复制到openharmony源码的`applications\sample\wifi-iot\app`目录下，
2. 修改openharmony源码的`applications\sample\wifi-iot\app\BUILD.gn`文件，将其中的 `features` 改为：
```
    features = [
       ...
       "12_oled/src:oled_ssd1306",
       "14_easy_wifi/src:easy_wifi",
       "18_udpserver:udpserver",、
       ...
    ]
```
3. 在`device\soc\hisilicon\ws63v100\sdk\build\config\target_config\ws63\config.py`文件中，找到`'ws63-liteos-app'`部分，在其`'ram_component'`中，添加以下代码：
```
"oled_ssd1306" ,"easy_wifi" ,"udpserver"
```

4. 在`device\soc\hisilicon\ws63v100\sdk\libs_url\ws63\cmake\ohos.cmake`文件中，找到`"ws63-liteos-app"`部分，在其`set(COMPONENT_LIST`部分，添加以下代码：
```
"oled_ssd1306"  "easy_wifi"  "udpserver"
```
5. 在openharmony sdk根目录目录执行：`rm -rf out && hb set -p nearlink_dk_3863 && hb build -f`


## 三、实验步骤和现象

1. 给开发板烧录程序。

2. 复位开发板，开发板首先会尝试连接wifi,连接的wifi名需要和connect_wifi_test.c配置的一致，连接wifi成功后OLED屏幕会显示WS63服务端的port,默认是1234。

3. 用手机或者电脑连上热点，连接的wifi需要和WS63开发板一致，启动一个udp客户端，IP需要配置为WS63服务端的ip地址（可以在WS63串口打印中看到WS63服务端的ip地址，如下图红色框选部分）,port需要配置为WS63服务端的port,默认是1234。

  ![image-1](./../../Image/18_uspserver/image-1.png)

4. udp客户端给开发板发送一条消息后，开发板的oled屏幕和串口会显示udp 客户端发送过来的消息。

  ![iamge-2](./../../Image/18_uspserver/image-2.jpg)

  ![iamge-3](./../../Image/18_uspserver/image-3.jpg)


### 【套件支持】

##### 1. 套件购买  https://item.taobao.com/item.htm?abbucket=16&id=816685710481&ns=1&priceTId=214783b117346662457694855ed644&skuId=5533042544092&spm=a21n57.sem.item.49.46a639031zWytE&utparam=%7B%22aplus_abtest%22%3A%22b28048df8f009463834be6bdac2a3713%22%7D&xxc=taobaoSearch

##### 2. 技术资料

- Gitee码云网站（使用说明书、规格说明书、OpenHarmony开发案例等） **https://gitee.com/hihopeorg_group/near-link**
- fbb_ws63代码仓（SDK包、技术文档下载）**https://gitee.com/HiSpark/fbb_ws63**

##### 3. 互动交流
- 海思社区星闪专区-论坛 **https://developer.hisilicon.com/forum/0133146886267870001**

