/* Copyright(C) 2020 Hex Five Security, Inc. - All Rights Reserved */

#include <lwip/arch.h>
#include <lwip/err.h>
#include <lwip/etharp.h>
#include <lwip/netif.h>
#include <lwip/pbuf.h>

#include <platform.h>
#include <string.h>
#include <sys/_stdint.h>
#include "xemaclite.h"

/* Register offsets */
#define XEMAC_TXPING_OFF		0x0000
#define XEMAC_TXPONG_OFF		0x0800
#define XEMAC_RXPING_OFF		0x1000
#define XEMAC_RXPONG_OFF		0x1800

#define XEMAC_MDIOADDR_OFF 		0x07E4
#define XEMAC_MDIOWR_OFF		0x07E8
#define XEMAC_MDIORD_OFF		0x07EC
#define XEMAC_MDIOCTRL_OFF		0x07F0
#define XEMAC_TXPING_LEN_OFF	0x07F4
#define XEMAC_GIE_OFF			0x07F8
#define XEMAC_TXPING_CTRL_OFF	0x07FC
#define XEMAC_TXPONG_LEN_OFF	0x0FF4
#define XEMAC_TXPONG_CTRL_OFF	0x0FFC
#define XEMAC_RXPING_CTRL_OFF	0x17FC
#define XEMAC_RXPONG_CTRL_OFF	0x1FFC

/* Receive Status Register (RSR) Bit Mask */
#define XEMAC_RX_STATUS_MASK	0x00000001	/* Rx complete = 1 */

/* Transmit Status Register (TSR) Bit Masks */
#define XEMAC_TX_STATUS_MASK	0x00000001	/* Busy = 1, Ready=0 */
#define XEMAC_MAC_PROG_MASK		0x00000003	/* Prog = 3, Ready=0 */

/* Global Interrupt Enable Bit Mask */
#define XEMAC_GIE_MASK 			0x80000000 /* Enable = 1, Disable = 0 */

/* Recive Interrupt Enable Bit Mask */
#define XEMAC_RX_IE_MASK 		0x00000008 /* Enable = 1, Disable = 0 */

/* Transmit Interrupt Enable Bit Mask */
#define XEMAC_TX_IE_MASK 		0x00000008 /* Enable = 1, Disable = 0 */

static uint8_t low_level_link_state(){

	/* MDIO bit masks */
	#define XEMAC_MDIOCTRL_STATUS_MASK	0x00000001
	#define XEMAC_MDIOCTRL_ENABLE_MASK	0x00000008

	#define XEMAC_MDIOADDR_PHYDEV_MASK  0x000003E0
	#define XEMAC_MDIOADDR_PHYREG_MASK  0x0000001F
	#define XEMAC_MDIOADDR_OP_MASK	  	0x00000400 /* Read=1, Write=0 */
	#define XEMAC_MDIOADDR_PHYDEV_SHFT  0x5

	#define PHY_DEV 				0x01
	#define PHY_STATUS_REG 			0x01
	#define PHY_STATUS_LINK_MASK 	0x0004

	while( ((*(volatile uint32_t *)(XEMAC_BASE + XEMAC_MDIOCTRL_OFF)) & XEMAC_MDIOCTRL_STATUS_MASK) !=0);

	*(volatile uint32_t *)(XEMAC_BASE + XEMAC_MDIOADDR_OFF) =
			(XEMAC_MDIOADDR_OP_MASK | (PHY_DEV << XEMAC_MDIOADDR_PHYDEV_SHFT) | PHY_STATUS_REG);

	uint32_t mdioctrl = *(volatile uint32_t *)(XEMAC_BASE + XEMAC_MDIOCTRL_OFF);

	*(volatile uint32_t *)(XEMAC_BASE + XEMAC_MDIOCTRL_OFF) = (mdioctrl | XEMAC_MDIOCTRL_ENABLE_MASK | XEMAC_MDIOCTRL_STATUS_MASK);

	while( ((*(volatile uint32_t *)(XEMAC_BASE + XEMAC_MDIOCTRL_OFF)) & XEMAC_MDIOCTRL_STATUS_MASK) !=0);

	return (*(volatile uint32_t *)(XEMAC_BASE + XEMAC_MDIORD_OFF) & PHY_STATUS_LINK_MASK);

}

/**
* In this function, the hardware should be initialized.
 * Called from xemaclite_init().
 *
 * @param netif the already initialized lwip network interface structure
 *        for this xemaclite
 */
static void low_level_init(struct netif *netif) {

	/* set MAC hardware address length */
	netif->hwaddr_len = ETHARP_HWADDR_LEN;

	/* set MAC hardware address */
#ifdef XEMAC_MAC
	/* User defined */
	memcpy(netif->hwaddr, (u8_t[])XEMAC_MAC, ETHARP_HWADDR_LEN);
#else
	/* ICANN, IANA Department */
	netif->hwaddr[0] = 0x00;
	netif->hwaddr[1] = 0x00;
	netif->hwaddr[2] = 0x5E;
	netif->hwaddr[3] = 0x00;
	netif->hwaddr[4] = 0xFA;
	netif->hwaddr[5] = 0xCE;
#endif

	/* maximum transfer unit */
	netif->mtu = 1500;

	/* device capabilities */
	netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_ETHERNET;

#ifdef XEMAC_MAC
	/* Program Ethernet MAC Address */
	*(volatile uint32_t *)(XEMAC_BASE + XEMAC_TXPING_OFF+0) =
		(uint32_t)netif->hwaddr[3] <<24 | (uint32_t)netif->hwaddr[2] <<16 |
		(uint32_t)netif->hwaddr[1] << 8 | (uint32_t)netif->hwaddr[0] <<0;
	*(volatile uint32_t *)(XEMAC_BASE + XEMAC_TXPING_OFF+4) =
		(uint32_t)netif->hwaddr[5] << 8 | (uint32_t)netif->hwaddr[4] <<0;
	*(volatile uint32_t *)(XEMAC_BASE + XEMAC_TXPING_CTRL_OFF) |= XEMAC_MAC_PROG_MASK;
	while ( ((*(volatile uint32_t *)(XEMAC_BASE + XEMAC_TXPING_CTRL_OFF)) & XEMAC_MAC_PROG_MASK) != 0 ){;}
#endif

	/* Enabe RX irq & GIE */
	*(volatile uint32_t *)(XEMAC_BASE + XEMAC_RXPING_CTRL_OFF) |= XEMAC_RX_IE_MASK;
  //*(volatile uint32_t *)(XEMAC_BASE + XEMAC_TXPING_CTRL_OFF) |= XEMAC_TX_IE_MASK;
	*(volatile uint32_t *)(XEMAC_BASE + XEMAC_GIE_OFF)         |= XEMAC_GIE_MASK;

}

/**
 * Should allocate a pbuf and transfer the bytes of the incoming
 * packet from the interface into the pbuf.
 *
 * @param netif the lwip network interface structure for this xemaclite
 * @return a pbuf filled with the received packet (including MAC header)
 *         NULL on memory error
 */
static struct pbuf* low_level_input(struct netif *netif) {

	static enum {
		PING,
		PONG
	} rx_ping_pong = PING;

	volatile uint32_t *rx_buff = NULL;
	volatile uint32_t *rx_ctrl = NULL;

	/* 1 - Check if there is a pkt ready in the RX buffer ping or pong */

	if ( (rx_ping_pong == PING) && ( *(volatile uint32_t *)(XEMAC_BASE + XEMAC_RXPING_CTRL_OFF) & XEMAC_RX_STATUS_MASK) !=0){
		rx_buff = (volatile uint32_t *)(XEMAC_BASE + XEMAC_RXPING_OFF);
		rx_ctrl = (volatile uint32_t *)(XEMAC_BASE + XEMAC_RXPING_CTRL_OFF);

	} else if ( (rx_ping_pong == PONG) && (*(volatile uint32_t *)(XEMAC_BASE + XEMAC_RXPONG_CTRL_OFF) & XEMAC_RX_STATUS_MASK) !=0){
		rx_buff = (volatile uint32_t *)(XEMAC_BASE + XEMAC_RXPONG_OFF);
		rx_ctrl = (volatile uint32_t *)(XEMAC_BASE + XEMAC_RXPONG_CTRL_OFF);

	} else 	if ( (*(volatile uint32_t *)(XEMAC_BASE + XEMAC_RXPING_CTRL_OFF) & XEMAC_RX_STATUS_MASK) !=0){
		rx_buff = (volatile uint32_t *)(XEMAC_BASE + XEMAC_RXPING_OFF);
		rx_ctrl = (volatile uint32_t *)(XEMAC_BASE + XEMAC_RXPING_CTRL_OFF);
		rx_ping_pong = PING;

	} else 	if ( (*(volatile uint32_t *)(XEMAC_BASE + XEMAC_RXPONG_CTRL_OFF) & XEMAC_RX_STATUS_MASK) !=0){
		rx_buff = (volatile uint32_t *)(XEMAC_BASE + XEMAC_RXPONG_OFF);
		rx_ctrl = (volatile uint32_t *)(XEMAC_BASE + XEMAC_RXPONG_CTRL_OFF);
		rx_ping_pong = PONG;

	} else	return NULL; // no data ready

	/* 2 - Determine pkt size including header and CRC */

	#define TYPE_ARP 0x0806
	#define TYPE_IPV4 0x0800
	#define HDR_SIZE 14
	#define CRC_SIZE 4
	#define ARP_SIZE 28
	#define MTU_SIZE 1500

	volatile uint8_t *bbuf = (volatile uint8_t*)rx_buff;
	const uint16_t type     = (uint16_t)((bbuf[12] << 8) | bbuf[13]);
	const uint16_t ipv4_len = (uint16_t)((bbuf[16] << 8) | bbuf[17]);

	int len = type == TYPE_ARP  ? HDR_SIZE + ARP_SIZE + CRC_SIZE :
			  type == TYPE_IPV4 ? HDR_SIZE + ipv4_len + CRC_SIZE :
					  	  	  	  HDR_SIZE + MTU_SIZE + CRC_SIZE ;

	/* 3 - copy pkt data into pkt buffer */

#if ETH_PAD_SIZE
	len += ETH_PAD_SIZE; /* allow room for Ethernet padding */
#endif

	/* allocate a pbuf chain of pbufs from the pool */
	struct pbuf *p = pbuf_alloc(PBUF_RAW, len, PBUF_POOL);

	if (p != NULL) {

#if ETH_PAD_SIZE
		pbuf_remove_header(p, ETH_PAD_SIZE); /* drop the padding word */
#endif

		if ( pbuf_take(p, (uint32_t *)rx_buff, len) == ERR_OK ){

#if ETH_PAD_SIZE
			pbuf_add_header(p, ETH_PAD_SIZE); /* reclaim the padding word */
#endif

		} else {

			// drop packet
			pbuf_free(p); p = NULL;

		}

	}

	/* 4 - clear the rx_buff status */
	*rx_ctrl = *rx_ctrl & ~XEMAC_RX_STATUS_MASK;

	/* 5 - expect next pkt in the other buffer */
	rx_ping_pong = (rx_ping_pong==PING ? PONG : PING);

	return p;
}

/**
 * This function should do the actual transmission of the packet. The packet is
 * contained in the pbuf that is passed to the function. This pbuf
 * might be chained.
 *
 * @param netif the lwip network interface structure for this xemaclite
 * @param p the MAC packet to send (e.g. IP packet including MAC addresses and type)
 * @return ERR_OK if the packet could be sent
 *         an err_t value if the packet couldn't be sent
 *
 * @note Returning ERR_MEM here if a DMA queue of your MAC is full can lead to
 *       strange results. You might consider waiting for space in the DMA queue
 *       to become available since the stack doesn't retry to send a packet
 *       dropped because of memory failure (except for the TCP timers).
 */
static err_t low_level_output(struct netif *netif, struct pbuf *p) {

	volatile uint32_t *tx_buff = NULL;
	volatile uint32_t *tx_len  = NULL;
	volatile uint32_t *tx_ctrl = NULL;

	/* 1 - pick 1st avail TX buffer (ping | pong) */
	if ( ( *(volatile uint32_t *)(XEMAC_BASE + XEMAC_TXPING_CTRL_OFF) & XEMAC_TX_STATUS_MASK) ==0){
		tx_buff = (volatile uint32_t *)(XEMAC_BASE + XEMAC_TXPING_OFF);
		tx_len  = (volatile uint32_t *)(XEMAC_BASE + XEMAC_TXPING_LEN_OFF);
		tx_ctrl = (volatile uint32_t *)(XEMAC_BASE + XEMAC_TXPING_CTRL_OFF);

	} else if ( (*(volatile uint32_t *)(XEMAC_BASE + XEMAC_TXPONG_CTRL_OFF) & XEMAC_TX_STATUS_MASK) ==0){
		tx_buff = (volatile uint32_t *)(XEMAC_BASE + XEMAC_TXPONG_OFF);
		tx_len  = (volatile uint32_t *)(XEMAC_BASE + XEMAC_TXPONG_LEN_OFF);
		tx_ctrl = (volatile uint32_t *)(XEMAC_BASE + XEMAC_TXPONG_CTRL_OFF);

	} else return ERR_IF;

	/* 2 - fill the TX buffer */

	// The xemaclite TX buffer requires 32-bit writes
	// 4-byte words can span multiple pbuf reads
	uint8_t tx_word[4]; int tx_word_count=0;

	/* Send the data from the pbuf to the interface, one pbuf at a
	 time. The size of the data in each pbuf is kept in the ->len
	 variable. */

#if ETH_PAD_SIZE
	pbuf_remove_header(p, ETH_PAD_SIZE); /* drop the padding word */
#endif

	for (struct pbuf *q = p; q != NULL; q = q->next) {

		uint8_t *src = q->payload;

		for(int i=0; i < q->len; i++){

			tx_word[tx_word_count++] = *src++;

			if (tx_word_count ==4){
				*tx_buff++ = *((uint32_t *)tx_word);
				tx_word_count = 0;
			}

		}

	} if (tx_word_count > 0) *tx_buff = *((uint32_t *)tx_word);

	/* set the TX buffer len */
	*tx_len = p->tot_len;

#if ETH_PAD_SIZE
	pbuf_add_header(p, ETH_PAD_SIZE); /* reclaim the padding word */
#endif

	/* send the frame to the wire */
	*tx_ctrl |= XEMAC_TX_STATUS_MASK;

	return ERR_OK;

}

/**
* Should be called at the beginning of the program to set up the
* network interface. It calls the function low_level_init() to do the
* actual setup of the hardware.
*
* This function should be passed as a parameter to netif_add().
*
* @param netif the lwip network interface structure for this xemaclite
* @return ERR_OK if the loopif is initialized
*         ERR_MEM if private data couldn't be allocated
*         any other err_t on error
*/
err_t xemaclite_init(struct netif *netif) {

	LWIP_ASSERT("netif != NULL", (netif != NULL));

	/* Initialize interface hostname */
#if LWIP_NETIF_HOSTNAME
	/* Initialize interface hostname */
	netif->hostname = "LWIP_NETIF_HOSTNAME";
#endif /* LWIP_NETIF_HOSTNAME */

	netif->name[0] = 'e';
	netif->name[1] = 'n';

	/* We directly use etharp_output() here to save a function call.
	 * You can instead declare your own function an call etharp_output()
	 * from it if you have to do some checks before sending (e.g. if link
	 * is available...) */
	netif->output = etharp_output;

	netif->linkoutput = low_level_output;

	/* initialize the hardware */
	low_level_init(netif);

	return ERR_OK;

}

/**
 * This function should be called when a packet is ready to be read
 * from the interface. It uses the function low_level_input() that
 * should handle the actual reception of bytes from the network
 * interface. Then the type of the received packet is determined and
 * the appropriate input function is called.
 *
 * @param netif the lwip network interface structure for this xemaclite
 */
struct pbuf* xemaclite_input(struct netif *netif) {

	/* move received packet into a new pbuf */
	return low_level_input(netif);

}

void xemaclite_poll_link_state(struct netif *netif) {

	if (low_level_link_state())
		netif_set_link_up(netif);
	else
		netif_set_link_down(netif);

}
