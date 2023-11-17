/*
简要说明:
1.实现点灯的例程
2.延时时长的单位为微秒us
3.HI3861的LED为GPIO9
4.无线程创建
5.GPIO设置为输出模式
*/
#include <stdio.h>
#include "ohos_init.h"
#include "iot_gpio.h"
#include "hi_gpio.h"
#include <unistd.h>
#define DELAYTIME 800000
void led_interface(void)
{
    hi_gpio_init();
    hi_gpio_set_dir(HI_GPIO_IDX_9, HI_GPIO_DIR_OUT);
    for (;;)
    {
        hi_gpio_set_ouput_val(HI_GPIO_IDX_9, HI_GPIO_VALUE0);
        printf("led turn on!\n");
        usleep(DELAYTIME);
        hi_gpio_set_ouput_val(HI_GPIO_IDX_9, HI_GPIO_VALUE1);
        printf("led turn down!\n");
        usleep(DELAYTIME);
    }
}

SYS_RUN(led_interface);