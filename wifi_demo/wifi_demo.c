#include "wifi_demo.h"
//demo主函数
int wifi_demo(void)
{
    char ifname[WIFI_IFNAME_MAX_SIZE + 1] = { 0 };
    int len = sizeof(ifname);
    //启动WIFI的STA模式，就是让Hi3861能像手机一样连接WIFI
    if (hi_wifi_sta_start(ifname, &len) != HISI_OK) {
        printf("wifi initialize failed!\n");
        return -1;
    }

    //注册一个事件回调函数
    if (hi_wifi_register_event_callback(wifi_event) != HISI_OK) {
        printf("registe callback function failed!\n");
        return -1;
    }
    //开启WIFI扫描，并打印扫描结果
    wifi_scan();
    //连接WIFI
    wifi_connect();
    //死循环，只要WIFI连接不上就一直死等，不过因为单独创建了一个线程，所以不会浪费Soc的资源
    for (;;) {
        usleep(30000);
    }
}
//WIFI连接功能块
int wifi_connect(void)
{
    hi_wifi_assoc_request connect_info = { 0 };
    //ssid指的是WIFI的名字
    char ssid[] = "harmony";
    //key指的是WIFI的密码
    char key[] = "harmony@11";
    connect_info.auth = HI_WIFI_SECURITY_WPA2PSK;
    memcpy_s(connect_info.ssid, HI_WIFI_MAX_SSID_LEN + 1, ssid, sizeof(ssid));
    memcpy(connect_info.key, key, sizeof(key));

    if (hi_wifi_sta_connect(&connect_info) != HISI_OK) {
        printf("wifi connect failed!\n");
        return -1;
    }
    return 0;
}
//WIFI扫描功能块
static int wifi_scan(void)
{
    unsigned int ap_num = WIFI_SCAN_AP_LIMIT;
    hi_wifi_ap_info* ap_info = malloc(sizeof(hi_wifi_ap_info) * ap_num);

    if (ap_info == NULL) {
        return -1;
    }

    if (hi_wifi_sta_scan() != HISI_OK) {
        printf("wifi scan failed!\n");
        // puts("wifi scan failed!");用puts()打印信息，编译会出错，理由不明
        return -1;
    }
    //扫描需要时间，所以扫描结果要等一段时间
    sleep(1);
    //传进去的都是指针，所以扫描结果最终会保存在ap_info和ap_num中
    hi_wifi_sta_scan_results(ap_info, &ap_num);

    //打印扫描到的所有热点
    for (unsigned int i = 0; (i < ap_num) && (i < WIFI_SCAN_AP_LIMIT); i++) {
        //冷知识:WIFI信号强度可以是负数，这说明设备的发射功率不高
        printf("SSID: %s  channel: %d  signal strength: %d   ", ap_info[i].ssid, ap_info[i].channel, ap_info[i].rssi);
        printf("\n");
    }
    free(ap_info);
}
/*当事件发生时的处理方法，前提是调用hi_wifi_register_event_callback注册回调函数
如果不自定义事件处理函数，鸿蒙是会有默认的事件响应处理的
*/
void wifi_event(const hi_wifi_event* event)
{
    if (event == NULL) {
        return;
    }
    switch (event->event) {
    case HI_WIFI_EVT_SCAN_DONE:
        printf("扫描完成！\n");
        break;
    case HI_WIFI_EVT_CONNECTED:
        printf("WIFI连接成功\n");
        break;
    case HI_WIFI_EVT_DISCONNECTED:
        printf("WIFI断开连接\n");
        break;
    case HI_WIFI_EVT_WPS_TIMEOUT:
        printf("WIFI连接超时\n");
        break;
    default:
        break;
    }
}
//线程创建函数
void wifi_interface(void)
{
    osThreadAttr_t attr;
    attr.name = "wifi_demo";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = 2048;
    attr.priority = 26;
    if (osThreadNew((osThreadFunc_t)wifi_demo, NULL, &attr) == NULL) {
        printf("[wifi_demo] Falied to create wifi_demo!\n");
    }
}
APP_FEATURE_INIT(wifi_interface);