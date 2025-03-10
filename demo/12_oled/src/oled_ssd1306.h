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
#ifndef OLED_SSD1306_H
#define OLED_SSD1306_H
#include <stdint.h>   
// 字库类型
typedef enum Font {
    FONT6x8 = 1,
    FONT8x16
}Font;
uint32_t OledInit(void);
void OledSetPosition(uint8_t x, uint8_t y);
void OledFillScreen(uint8_t fillData);
//uint32_t WriteData(uint8_t data);

//显示字符
void OledShowChar(uint8_t x, uint8_t y, uint8_t ch, Font font);
//显示字符串
void OledShowString(uint8_t x, uint8_t y, const char* str, Font font);


#endif 
