#include "iic_demo.h"
#include "../ssd1306/ssd1306.h"
#include "cmsis_os2.h"
#include "hi_gpio.h"
#include "hi_i2c.h"
#include "hi_io.h"
#include "ohos_init.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void DrawWord(void)
{
    const uint8_t width = 12, height = 12, stride = 16;
    uint8_t word[][24] = {
        { /*-- ID:0,字符:"原",ASCII编码:D4AD,对应字:宽x高=12x12,画布:宽W=16 高H=12,共24字节*/
            0x00, 0x00, 0x3F, 0xF0, 0x22, 0x00, 0x2F, 0xE0, 0x28, 0x20, 0x2F, 0xE0, 0x28, 0x20, 0x2F, 0xE0,
            0x45, 0x40, 0x49, 0x20, 0x95, 0x10, 0x82, 0x00 },
        { /*-- ID:1,字符:"神",ASCII编码:C9F1,对应字:宽x高=12x12,画布:宽W=16 高H=12,共24字节*/
            0x20, 0x80, 0x10, 0xA0, 0xF7, 0xF0, 0x14, 0xA0, 0x27, 0xE0, 0x34, 0xA0, 0x6F, 0xE0, 0xA4, 0xA0,
            0x20, 0x80, 0x20, 0x80, 0x20, 0x80, 0x20, 0x80 },
        { /*-- ID:2,字符:"启",ASCII编码:C6F4,对应字:宽x高=12x12,画布:宽W=16 高H=12,共24字节*/
            0x04, 0x00, 0x02, 0x00, 0x3F, 0xE0, 0x20, 0x20, 0x3F, 0xE0, 0x20, 0x00, 0x20, 0x00, 0x2F, 0xE0,
            0x48, 0x20, 0x48, 0x20, 0x8F, 0xE0, 0x08, 0x20 },
        { /*-- ID:3,字符:"动",ASCII编码:B6AF,对应字:宽x高=12x12,画布:宽W=16 高H=12,共24字节*/
            0x00, 0x80, 0x78, 0x80, 0x00, 0x80, 0x03, 0xF0, 0xFC, 0x90, 0x10, 0x90, 0x20, 0x90, 0x49, 0x10,
            0xFD, 0x10, 0x42, 0x10, 0x04, 0x50, 0x18, 0x20 },
        { /*-- ID:4,字符:"！",ASCII编码:A3A1,对应字:宽x高=12x12,画布:宽W=16 高H=12,共24字节*/
            0x00, 0x00, 0x18, 0x00, 0x3C, 0x00, 0x3C, 0x00, 0x3C, 0x00, 0x18, 0x00, 0x18, 0x00, 0x00, 0x00,
            0x18, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00 }
    };
    ssd1306_Fill(Black);
    for (size_t i = 0; i < sizeof(word) / sizeof(word[0]); i++) {
        ssd1306_DrawRegion(i * width + 12, 2 * height, width, height, word[i], sizeof(word[0]), stride);
    }
}
void iic_interface(void* arg)
{
    (void)arg;
    hi_gpio_init();
    hi_io_set_func(HI_IO_NAME_GPIO_13, HI_IO_FUNC_GPIO_13_I2C0_SDA);
    hi_io_set_func(HI_IO_NAME_GPIO_14, HI_IO_FUNC_GPIO_14_I2C0_SCL);
    hi_i2c_init(HI_I2C_IDX_0, 400000);
    ssd1306_Init();
    ssd1306_Fill(Black);
    while (1) {
        DrawWord();
        usleep(10000);
        ssd1306_DrawRectangle(0, 0, SSD1306_WIDTH - 5, SSD1306_HEIGHT - 5, White);
        ssd1306_UpdateScreen();
        printf("hello\n");
    }
}
void iic_demo(void)
{
    osThreadAttr_t attr;
    attr.name = "iic_interface";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = 10240;
    attr.priority = osPriorityNormal;
    if (osThreadNew(iic_interface, NULL, &attr) == NULL) {
        printf("[iic_interface] Falied to create iic_interface!\n");
    }
}
APP_FEATURE_INIT(iic_demo);