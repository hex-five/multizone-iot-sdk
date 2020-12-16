/* Copyright(C) 2020 Hex Five Security, Inc. - All Rights Reserved */

#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "lwip/dhcp.h"
#include "lwip/dns.h"
#include "lwip/init.h"
#include "lwip/netif.h"
#include "lwip/timeouts.h"
#include "netif/ethernet.h"
#include "lwip/apps/sntp.h"

#include "multizone.h"
#include "platform.h"
#include "xemaclite.h"
#include "queue.h"
#include "mqtt_config.h"
#include "mqtt_wrap.h"

// ----------------------------------------------------------------------------

/* lwIP system time (ms) - no date/time, just delta */
u32_t sys_now(void) {

	return (u32_t) (MZONE_RDTIME() * 1000 / RTC_FREQ);
}

/* mtime zero epoque (sec) - updated via sntp */
static time_t mtime_zero_epoque = 0;

/* set mtime zero epoque to avoid changes to mtime/mtimecmp (i.e. FreeRTOS tick) */
void sntp_set_system_time(uint32_t t){
	mtime_zero_epoque = t - (time_t) (MZONE_RDTIME() / RTC_FREQ);
}

/* mbedtls system time (sec) - actual date/time for X509 exp verification */
time_t time(time_t *timer) {

	time_t lt = mtime_zero_epoque + (time_t) (MZONE_RDTIME() / RTC_FREQ);

	if (timer != NULL) {

		*timer = lt;

	}

	return lt;

}

int mbedtls_platform_entropy_poll( void *data, unsigned char *output, size_t len, size_t *olen ) {

	for (size_t i=0; i<len; i++){

		output[i] = (unsigned char)rand();

	}

    *olen = len;

    return( 0 );
}

// ----------------------------------------------------------------------------

#define printf2(format, args...) { /* Multi-part printf() */ \
	char *str = malloc(64);									 \
	if (str != NULL){										 \
		const int len = snprintf(str, 64, format, ## args);	 \
		for (int i=0; i<(len/16)+1; i++){					 \
			while(!MZONE_SEND(2, (str+16*i))) MZONE_YIELD(); \
		}													 \
		free(str);											 \
	}														 \
}															 \

static struct netif netif;
static struct queue *rx_queue;

__attribute__((interrupt())) void trp_handler(void)  { // non maskable traps

	const unsigned long mcause = MZONE_CSRR(CSR_MCAUSE);
	const unsigned long mepc   = MZONE_CSRR(CSR_MEPC);
	const unsigned long mtval  = MZONE_CSRR(CSR_MTVAL);

/*	switch(mcause){

	case 0 : printf("Instruction address missaligned : 0x%08x 0x%08x 0x%08x \n", mcause, mepc, mtval);
			 break;

	case 1 : printf("Instruction access fault : 0x%08x 0x%08x 0x%08x \n", mcause, mepc, mtval);
			 break;

	case 2 : printf("Illegal instruction : 0x%08x 0x%08x 0x%08x \n", mcause, mepc, mtval);
			 break;

	case 3 : printf("Breakpoint : 0x%08x 0x%08x 0x%08x \n", mcause, mepc, mtval);
			 break;

	case 4 : printf("Load address missaligned : 0x%08x 0x%08x 0x%08x \n", mcause, mepc, mtval);
			 break;

	case 5 : printf("Load access fault : 0x%08x 0x%08x 0x%08x \n", mcause, mepc, mtval);
			 break;

	case 6 : printf("Store/AMO address missaligned : 0x%08x 0x%08x 0x%08x \n", mcause, mepc, mtval);
	 	 	 break;

	case 7 : printf("Store access fault : 0x%08x 0x%08x 0x%08x \n", mcause, mepc, mtval);
			 break;

	case 8 : printf("Environment call from U-mode : 0x%08x 0x%08x 0x%08x \n", mcause, mepc, mtval);
			 break;

	case 9 : printf("Environment call from S-mode : 0x%08x 0x%08x 0x%08x \n", mcause, mepc, mtval);
			 break;

	case 11: printf("Environment call from M-mode : 0x%08x 0x%08x 0x%08x \n", mcause, mepc, mtval);
			 break;

	default : printf("Exception : 0x%08x 0x%08x 0x%08x \n", mcause, mepc, mtval);

	}*/

	asm("1: j 1b"); // TBD restart this zone?

}
__attribute__((interrupt())) void msi_handler(void)  { // machine software interrupt (3)
	asm volatile("ebreak");
}
__attribute__((interrupt())) void tmr_handler(void)  { // machine timer interrupt (7)
	 MZONE_WRTIMECMP((uint64_t)-1); // clear mip.7
}
__attribute__((interrupt())) void plic_handler(void) { // machine external interrupt (11)

	const uint32_t plic_int = PLIC_REG(PLIC_CLAIM_OFFSET); // PLIC claim

	struct pbuf *p;
	// check both rx buffers as they share the same irq
	while ( (p = xemaclite_input(&netif)) !=NULL){
		if (!queue_is_full(rx_queue))
			queue_put(rx_queue, p);
		else {
			pbuf_free(p);
		}
	}

	PLIC_REG(PLIC_CLAIM_OFFSET) = plic_int; // PLIC complete

}

static int msg_handler(const int zone, const char *msg){

    // consumed messages are processed locally and not forwarded to the broker
    int consumed = 0;

    if (strcmp("ping", msg)==0){
        MZONE_SEND(zone, "pong");
        consumed = 1;

    } else if (strcmp("broker hello", msg)==0){
        MZONE_SEND(zone, "online");

    }

    return consumed;

}

static void netif_link_callback(struct netif *netif) {

	printf2("netif_link_callback: %s\n", netif_is_link_up(netif) ? "up" : "down");

}

static void netif_status_callback(struct netif *netif){

	printf2("netif_status_callback: address %s\n", ipaddr_ntoa(netif_ip4_addr(netif)));

/*	printf("netif_status_callback:\n");
	printf("   address %s\n", ipaddr_ntoa(netif_ip4_addr(netif)));
	printf("   netmask %s\n", ipaddr_ntoa(netif_ip4_netmask(netif)));
	printf("   gateway %s\n", ipaddr_ntoa(netif_ip4_gw(netif)));
    printf("     dns 1 %s\n", ipaddr_ntoa(dns_getserver(0)));
    printf("     dns 2 %s\n", ipaddr_ntoa(dns_getserver(1)));
*/
}

static void dns_callback(const char *name, const ip_addr_t *addr, void *arg ){

	printf2("dns_callback: %s %s\n", name, addr ? ipaddr_ntoa(addr) : "<not found>");

	ip_addr_copy(*(ip_addr_t *)arg, *addr);

}

static void lwip_thread(){

	/* Check for received frames, feed them to lwIP */
	while (1){

		CSRC(mie, 1<<11);
		struct pbuf* p = queue_get(rx_queue);
		CSRS(mie, 1<<11);

			if(p != NULL) {

				if(netif.input(p, &netif) != ERR_OK)
					pbuf_free(p);

			} else

				break;

	}

	/* lwIP background tasks ( 250ms, 500ms) */
	sys_check_timeouts();

	/* set MultiZone timer and go to sleep */
	const unsigned int sleeptime = sys_timeouts_sleeptime();
	//printf2(">>> sys_timeouts_sleeptime() %d \n", sleeptime);
	if (sleeptime != SYS_TIMEOUTS_SLEEPTIME_INFINITE)
		MZONE_ADTIMECMP( (uint64_t)(sleeptime * RTC_FREQ/1000));

}

int main(void) {

	//while(1) MZONE_WFI();
	//while(1) MZONE_YIELD();
	//while(1);

	// vectored trap handler
	static __attribute__ ((aligned(4)))void (*trap_vect[32])(void) = {};
	trap_vect[0]  = trp_handler;
	trap_vect[3]  = msi_handler;
	trap_vect[7]  = tmr_handler;
	trap_vect[11] = plic_handler;
	CSRW(mtvec, trap_vect);	CSRS(mtvec, 0x1);

    CSRS(mie, 1<<11); 		// enable external interrupts (PLIC)
    CSRS(mie, 1<<7); 		// enable timer (TMR)
    CSRS(mstatus, 1<<3);	// enable global interrupts (PLIC, TMR)

	// Enable XEMACLITE RX IRQ (PLIC Priority 1=lowest 7=highest)
	PLIC_REG(PLIC_PRI_OFFSET + (PLIC_XEMAC_RX_SOURCE << PLIC_PRI_SHIFT_PER_SOURCE)) = 1;
	PLIC_REG(PLIC_EN_OFFSET) |= 1 << PLIC_XEMAC_RX_SOURCE;

	/* IP Stack Init */
	lwip_init();

	/* init ethernet interface */
	rx_queue = queue_new(16);

#if defined(IP_ADDR) && defined(IP_NETMASK) && defined(IP_GATEWAY) && defined(IP_DNS_1) && defined(IP_DNS_2)
	ip4_addr_t addr; ip4_addr_set_u32(&addr, ipaddr_addr(IP_ADDR));
	ip4_addr_t netmask; ip4_addr_set_u32(&netmask, ipaddr_addr(IP_NETMASK));
	ip4_addr_t gw; ip4_addr_set_u32(&gw, ipaddr_addr(IP_GATEWAY));
    ip4_addr_t dsn1; ip4_addr_set_u32(&dsn1, ipaddr_addr(IP_DNS_1)); dns_setserver(0, &dsn1);
    ip4_addr_t dsn2; ip4_addr_set_u32(&dsn2, ipaddr_addr(IP_DNS_2)); dns_setserver(1, &dsn2);
	netif_add(&netif, &addr, &netmask, &gw, NULL, xemaclite_init, ethernet_input);
#else
    netif_add(&netif, IP4_ADDR_ANY, IP4_ADDR_ANY, IP4_ADDR_ANY, NULL, xemaclite_init, ethernet_input);
#endif

	netif_set_default(&netif);
	netif_set_up(&netif);
	netif_set_status_callback(&netif, netif_status_callback);
	netif_set_link_callback(&netif, netif_link_callback);

	// Poll ethernet link
	while(!netif_is_link_up(&netif)){
		xemaclite_poll_link_state(&netif);
		lwip_thread();
		MZONE_WFI();
	}

#if !(defined(IP_ADDR) && defined(IP_NETMASK) && defined(IP_GATEWAY) && defined(IP_DNS_1) && defined(IP_DNS_2))
	// Wait for dhcp settings
	dhcp_start(&netif);
	while (!dhcp_supplied_address(&netif)){
		lwip_thread();
		MZONE_WFI();
	}
#endif

	// Resolve broker address
	ip_addr_t broker_ip = IPADDR4_INIT(IPADDR_ANY); // (54,176,2,35) (192,168,0,101);
	dns_gethostbyname(MQTT_BROKER_FQN, &broker_ip, dns_callback, &broker_ip);
	while (ip_addr_isany_val(broker_ip)){
		lwip_thread();
		MZONE_WFI();
	}

	// Get network time & set RTC (used for TLS enthropy & cert exp verification)
	// RTC is set by the callback SNTP_SET_SYSTEM_TIME(sec) defined in lwipopts.h
	sntp_setservername(0, SNTP_SERVER_0);
	sntp_setservername(1, SNTP_SERVER_1);
	sntp_init();
	while (time(NULL) < 1595137568){ // Sunday, July 19, 2020 5:46:08 AM
		lwip_thread();
		MZONE_WFI();
	} sntp_stop();
	const time_t ut = time(NULL);
	printf2( "sntp_process: %lu %s", (unsigned long)ut, ctime(&ut));

	// At this point there should be enough variance to seed the RNG
	srand( (unsigned)MZONE_CSRR(CSR_MCYCLE) ^ (unsigned)time(NULL) );
	//printf2("rand: 0x%08x \n", rand());

#ifdef MQTT_CLIENT_ID
    const char client_id[] = MQTT_CLIENT_ID;
#else
    char client_id[14+1] = "mzone-00000000";
	char *rand_str = malloc(8+1);
		itoa(rand(), rand_str, 16);
		strcpy(client_id+14-strlen(rand_str), rand_str);
	free(rand_str);
#endif
	printf2("client_id: %s \n", client_id);

	// Init mqtt gateway
	printf2("mqtt: connecting ... \n");
	mqtt_init(broker_ip, client_id);
	mqtt_connect();
	while (!mqtt_is_mqtt_connected()){
		lwip_thread();
		MZONE_WFI();
	}
	printf2("mqtt: connected \n");

	// Main loop (link is up, dhcp bound, broker addr resolved)
	while (1) {

        MZONE_WFI(); // resume upon 1) timer exp 2) eth irq 3) incoming msg

		lwip_thread();

		xemaclite_poll_link_state(&netif);

		if (netif_is_link_up(&netif)){

			// Link
			if (mqtt_is_tcp_disconnected())
				mqtt_connect();

			// Message Router
			typedef enum {zone1=1, zone2, zone3, zone4} Zone;

			if (mqtt_is_mqtt_connected()){

				// inbound: broker => zone1 => zones

				for (Zone zone = zone1; zone<=zone4; zone++){

					if (strlen(mqtt_get_msg(zone-1))){

						if (zone==zone1) {
                            msg_handler(zone, mqtt_get_msg(zone-1));
							mqtt_clear_msg(zone-1);

						} else if ( MZONE_SEND(zone, mqtt_get_msg(zone-1))){
							mqtt_clear_msg(zone-1);
						}
					}

				}

				// outbound: zones => zone1 => broker
				char msg[16+1]; msg[sizeof(msg)-1]='\0';
                char topic[strlen(client_id) + strlen("/zone?") +1];
                strcpy(topic, client_id); strcat(topic, "/zone?");

				for (Zone zone = zone1; zone<=zone4; zone++){
					if (MZONE_RECV(zone, msg)) {

                        // forward to broker if not consumed by zone1
                        if (msg_handler (zone, msg) == 0) {
							topic[strlen(topic)-1] = (char)('0'+zone);
							mqtt_wrap_publish(topic, msg);
						}
					}
				}
			}

		}

	}

	return 0;
}
