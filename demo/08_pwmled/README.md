# 润和星闪派物联网开发套件--PWM（pwm_led）

![hihope_illustration](https://gitee.com/hihopeorg/hispark-hm-pegasus/raw/master/docs/figures/hihope_illustration.png)

[润和星闪派物联网开发套件](https://item.taobao.com/item.htm?abbucket=16&id=816685710481&ns=1&priceTId=214783b117346662457694855ed644&skuId=5533042544092&spm=a21n57.sem.item.49.46a639031zWytE&utparam=%7B%22aplus_abtest%22%3A%22b28048df8f009463834be6bdac2a3713%22%7D&xxc=taobaoSearch) 基于海思WS63E解决方案的一套软硬件组合的综合性开发套件。

![wifi_iot](https://img.alicdn.com/imgextra/i4/3583112207/O1CN01SvRG981SAr7bdEg3i_!!3583112207.png)

## 一、PWM API

| API名称                                                      | 说明              |
| ------------------------------------------------------------ | ----------------- |
| unsigned int PwmInit(WifiIotPwmPort port);                   | PWM模块初始化     |
| unsigned int PwmStart(WifiIotPwmPort port, unsigned short duty, unsigned short freq); | 开始输出PWM信号   |
| unsigned int PwmStop(WifiIotPwmPort port);                   | 停止输出PWM信号   |
| unsigned int PwmDeinit(WifiIotPwmPort port);                 | 解除PWM模块初始化 |



## 二、如何编译

1. 将08_pwmled目录复制到openharmony源码的`applications\sample\wifi-iot\app`目录下，
2. 修改openharmony源码的`applications\sample\wifi-iot\app\BUILD.gn`文件，将其中的 `features` 改为：

```
    features = [
        ...
        "08_pwmled:pwm_led_demo",
        ...
    ]
```
3. 在`device\soc\hisilicon\ws63v100\sdk\build\config\target_config\ws63\config.py`文件中，找到`'ws63-liteos-app'`部分，在其`'ram_component'`中，添加以下代码：
```
"pwm_led_demo"
```

4. 在`device\soc\hisilicon\ws63v100\sdk\libs_url\ws63\cmake\ohos.cmake`文件中，找到`"ws63-liteos-app"`部分，在其`set(COMPONENT_LIST`部分，添加以下代码：
```
"pwm_led_demo"
```
5. 在openharmony sdk根目录目录执行：`rm -rf out && hb set -p nearlink_dk_3863 && hb build -f`

## 三、运行结果

烧录文件后，按下reset按键，程序开始运行，红绿灯板会亮黄色灯（对应核心板gpio10），并且亮度会逐渐变化：暗--亮




### 【套件支持】

##### 1. 套件购买  https://item.taobao.com/item.htm?abbucket=16&id=816685710481&ns=1&priceTId=214783b117346662457694855ed644&skuId=5533042544092&spm=a21n57.sem.item.49.46a639031zWytE&utparam=%7B%22aplus_abtest%22%3A%22b28048df8f009463834be6bdac2a3713%22%7D&xxc=taobaoSearch

##### 2. 技术资料

- Gitee码云网站（使用说明书、规格说明书、OpenHarmony开发案例等） **https://gitee.com/hihopeorg_group/near-link**
- fbb_ws63代码仓（SDK包、技术文档下载）**https://gitee.com/HiSpark/fbb_ws63**

##### 3. 互动交流
- 海思社区星闪专区-论坛 **https://developer.hisilicon.com/forum/0133146886267870001**
