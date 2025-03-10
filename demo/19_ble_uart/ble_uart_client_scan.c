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
#include "errcode.h"
#include "bts_def.h"
#include "bts_le_gap.h"
#include "ble_uart_client_scan.h"
#include "osal_debug.h"

static uint16_t g_uart_scan_interval = 0x48;
static uint16_t g_uart_scan_window = 0x48;
static uint8_t g_uart_scan_type = 0x00;
static uint8_t g_uart_scan_phy = 0x01;
static uint8_t g_uart_scan_filter_policy = 0x00;

errcode_t ble_uart_set_scan_parameters(void)
{
    errcode_t ret = ERRCODE_BT_SUCCESS;
    gap_ble_scan_params_t ble_uart_scan_params = { 0 };
    ble_uart_scan_params.scan_interval = g_uart_scan_interval;
    ble_uart_scan_params.scan_window = g_uart_scan_window;
    ble_uart_scan_params.scan_type = g_uart_scan_type;
    ble_uart_scan_params.scan_phy = g_uart_scan_phy;
    ble_uart_scan_params.scan_filter_policy = g_uart_scan_filter_policy;
    ret = gap_ble_set_scan_parameters(&ble_uart_scan_params);
    if (ret != ERRCODE_BT_SUCCESS) {
        printf("gap_ble_set_scan_parameters ret = %x\n", ret);
    }
    return ret;
}

errcode_t ble_uart_start_scan(void)
{
    return gap_ble_start_scan();
}