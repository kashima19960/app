#ifndef WIFI_DEMO_H
#define WIFI_DEMO_H
#include "cmsis_os2.h"
#include "hi_wifi_api.h"
#include "ohos_init.h"
#include <stdio.h>
#include <unistd.h>
int wifi_connect(void);
int wifi_demo(void);
static int wifi_scan(void);
void wifi_event(const hi_wifi_event* event);
void wifi_interface(void);
#endif