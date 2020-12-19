/* Copyright(C) 2020 Hex Five Security, Inc. - All Rights Reserved */

/* Overrides lwip/opt.h */

#ifndef __LWIPOPTS_H__
#define __LWIPOPTS_H__


#define NO_SYS				1
#define LWIP_RAW			1
#define LWIP_ALTCP			1
#define LWIP_ALTCP_TLS		1
#define LWIP_NOASSERT		1
#define LWIP_TCP 			1
#define LWIP_UDP 			1
#define LWIP_DNS			1
#define LWIP_DHCP   		1
#define LWIP_DHCP_DOES_ACD_CHECK 	0
#define LWIP_SNTP					1
#define LWIP_SINGLE_NETIF			1
#define LWIP_NETIF_STATUS_CALLBACK	1
#define LWIP_NETIF_LINK_CALLBACK	1
#define SYS_LIGHTWEIGHT_PROT		0
#define SNTP_SERVER_DNS 			1
#define SNTP_MAX_SERVERS			2
#define SNTP_SERVER_1 "pool.ntp.org"
#define SNTP_SERVER_0 "time.nist.gov"
#define MQTT_VAR_HEADER_BUFFER_LEN 256


#endif /* __LWIPOPTS_H__ */
