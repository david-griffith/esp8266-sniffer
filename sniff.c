#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "os_type.h"
#include "user_config.h"
#include "user_interface.h"
#include "driver/uart.h"

#define user_procTaskPrio        0
#define user_procTaskQueueLen    1
#define CHANNEL_HOP_INTERVAL 200
os_event_t    user_procTaskQueue[user_procTaskQueueLen];
static volatile os_timer_t channelHop_timer;

static void loop(os_event_t *events);
static void promisc_cb(uint8 *buf, uint16 len);

// Aeroscout MAC destination header
uint8_t dest[3] = {0x01, 0x0c, 0xcc};


struct RxControl {
    signed rssi:8;
    unsigned rate:4;
    unsigned is_group:1;
    unsigned:1;
    unsigned sig_mode:2;
    unsigned legacy_length:12;
    unsigned damatch0:1;
    unsigned damatch1:1;
    unsigned bssidmatch0:1;
    unsigned bssidmatch1:1;
    unsigned MCS:7;
    unsigned CWB:1;
    unsigned HT_length:16;
    unsigned Smoothing:1;
    unsigned Not_Sounding:1;
    unsigned:1;
    unsigned Aggregation:1;
    unsigned STBC:2;
    unsigned FEC_CODING:1;
    unsigned SGI:1;
    unsigned rxend_state:8;
    unsigned ampdu_cnt:8;
    unsigned channel:4;
    unsigned:12;
};
 
struct LenSeq {
    uint16_t length;
    uint16_t seq;
    uint8_t  address3[6];
};

struct sniffer_buf {
    struct RxControl rx_ctrl;
    uint8_t buf[36];
    uint16_t cnt;
    struct LenSeq lenseq[1];
};

struct sniffer_buf2{
    struct RxControl rx_ctrl;
    uint8_t buf[112];
    uint16_t cnt;
    uint16_t len;
};

static void ICACHE_FLASH_ATTR
promisc_cb(uint8 *buf, uint16 len)
{
if (len == 60){
        struct sniffer_buf *sniffer = (struct sniffer_buf*) buf;
        int i=0;
        // Check MACs
        for (i=0; i<3; i++) {
	    if (sniffer->buf[i+4] != dest[i]) return;
	}
        os_printf("%02x:%02x:%02x:%02x:%02x:%02x,%d\n ",sniffer->buf[10], sniffer->buf[11], sniffer->buf[12], sniffer->buf[13], sniffer->buf[14], sniffer->buf[15], sniffer->rx_ctrl.rssi);
    }
}

//Main code function
static void ICACHE_FLASH_ATTR
loop(os_event_t *events)
{
    os_delay_us(10);
}

void ICACHE_FLASH_ATTR
sniffer_system_init_done(void)
{
//    os_printf("Enter last three digits of tag MAC address, or hit enter\n to detect all tags");
    wifi_set_channel(6);
//    os_printf(" -> Promisc mode setup, channel 6 ... ");
    wifi_promiscuous_enable(0);
    wifi_set_promiscuous_rx_cb(promisc_cb);
    wifi_promiscuous_enable(1);
}

//Init function
void ICACHE_FLASH_ATTR
user_init()
{
    uart_init(115200, 115200);
    os_delay_us(100);

//    uart0_sendStr("*** Monitor mode test ***\r\n");
//    os_printf(" -> Set opmode ... ");
    wifi_set_opmode( STATION_MODE );

//    os_printf("done.\n");
//    os_printf(" -> Timer setup ... ");
//    os_timer_disarm(&channelHop_timer);
//    os_timer_setfn(&channelHop_timer, (os_timer_func_t *) channelHop, NULL);
//    os_timer_arm(&channelHop_timer, CHANNEL_HOP_INTERVAL, 1);
//    os_printf("done.\n");

    system_init_done_cb(sniffer_system_init_done);
    
    //Start os task
    system_os_task(loop, user_procTaskPrio,user_procTaskQueue, user_procTaskQueueLen);

//    os_printf(" -> Init finished!\n\n");
}

