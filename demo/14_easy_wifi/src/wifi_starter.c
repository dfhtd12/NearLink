 /*
 Copyright (C) 2024 HiHope Open Source Organization .
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

#include "stdlib.h"
#include "ohos_init.h" 
#include "cmsis_os2.h" 
#include "lwip/netifapi.h"
#include "soc_osal.h"
#include "errcode.h"
#include "wifi_hotspot.h"
#include "wifi_hotspot_config.h"
#include "wifi_linked_info.h"
#include "wifi_device_config.h"
#include "td_base.h"
#include "td_type.h"

#define NUM1 255
#define NUM2 0

errcode_t StartHotspot(char *ssid, char *key, wifi_security_enum security_type, uint8_t channel_num)
{
    //WiFi SoftAP 网络设备名
    char ifname[] = "ap0";

    /* SoftAp接口的信息 */
    ip4_addr_t st_ipaddr = {0};
    ip4_addr_t st_netmask = {0};
    ip4_addr_t st_gw = {0};
    softap_config_stru hapd_conf = {0};
    //softap_config_advance_stru config = {0};
    struct netif *netif_p = TD_NULL;
    
    IP4_ADDR(&st_ipaddr, 192, 168, 43, 1); /* IP地址设置：192.168.43.1 */
    IP4_ADDR(&st_netmask, NUM1, NUM1, NUM1, NUM2); 
    IP4_ADDR(&st_gw, 192, 168, 43, 2); /* 网关地址设置：192.168.43.2 */

    /* 配置SoftAp基本参数 */
    memcpy_s(hapd_conf.ssid, sizeof(hapd_conf.ssid), ssid, strlen(ssid));                   /* SSID */
    memcpy_s(hapd_conf.pre_shared_key, WIFI_MAX_KEY_LEN, key, WIFI_MAX_KEY_LEN); /* 密码 */
    hapd_conf.security_type = security_type;                                                /*  加密方式*/
    hapd_conf.channel_num = channel_num; /* 工作信道 */
    //hapd_conf.wifi_psk_type = 0;     


    // /* 配置SoftAp网络参数 */
    // config.beacon_interval = 100; /* 100：Beacon周期设置为100ms */
    // config.dtim_period = 2; /* 2：DTIM周期设置为2 */
    // config.gi = 0; /* 0：short GI默认关闭 */
    // config.group_rekey = 86400; /* 86400：组播秘钥更新时间设置为1天 */
    // config.protocol_mode = 4; /* 4：协议类型设置为802.11b + 802.11g + 802.11n + 802.11ax */
    // config.hidden_ssid_flag = 1; /* 1：不隐藏SSID */


    // if (wifi_set_softap_config_advance(&config) != 0) {
    //     return -1;
    // }                                              

    /* 启动SoftAp接口 */
    while (wifi_softap_enable(&hapd_conf) != ERRCODE_SUCC){
        printf("Hotspot enable fail\r\n");
        osal_msleep(100);
    }
     /* 配置DHCP服务器 */
    netif_p = netif_find(ifname);
    if (netif_p == TD_NULL)
    {
        printf("dhcp find netif fail.\r\n", ifname);
        wifi_softap_disable();
        return ERRCODE_FAIL;
    }
    if (netifapi_netif_set_addr(netif_p, &st_ipaddr, &st_netmask, &st_gw) != ERRCODE_SUCC)
    {
        printf("dhcp set addr fail.\r\n");
        wifi_softap_disable();
        return ERRCODE_FAIL;
    }
    if (netifapi_dhcps_start(netif_p, NULL, 0) != ERRCODE_SUCC)
    {
        printf("dhcp start fail.\r\n");
        wifi_softap_disable();
        return ERRCODE_FAIL;
    }

    printf("wifi init succ.\r\n");
    return ERRCODE_SUCC;
}
