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

#ifndef BLE_UART_SERVER_ADV_H
#define BLE_UART_SERVER_ADV_H

#include <stdint.h>

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

uint8_t ble_uart_set_adv_data(void);
uint8_t ble_uart_start_adv(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
#endif