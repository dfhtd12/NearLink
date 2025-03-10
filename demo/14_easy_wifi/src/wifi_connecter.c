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
#include "lwip/nettool/misc.h"
#include "soc_osal.h"
#include "errcode.h"
#include "wifi_device.h"
#include "wifi_hotspot.h"
#include "wifi_hotspot_config.h"
#include "wifi_linked_info.h"
#include "wifi_device_config.h"
#include "td_base.h"
#include "td_type.h"
#include "wifi_event.h"

#define WIFI_IFNAME_MAX_SIZE             64
#define WIFI_MAX_SSID_LEN                33
#define WIFI_SCAN_AP_LIMIT               64
#define WIFI_MAC_LEN                     6
#define WIFI_NOT_AVALLIABLE              0
#define WIFI_AVALIABE                    1
#define WIFI_GET_IP_MAX_COUNT            300
#define WIFI_CONN_STATUS_MAX_GET_TIMES 10   /* 获取连接信息最大尝试次数 */
#define DHCP_BOUND_STATUS_MAX_GET_TIMES 10 /* 获取绑定状态最大尝试次数 */
#define WIFI_STA_IP_MAX_GET_TIMES 10        /* 获取IP最大尝试次数 */
#define WIFI_STA_SAMPLE_LOG              "[WIFI_STA_SAMPLE]"
int a = 1;
enum {
    WIFI_STA_SAMPLE_INIT = 0,       /* 0:初始态 */
    WIFI_STA_SAMPLE_SCANING,        /* 1:扫描中 */
    WIFI_STA_SAMPLE_SCAN_DONE,      /* 2:扫描完成 */
    WIFI_STA_SAMPLE_FOUND_TARGET,   /* 3:匹配到目标AP */
    WIFI_STA_SAMPLE_CONNECTING,     /* 4:连接中 */
    WIFI_STA_SAMPLE_CONNECT_DONE,   /* 5:关联成功 */
    WIFI_STA_SAMPLE_GET_IP,         /* 6:获取IP */
} wifi_state_enum;

struct netif * g_iface = NULL;
static uint8_t g_wifi_state = WIFI_STA_SAMPLE_INIT;
static td_void wifi_scan_state_changed(td_s32 state, td_s32 size);
static td_void wifi_connection_changed(td_s32 state, const wifi_linked_info_stru *info, td_s32 reason_code);

wifi_event_stru wifi_event_cb = {
    .wifi_event_connection_changed      = wifi_connection_changed,
    .wifi_event_scan_state_changed      = wifi_scan_state_changed,
};




/*****************************************************************************
  STA 扫描事件回调函数
*****************************************************************************/
static td_void wifi_scan_state_changed(td_s32 state, td_s32 size)
{
    (void)state;
    (void)size;
    printf("%s Scan done!.\r\n", WIFI_STA_SAMPLE_LOG);
    g_wifi_state = WIFI_STA_SAMPLE_SCAN_DONE;
    return;
}

/*****************************************************************************
  STA 关联事件回调函数
*****************************************************************************/
static td_void wifi_connection_changed(td_s32 state, const wifi_linked_info_stru *info, td_s32 reason_code)
{
    (void)info;
    (void)reason_code;

    if (state == WIFI_NOT_AVALLIABLE) {
        printf("%s Connect fail!. try agin !\r\n", WIFI_STA_SAMPLE_LOG);
        g_wifi_state = WIFI_STA_SAMPLE_INIT;
    } else {
        printf("%s Connect succ!.\r\n", WIFI_STA_SAMPLE_LOG);
        g_wifi_state = WIFI_STA_SAMPLE_CONNECT_DONE;
    }
}

void DisconnectWithHotspot()
{
    if (g_iface) {
        err_t ret = netifapi_dhcp_stop(g_iface);
        printf("netifapi_dhcp_stop: %d\r\n", ret);
    }

    int errCode = wifi_sta_disconnect(); // disconnect with your AP
    printf("Disconnect: %d\r\n", errCode);

}

/*STAl链接目标AP
    param1:待连接的网络名称
    param2:待连接的网络接入密码
    param3:wifi sta配置结构体
*/
errcode_t get_match_network(char *expected_ssid,
                            char *key,
                            wifi_sta_config_stru *expected_bss)
{
    uint32_t num = WIFI_IFNAME_MAX_SIZE; /* 64:可以扫描到的Wi-Fi网络数量 */
    uint32_t bss_index = 0;

    /* 获取扫描结果 */
    uint32_t scan_len = sizeof(wifi_scan_info_stru) * num;
    wifi_scan_info_stru *result = osal_kmalloc(scan_len, OSAL_GFP_ATOMIC);
    if (result == NULL)
    {
        return ERRCODE_MALLOC;
    }
    memset_s(result, scan_len, 0, scan_len);
    if (wifi_sta_get_scan_info(result, &num) != ERRCODE_SUCC)
    {
        osal_kfree(result);
        return ERRCODE_FAIL;
    }

    /* 筛选扫描到的Wi-Fi网络，选择待连接的网络 */
    for (bss_index = 0; bss_index < num; bss_index++)
    {
        if (strlen(expected_ssid) == strlen(result[bss_index].ssid))
        {
            if (memcmp(expected_ssid, result[bss_index].ssid, strlen(expected_ssid)) == 0)
            {
                break;
            }
        }
    }

    /* 未找到待连接AP,可以继续尝试扫描或者退出 */
    if (bss_index >= num)
    {
        osal_kfree(result);
        return ERRCODE_FAIL;
    }

    /* 找到网络后复制网络信息和接入密码 */
    if (memcpy_s(expected_bss->ssid, WIFI_MAX_SSID_LEN, result[bss_index].ssid, WIFI_MAX_SSID_LEN) != EOK)
    {
        osal_kfree(result);
        return ERRCODE_MEMCPY;
    }
    if (memcpy_s(expected_bss->bssid, WIFI_MAC_LEN, result[bss_index].bssid, WIFI_MAC_LEN) != EOK)
    {
        osal_kfree(result);
        return ERRCODE_MEMCPY;
    }
    expected_bss->security_type = result[bss_index].security_type;
    if (memcpy_s(expected_bss->pre_shared_key, WIFI_MAX_KEY_LEN, key, strlen(key)) != EOK)
    {
        osal_kfree(result);
        return ERRCODE_MEMCPY;
    }
    expected_bss->ip_type = DHCP; /* IP类型为动态DHCP获取 */

    osal_kfree(result);
    return ERRCODE_SUCC;
}

errcode_t ConnectToHotspot(char *ssid, char *key)
{
    /*WiFi sta 网络设备名*/
    char ifname[] = "wlan0";
    wifi_sta_config_stru expected_bss = {0};
    wifi_linked_info_stru wifi_status;
    
    while (wifi_sta_enable() != ERRCODE_SUCC)
    {
        printf("STA enable fail, retry...\r\n");
        osal_msleep(100);
    }

    while (a)
    {
        /* start scan*/
        osal_msleep(1000);
        printf("Start Scan !\r\n");
        if (wifi_sta_scan() != ERRCODE_SUCC)
        {
            printf("STA scan fail, try again !\r\n");
            continue;
        }
        if(wifi_register_event_cb(&wifi_event_cb) != ERRCODE_SUCC){
            printf("Register wifievent fail\r\n");
            continue;
        }
        /* wait scan finish*/
        osal_msleep(2000);
        if (get_match_network(ssid, key, &expected_bss) != ERRCODE_SUCC)
        {
            printf("Can not find AP, try again !\r\n");
            continue;
        }

        printf("STA try connect.\r\n");
       
        if (wifi_sta_connect(&expected_bss) != ERRCODE_SUCC)
        {
            printf("connectc fail.\r\n");
            continue;
        }

      
        uint8_t index = 0;
        for (index = 0; index < WIFI_CONN_STATUS_MAX_GET_TIMES; index++)
        {
            osal_msleep(500); 
            memset_s(&wifi_status, sizeof(wifi_linked_info_stru), 0, sizeof(wifi_linked_info_stru));
            if (wifi_sta_get_ap_info(&wifi_status) != ERRCODE_SUCC)
            {
                continue;
            }
            if (wifi_status.conn_state == WIFI_CONNECTED)
            {
                break;
            }
        }

        if (wifi_status.conn_state == WIFI_CONNECTED)
        {
            break; 
        }
    }

    /* get DHCP ip */
    printf("STA DHCP start.\r\n");

     g_iface = netifapi_netif_find(ifname);
    if ( g_iface == NULL)
    {
        return ERRCODE_FAIL;
    }

    // start DHCP server
    if (netifapi_dhcp_start( g_iface) != ERR_OK)
    {
        printf("STA DHCP Fail.\r\n");
        return ERRCODE_FAIL;
    }

    // wait bond
    for (uint8_t i = 0; i < DHCP_BOUND_STATUS_MAX_GET_TIMES; i++)
    {
        osal_msleep(500); 
        if (netifapi_dhcp_is_bound( g_iface) == ERR_OK)
        {
            printf("STA DHCP bound success.\r\n");
            break;
        }
    }

    // wait DHCP server give me IP
    for (uint8_t i = 0; i < WIFI_STA_IP_MAX_GET_TIMES; i++)
    {
        osal_msleep(10); 
        if ( g_iface->ip_addr.u_addr.ip4.addr != 0)
        {
            printf("STA IP %u.%u.%u.%u\r\n", 
                   ( g_iface->ip_addr.u_addr.ip4.addr & 0x000000ff),
                   ( g_iface->ip_addr.u_addr.ip4.addr & 0x0000ff00) >> 8,
                   ( g_iface->ip_addr.u_addr.ip4.addr & 0x00ff0000) >> 16,
                   ( g_iface->ip_addr.u_addr.ip4.addr & 0xff000000) >> 24);
            netifapi_netif_common( g_iface, dhcp_clients_info_show, NULL);
            if (netifapi_dhcp_start( g_iface) != 0)
            {
                printf("STA DHCP Fail.\r\n");
                return ERRCODE_FAIL;
            }

            /* connect sta success */
            printf("Connect success.\r\n");
            return ERRCODE_SUCC;
        }
    }

    printf("STA connect fail.\r\n");
    return ERRCODE_FAIL;
}

