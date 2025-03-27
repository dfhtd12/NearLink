# 润和星闪派物联网开发套件--IO控制（gpio_button）

![hihope_illustration](../../Image/hihope_illustration.png)

[润和星闪派物联网开发套件](https://item.taobao.com/item.htm?abbucket=16&id=816685710481&ns=1&priceTId=214783b117346662457694855ed644&skuId=5533042544092&spm=a21n57.sem.item.49.46a639031zWytE&utparam=%7B%22aplus_abtest%22%3A%22b28048df8f009463834be6bdac2a3713%22%7D&xxc=taobaoSearch) 基于海思WS63E解决方案的一套软硬件组合的综合性开发套件。

![wifi_iot](../../Image/HH-K01.png)

## 一、GPIO API

| API名称                                                      | 说明                                                         |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| unsigned int GpioGetInputVal(WifiIotGpioIdx id, WifiIotGpioValue *val); | 获取GPIO引脚状态，id参数用于指定引脚，val参数用于接收GPIO引脚状态 |
| unsigned int GpioRegisterIsrFunc(WifiIotGpioIdx id, WifiIotGpioIntType intType, WifiIotGpioIntPolarity intPolarity, GpioIsrCallbackFunc func, char *arg); | 注册GPIO引脚中断，id参数用于指定引脚，intType参数用于指定中断触发类型（边缘触发或水平触发），intPolarity参数用于指定具体的边缘类型（下降沿或上升沿）或水平类型（高电平或低电平），func参数用于指定中断处理函数，arg参数用于指定中断处理函数的附加参数 |
| typedef void (*GpioIsrCallbackFunc) (char *arg);             | 中断处理函数原型，arg参数为附加参数，可以不适用（填NULL），或传入指向用户自定义类型的参数 |
| unsigned int GpioUnregisterIsrFunc(WifiIotGpioIdx id);       | 解除GPIO引脚中断注册，id参数用于指定引脚                     |

## 二、核心板USER按键与主控芯片（Pegasus）引脚的对应关系

- **USER按键：**GPIO5/按键中断控制LED灯状态反转


## 三、如何编译

1. 将07_gpiobutton目录复制到openharmony源码的`applications\sample\wifi-iot\app`目录下，
2. 修改openharmony源码的`applications\sample\wifi-iot\app\BUILD.gn`文件，将其中的 `features` 改为：

```
    features = [
        ...
        "07_gpiobutton:button_example",
        ...
    ]
```
3. 在`device\soc\hisilicon\ws63v100\sdk\build\config\target_config\ws63\config.py`文件中，找到`'ws63-liteos-app'`部分，在其`'ram_component'`中，添加以下代码：
```
"button_example"
```

4. 在`device\soc\hisilicon\ws63v100\sdk\libs_url\ws63\cmake\ohos.cmake`文件中，找到`"ws63-liteos-app"`部分，在其`set(COMPONENT_LIST`部分，添加以下代码：
```
"button_example"
```
5. 在openharmony sdk根目录目录执行：`rm -rf out && hb set -p nearlink_dk_3863 && hb build -f`

## 四、运行结果

烧录文件后，按下reset按键，程序开始运行，红绿灯板上的黄色led灯（对应核心板gpio10）会先闪烁，在按下核心板USER按键时，led会熄灭，再次按下USER按键，led会亮



### 【套件支持】

##### 1. 套件购买  https://item.taobao.com/item.htm?abbucket=16&id=816685710481&ns=1&priceTId=214783b117346662457694855ed644&skuId=5533042544092&spm=a21n57.sem.item.49.46a639031zWytE&utparam=%7B%22aplus_abtest%22%3A%22b28048df8f009463834be6bdac2a3713%22%7D&xxc=taobaoSearch

##### 2. 技术资料

- Gitee码云网站（使用说明书、规格说明书、OpenHarmony开发案例等） **https://gitee.com/hihopeorg_group/near-link**
- fbb_ws63代码仓（SDK包、技术文档下载）**https://gitee.com/HiSpark/fbb_ws63**

##### 3. 互动交流
- 海思社区星闪专区-论坛 **https://developer.hisilicon.com/forum/0133146886267870001**

