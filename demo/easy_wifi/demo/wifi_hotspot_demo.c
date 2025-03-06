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

#include <stdio.h>    
#include <string.h>    
#include "ohos_init.h" 
#include "cmsis_os2.h" 
#include "wifi_starter.h" 


#define SSID "HiSpark_AP"
#define KEY "123456789"
#define WIFI_SEC_TYPE_PSK 2
#define CHANNEL 7

static void WifiHotspotTask(void)
{

    if (StartHotspot(SSID, KEY, WIFI_SEC_TYPE_PSK, CHANNEL) != 0)
    {
       printf("StartHotspot failed!\n");
    }
    else
    {
       printf("StartHotspot success!\n");
    }
}


static void WifiHotspotDemo(void)
{
 
    osThreadAttr_t attr;
    attr.name = "WifiHotspotTask";
    attr.stack_size = 0x1000;
    attr.priority = osPriorityNormal;

   
    if (osThreadNew(WifiHotspotTask, NULL, &attr) == NULL)
    {
       printf("[WifiHotspotDemo] Falied to create WifiHotspotTask!\n");
    }
}


SYS_RUN(WifiHotspotDemo);
