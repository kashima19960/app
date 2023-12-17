#include "pwm_demo.h"
void pwm_demo(void* arg)
{
    arg = (void*)arg;
    hi_gpio_init();
    //判断的标志
    uint8_t flag = 1;
    //先要将GPIO9(LED灯)复用成PWM0_OUT,官方demo没有进行复用，导致不会输出PWM方波(亲测)
    hi_io_set_func(HI_IO_NAME_GPIO_9, HI_IO_FUNC_GPIO_9_PWM0_OUT);
    hi_pwm_init(HI_PWM_PORT_PWM0);
    hi_pwm_set_clock(PWM_CLK_160M);
    //逐级改变pwm方波输出的占空比，从而实现呼吸灯的效果
    for (int i = 0;;) {
        //160000000就是160MHz转换成Hz的数值
        if (i >= 160000000 / PSC) {
            flag = 1;
        }
        if (i <= 0) {
            flag = 0;
        }
        if (flag == 0) {
            i += INCREATMENT;
        } else {
            i -= INCREATMENT;
        }
        hi_pwm_start(HI_PWM_PORT_PWM0, i, PSC);
        usleep(75000);
    }
}
void pwm_interface(void)
{
    osThreadAttr_t attr;
    attr.name = "pwm_demo";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = 2048;
    attr.priority = 26;

    if (osThreadNew((osThreadFunc_t)pwm_demo, NULL, &attr) == NULL) {
        printf("[pwm_demo] Falied to create pwm_demo!\n");
    }
}
SYS_RUN(pwm_interface);
