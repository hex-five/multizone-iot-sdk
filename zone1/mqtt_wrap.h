/* Copyright(C) 2020 Hex Five Security, Inc. - All Rights Reserved */

#ifndef MQTT_WRAP_H
#define MQTT_WRAP_H


void mqtt_init(const ip_addr_t broker, const char *client_id);

u8_t mqtt_is_tcp_disconnected();
u8_t mqtt_is_mqtt_connected();

void mqtt_connect();

void mqtt_wrap_publish(char *pub_topic, char *pub_payload);

char * mqtt_get_msg(const int i);
void mqtt_clear_msg(const int i);


#endif /* MQTT_WRAP_H */
