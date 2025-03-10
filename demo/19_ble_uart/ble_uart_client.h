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

#ifndef BLE_UART_CLIENT_H
#define BLE_UART_CLIENT_H

#include <stdint.h>
#include "common_def.h"
#include "errcode.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

typedef enum {
BLE_PUBLIC_DEVICE_ADDRESS,
BLE_RANDOM_DEVICE_ADDRESS,
BLE_PUBLIC_IDENTITY_ADDRESS,
BLE_RANDOM_STATIC_IDENTITY_ADDRESS
} ble_address_t;

errcode_t ble_uart_client_discover_all_service(uint16_t conn_id);
errcode_t ble_uart_client_write_cmd(uint8_t *data, uint16_t len, uint16_t hand);
errcode_t ble_uart_client_init(void);
uint16_t ble_uart_get_write_vlaue_handle(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
#endif