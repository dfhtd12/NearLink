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
#ifndef BLE_UART_SERVER_H
#define BLE_UART_SERVER_H

#include <stdint.h>
#include "errcode.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/* Service UUID */
#define BLE_UART_UUID_SERVER_SERVICE 0xABCD
/* Characteristic UUID */
#define BLE_UART_CHARACTERISTIC_UUID_TX 0xCDEF
/* Characteristic UUID */
#define BLE_UART_CHARACTERISTIC_UUID_RX 0xEFEF
/* Client Characteristic Configuration UUID */
#define BLE_UART_CLIENT_CHARACTERISTIC_CONFIGURATION 0x2902

void ble_uart_set_device_name_value(const uint8_t *name, const uint8_t len);
void ble_uart_set_device_appearance_value(uint16_t appearance);
void ble_uart_server_init(void);
errcode_t ble_uart_server_send_input_report(uint8_t *data, uint16_t len);
uint8_t ble_uart_get_connection_state(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif  /* __cplusplus */
#endif  /* __cplusplus */
#endif