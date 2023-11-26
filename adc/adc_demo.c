/*
0.功能实现逻辑:按下某个特定按键时，都有其独特的采样范围值，通过判断采样值就能得知用户按下了哪一个按键
而无需对电压进行判断

1.s2采样固定为120，电压固定为0.211，所以可以取采样范围为110~130,电压范围为0.200~0.220

2.按键1采样范围为325~335，电压范围0.575~0.590

3.按键2采样范围550~570，电压范围0.980~0.995
*/
#include "cmsis_os2.h"
#include "ohos_init.h"
#include <hi_adc.h>
#include <hi_gpio.h>
#include <hi_io.h>
#include <stdio.h>
#include <unistd.h>

typedef enum {
    KeyEventnone, //没有按下任何按键
    KeyEvent1, //按下了按键1
    KeyEvent2, //按下了按键2
    KeyEvent3, //按下按键s2

} keyevent;

void adc_demo_init(void)
{
    hi_gpio_init();
    hi_gpio_set_dir(HI_GPIO_IDX_9, HI_GPIO_DIR_OUT); //设置GPIO9为输出模式
    hi_io_set_func(HI_IO_NAME_GPIO_5, HI_IO_FUNC_GPIO_5_GPIO);
    hi_gpio_set_dir(HI_GPIO_IDX_5, HI_GPIO_DIR_IN);
}
keyevent GetKeyEvent(void)
{
    keyevent temp = KeyEventnone;
    hi_u16 data = 0;
    hi_adc_read(HI_ADC_CHANNEL_2, &data, HI_ADC_EQU_MODEL_1, HI_ADC_CUR_BAIS_DEFAULT, 0);
    if (data >= 110 && data <= 130) {
        temp = KeyEvent3;
    } else if (data >= 325 && data <= 335) {
        temp = KeyEvent1;
    } else if (data >= 550 && data <= 570) {
        temp = KeyEvent2;
    } else {
        temp = KeyEventnone;
    }
    return temp;
}
void adc_interface(void)
{
    printf("start adc test!\n");
    adc_demo_init();
    for (;;) {
        keyevent key = GetKeyEvent();
        switch (key) {
        case KeyEventnone:
            break;
        case KeyEvent1:
            hi_gpio_set_ouput_val(HI_GPIO_IDX_9, HI_GPIO_VALUE0); //输出低电平
            printf("点灯大师！\n");
            break;
        case KeyEvent2:
            hi_gpio_set_ouput_val(HI_GPIO_IDX_9, HI_GPIO_VALUE1); //输出高电平
            printf("啊? 灯灭了\n");
            break;
        case KeyEvent3:
            printf("啥都没有发生呢^_^\n");
            break;
        }
        usleep(200000);
    }
}
void adc_demo(void)
{
    osThreadAttr_t attr;
    attr.name = "KeyTask";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = 2048;
    attr.priority = 26;
    if (osThreadNew((osThreadFunc_t)adc_interface, NULL, &attr) == NULL) {
        printf("[key_demo] Falied to create KeyTask!\n");
    }
}
SYS_RUN(adc_demo);