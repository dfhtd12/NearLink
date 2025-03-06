## BS21产品手册

### 一、产品简介

#### 1概述

<img src="../../Image/HH-D03/HH-D03/HH-D03.png" alt="1" style="zoom:25%;" />

主板是基于海思星闪BS21的解决方案，支持BLE5.3协议和BLE Mesh功能，支持SLE1.0协议和SLE Mesh功能，可广泛适用于智能家电等物联网智能终端领域。

#### 2.产品特点

- 丰富的SDK
  - 支持 HID，Battery，HeartRate
  - 提供 Keyboard，Mouse，麦克风等多种应用 example
- 灵活的组网
  - 支持 BLE mesh 组网，支持 256 个节点
  - 支持 SLE mesh 组网，支持 512 个节点
- 稳定可靠的通信能力
  - 支持复杂环境下 TPC，干扰检测， polar 编码等方式增加通信可靠性
  - 支持多设备的高精度时钟同步
- 强大的安全引擎
  - 支持 AES128/256 加解密算法
  - 支持 SM4 加解密算法
  - 内部集成 efuse
  - 内部集成 PMP 特性，支持内存隔 离特性
- 开放的操作系统
  - 支持 FreeRTOS 操作系统
  - 支持 Non-OS
  

#### 3.主要规格

| 模块      | 规格描述                                                     |
| --------- | ------------------------------------------------------------ |
| BLE       | 支持 BLE4.0/4.1/4.2/5.0/5.1/5.2/5.3规范<br />支持数据速率 ：1Mbps，2Mbps，500kbps 和 125kbps |
| SLE       | 支持标准 SLE1.0 规范<br />支持无线帧类型 1 (GFSK 帧)<br />支持无线帧类型 2(超低时延帧)<br />支持测距、测角功能 |
| MCU子系统 | RISC-V 32bit CPU，带浮点单元<br />最大主频 64MHz<br />内嵌512KB Flash，160KB RAM<br />支持 1024bit eFuse<br />支持增强安全特性：AES128/SM4 、TRNG |
| 外围接口  | 支持 2 \* I2C，支持 master 和 slave 模式<br />支持 1 路 2 通道 I2S/PCM<br />支持 2CH PDM<br />支持 3 \* SPI，支持 master 和 slave 模式<br />支持 3 \* PWM<br />支持 USB2.0 HS/FS<br />支持 8 通道 12bit ADC，最大采样 率 1.6M<br />支持 NFC Type2 Tag 功能，支持 NFC 场唤醒功能<br />支持 QDEC 功能<br />支持 KeyScan 功能<br />支持 32 \* GPIO |
| AFE       | 支持 ADC 复用为音频 AMIC 采样                                |
| 其他信息  | 电源电压输入：典型值 5V<br />工作温度：-30℃～+85℃            |

### 二、硬件

#### 1.功能布局

<img src="..\Image\HH-D03\GNBJ.png" alt="GNBJ" style="zoom:50%;" />

<center>图2.1 HH-D03星闪开发板功能</center>

- 用户按键：USER为用户自定义按键，开关S2 通过 GPIO5 引脚向 BS21 上报“按下/释放”状态。功能由软件定制。
- Type-C 接口：可对主板及整个套件进行供电，或连接至电脑进行串口调试、系统烧录。
- 复位按键：RST 为复位按键，可以对主板进行复位。
- 电源指示灯（红色）：用于指示电源状态，正常上电后电源指示灯亮。
- 状态指示灯（三色）：用于指示相关的IO口状态使用，用户通过PWM波进行控制
- 稳压器AMS1117：用于串口5V供电转换为芯片的3.3V供电。
- CH340C USB 转串口芯片：使用串口功能时，需要在 PC 上安装该芯片的驱动程序。
- HH-M03 模组：高度集成BLE 和 SLE，具有高速传输、低延迟、高性能、低功耗的特点，Type-C型 USB 接口及丰富的管脚功能。
- SLE板载天线：用于增强SLE/BLE/Wifi的信号
- SLE外接天线（可选）：用于增强SLE/BLE/Wifi的信号，使用1代IPEX接口，特殊场景下需要很强的信号可以使用，通过更换焊接电阻实现。

#### 2.管脚定义

![YJDY](..\Image\HH-D03\YJDY.png)

<center>图2.2 HH-D03星闪开发板接口</center>

| 序号 | 名称 | 类型 | 功能                                     |
| ---- | ---- | ---- | ---------------------------------------- |
| 1    | 3V3  | PWR  | 电源，3.3V                               |
| 2    | GND  | PWR  | 电源地                                   |
| 3    | DP   | IO   | GPIO8,USB_D+,SPI1_CS1,PWM1               |
| 4    | DM   | IO   | GPIO7,USB_D-,SPI1_CS0,PWM0               |
| 5    | IO6  | IO   | GPIO6,SPI1_TXD,I2S_MCLK                  |
| 6    | IO5  | IO   | GPIO5,AIN3,SPI1_RXD,I2S_DIN              |
| 7    | IO4  | IO   | IO4,AIN2,SPI0_CS1,I2S_DOUT               |
| 8    | IO3  | IO   | IO3,AIN1,SPI0_CS0,I2S_WS                 |
| 9    | IO2  | IO   | IO2,AIN0,SPI0_SCLK,I2S_SCLK              |
| 10   | IO1  | IO   | GPIO1, XTAL_32K_N,SPI0_TXD,DMIC_CLK      |
| 11   | IO0  | IO   | GPIO0, XTAL_32K_P,SPI0_RXD,DMIC_DIN      |
| 12   | IO0  | IO   | IO31,AIN7,LED_OUT                        |
| 13   | IO30 | IO   | IO30,AIN6,QDEC_B                         |
| 14   | IO29 | IO   | IO29,AIN5,QDEC_A                         |
| 15   | IO28 | IO   | IO28,AIN4,I2C1_DATA                      |
| 16   | IO27 | IO   | IO27,I2C1_CLK                            |
| 17   | IO26 | IO   | IO26,I2C0_DATA                           |
| 18   | IO25 | IO   | IO25,I2C0_CLK                            |
| 19   | IO09 | IO   | GPIO9,NFC1,SPI1_CLK,PWM2                 |
| 20   | IO10 | IO   | GPIO10,NFC2,SPI2_RXD,PWM3                |
| 21   | IO11 | IO   | IO11,High speed. SPI_TXD,SPI2_TXD,PWM4   |
| 22   | IO12 | IO   | GPIO12,High speed. SPI_RXD,SPI2_CS0,PWM5 |
| 23   | IO13 | IO   | GPIO13,High speed. SPI_CS,SPI2_CS1,PWM6  |
| 24   | IO14 | IO   | GPIO14,High speed. SPI_CLK,SPI2_CLK,PWM7 |
| 25   | IO15 | IO   | GPIO15,UART_L0_RXD,PWM8                  |
| 26   | IO16 | IO   | GPIO16,UART_L0_TXD,PWM9                  |
| 27   | IO17 | IO   | GPIO17,UART_L1_RTS,PWM10                 |
| 28   | IO18 | IO   | GPIO18,UART_L1_TXD,PWM11                 |
| 29   | IO19 | IO   | GPIO19,UART_L1_CTS,KEY_SCAN_BIR          |
| 30   | IO20 | IO   | GPIO20,UART_L1_RXD                       |
| 21   | IO21 | IO   | GPIO21,UART_H0_CTS                       |
| 32   | S-C  | \-   | SWD_CLK                                  |
| 33   | S-D  | \-   | SWDIO                                    |
| 34   | IO22 | IO   | GPIO22,UART_H0_RTS                       |
| 35   | IO23 | IO   | GPIO23,UART_H0_RXD                       |
| 36   | IO24 | IO   | GPIO24,UART_H0_TXD                       |

#### 3.尺寸

![CCT](..\Image\HH-D03\CCT.png)

<center>图2.3 HH-D03星闪开发板及排针尺寸</center>

#### 4.原理图

[原理图](HH-D03 原理图.pdf )

![8](..\Image\HH-D03\8.jpg)

### 三、AT 指令通信使用指南

#### 1. 蓝牙AT通信流程

##### 1.1服务端配置流程

​	1）使能蓝牙

``` 
AT+BLEENABLE\n
```

​	2）设置蓝牙地址

``` 
AT+BLESETADDR=0,0x112233445566\n
```

​	3）注册服务端回调函数

```
AT+GATTSREGCBK\n
```

​	4）创建一个 GATT server

``` 
AT+GATTSREGSRV=0x1122\n
```

​	5）添加一个 GATT 服务

``` 
AT+GATTSSYNCADDSERV=1,0x1122,1\n
```

​	6）给服务添加一个特征

``` 
AT+GATTSSYNCADDCHAR=1,14,0x2a4a,0x03,0,1,0x01\n
```

​	7）给特征添加一个描述符

``` 
AT+GATTSSYNCADDDESCR=1,14,0x2a4a,0x03,1,0x02\n
```

​	8）启动服务

``` 
AT+GATTSSTARTSERV=1,14\n
```

​	9）设置 BLE 广播数据

``` 
AT+BLESETADVDATA=6,0x112233445566,0,0,1\n
```

​	10）设置 BLE 广播参数

``` 
AT+BLESETADVPAR=48,48,0,0x000000000000,0,0x000000000000,7,0,1,0,1\n
```

​	11）开始发送 BLE 广播

``` 
AT+BLESTARTADV=1\n
```

##### 1.2客户端配置流程

​	1）使能蓝牙

``` 
AT+BLEENABLE\n
```

​	2）设置地址

``` 
AT+BLESETADDR=0,0x112233445577\n
```

​	3）注册客户端回调函数

``` 
AT+GATTCREGCBK\n
```

​	4）创建一个 GATT client

``` 
AT+GATTCREG=0x3434\n
```

​	5）设置 BLE 扫描参数

``` 
AT+BLESETSCANPAR=0x48,0x48,0,1,0\n
```

​	6）连接服务端

``` 
AT+BLECONN=0,0x112233445566\n
```

​	7）配对服务端

``` 
AT+BLEPAIR=0,0x112233445566\n
```

​	8）获取配对信息

``` 
AT+BLEGETPAIREDDEV\n
```

​	9）发现服务

``` 
AT+GATTCFNDSERV=1,0,0x1122\n
```

##### 1.3通信测试

​	1）客户端向服务端发送消息

​	客户端发送写请求，

``` 
AT+GATTCWRITEREQ=1,0,17,1,0x11\n
```

​	服务端打印接收数据说明收发正常。![image-20241119095511581](..\Image\HH-D03\BLEServerData.png)	2)服务端向客户端发送消息

​	服务端发送通知

``` 
AT+GATTSSNDNTFY=1,0,14,1,0x11\n
```

​	客户端读取服务端消息

``` 
AT+GATTCREADBYHDL=1,0,14\n
```

​	客户端打印接收数据说明收发正常![image-20241119101021678](..\Image\HH-D03\BLEClientData.png)

#### 2.星闪AT通信流程

##### 2.1 服务端配置流程

​	1）使能星闪

``` 
AT+SLEENABLE\n
```

​	2）设置地址

``` 
AT+SLESETADDR=0,0x112233445566\n
```

​	3）注册服务端回调函数

```
AT+SSAPSREGCBK\n
```

​	4）创建一个服务端

``` 
AT+SSAPSADDSRV=0x1234\n
```

​	5）添加一个服务

```
AT+SSAPSADDSERV=0x2222,1\n
```

​	6）给服务添加一个属性

```
AT+SSAPSADDPROPERTY=1,0x2323,5,5,2,0x1234\n
```

​	7）给属性添加一个描述符

```
AT+SSAPSADDDESCR=1,2,0x3333,5,5,2,2,0x0200\n
```

​	8）启动服务

```
AT+SSAPSSTARTSERV=1\n
```

​	9）设置 SLE 广播参数

```
AT+SLESETADVPAR=1,3,200,200,0,0x112233445566,0,0x000000000000\n
```

​	10）设置 SLE 广播数据

```
AT+SLESETADVDATA=1,10,4,aabbccddeeff11223344,11224455\n
```

​	11）开始发送 SLE 广播

```
AT+SLESTARTADV=1\n
```

##### 2.2客户端配置流程

​	1）使能星闪

```
AT+SLEENABLE\n
```

​	2）设置地址

```
AT+SLESETADDR=0,0x112233445577\n
```

​	3）注册客户端回调函数

```
AT+SSAPCREGCBK\n
```

​	4）设置 SLE 扫描参数

```
AT+SLESETSCANPAR=1,0x48,0x48\n
```

​	5）开始扫描

```
AT+SLESTARTSCAN\n
```

​	6）停止扫描

```
AT+SLESTOPSCAN\n
```

​	7）连接服务端

```
AT+SLECONN=0,112233445566\n
```

​	8）发现服务

```
AT+SSAPCFNDSTRU=1,0,1\n
```

##### 2.3通信测试

​	1）客户端向服务端发送消息

​	客户端向服务端写数据

```
AT+SSAPCWRITECMD=0,0,1,0,5,0x1122334455\n
```

​	服务端打印接收数据说明收发正常![image-20241119102853682](..\Image\HH-D03\SLEServerData.png)

​	2）服务端向客户端发送消息

​	服务端发送通知

```
AT+SSAPSSNDNTFY=0,1,0,2,0xAA55\n
```

​	客户端读取服务端属性数据

```
AT+SSAPCREADREQ=0,0,1,0\n
```

​	客户端打印接收数据说明收发正常。![image-20241119103004273](..\Image\HH-D03\SLEClientData.png)

