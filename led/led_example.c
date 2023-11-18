/*
简要说明:
1.实现点灯的例程
2.延时时长的单位为微秒us
3.HI3861的LED为GPIO9
4.在led_interface进程下,创建一个线程LedTask
5.GPIO设置为输出模式
*/
#include <stdio.h>
#include "ohos_init.h"
#include"cmsis_os2.h"
#include "iot_gpio.h"
#include "hi_gpio.h"
#include <unistd.h>
#define DELAYTIME 800000


void LedTask(void *arg)
{
    (void)arg;

    hi_gpio_init();                  //初始化GPIO
    hi_gpio_set_dir(HI_GPIO_IDX_9, HI_GPIO_DIR_OUT);      //设置为输出模式

    while(1)
    {
        hi_gpio_set_ouput_val(HI_GPIO_IDX_9, HI_GPIO_VALUE0);    //输出低电平
        printf("led turn on!\n");
        usleep(DELAYTIME);
        hi_gpio_set_ouput_val(HI_GPIO_IDX_9, HI_GPIO_VALUE1);    //输出高电平
        printf("led turn off!\n");
        usleep(DELAYTIME);
    }

}

/*
遇到问题:
1.一开始使用的栈的大小为512字节,发现烧录后会有栈溢出的问题,且LED灯不会闪烁,改成1024后成功解决.
2.使用iot_gpio.h文件里封装过的函数,不够专业且影响编译速度,后面改用hi_gpio.h文件里的函数.
*/

void led_interface(void)
{
    osThreadAttr_t attr;

    attr.name = "LedTask";      //线程名称
    attr.attr_bits = 0U;        //线程属性位
    attr.cb_mem = NULL;         //用户指定的控制块指针
    attr.cb_size = 0U;          //用户指定的控制块大小,单位:字节
    attr.stack_mem = NULL;      //用户指定的线程栈指针
    attr.stack_size = 1024;     //线程栈大小,单位:字节。
    attr.priority = 26;         //线程优先级

    if(osThreadNew((osThreadFunc_t)LedTask,NULL,&attr)==NULL)
    {
        printf("[led_example] Failed to create LedTask!\n");
    }
    
}

SYS_RUN(led_interface);  