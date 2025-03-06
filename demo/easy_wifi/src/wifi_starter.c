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


errcode_t StartHotspot(char *ssid, char *pre_shared_key, wifi_security_enum security_type, uint8_t channel_num)
{
    //WiFi SoftAP 网络设备名
    char ifname[] = "ap0";

    // 设置IP地址、子网掩码、网关地址
    ip4_addr_t st_ipaddr = {0};
    ip4_addr_t st_netmask = {0};
    ip4_addr_t st_gw = {0};
    IP4_ADDR(&st_ipaddr, 192, 168, 43, 1); /* IP地址设置：192.168.43.1 */
    IP4_ADDR(&st_netmask, 255, 255, 255, 0); /* 子网掩码设置：255.255.255.0 */
    IP4_ADDR(&st_gw, 192, 168, 43, 2); /* 网关地址设置：192.168.43.2 */

    // Hotspot的配置
    softap_config_stru hapd_conf = {0};
    memcpy_s(hapd_conf.ssid, sizeof(hapd_conf.ssid), ssid, strlen(ssid));                   /* SSID */
    memcpy_s(hapd_conf.pre_shared_key, WIFI_MAX_KEY_LEN, pre_shared_key, WIFI_MAX_KEY_LEN); /* 密码 */
    hapd_conf.security_type = security_type;                                                // WiFi安全类型
    hapd_conf.channel_num = channel_num;                                                    /* 工作信道 */

    // 启动Hotspot
    printf("Hotspot try enable.\r\n");
    while (wifi_softap_enable(&hapd_conf) != ERRCODE_SUCC)
    {
        printf("Hotspot enable fail, retry...\r\n");
        osal_msleep(100);
    }

    //配置DHCP服务器
    struct netif *netif_p = NULL;
    netif_p = netif_find(ifname);
    if (netif_p == NULL)
    {
        printf("find netif fail.\r\n", ifname);
        wifi_softap_disable();
        return ERRCODE_FAIL;
    }
    if (netifapi_netif_set_addr(netif_p, &st_ipaddr, &st_netmask, &st_gw) != ERR_OK)
    {
        printf("set addr fail.\r\n");
        wifi_softap_disable();
        return ERRCODE_FAIL;
    }
    if (netifapi_dhcps_start(netif_p, NULL, 0) != ERR_OK)
    {
        printf("dhcps start fail.\r\n");
        wifi_softap_disable();
        return ERRCODE_FAIL;
    }

    printf("SoftAp start success.\r\n");
    return ERRCODE_SUCC;
}
