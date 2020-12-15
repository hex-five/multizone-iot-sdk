/* Copyright(C) 2020 Hex Five Security, Inc. - All Rights Reserved */

#ifndef XEMACLITE_H
#define XEMACLITE_H


err_t xemaclite_init(struct netif *netif);

struct pbuf* xemaclite_input(struct netif *netif);

void xemaclite_poll_link_state(struct netif *netif);


#endif /* XEMACLITE_H */
