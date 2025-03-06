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

#ifndef WIFI_STARTER_H
#define WIFI_STARTER_H

#include "errcode.h"
#include "wifi_device_config.h"

errcode_t StartHotspot(char *ssid, char *pre_shared_key, wifi_security_enum security_type, uint8_t channel_num);


#endif  