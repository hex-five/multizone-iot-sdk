/* Copyright(C) 2020 Hex Five Security, Inc. - All Rights Reserved */

#include <string.h> // memset()

#include "lwip/opt.h"
#include "lwip/err.h"
#include "lwip/apps/mqtt.h"
#include "lwip/apps/mqtt_priv.h"
#include "lwip/altcp_tls.h"

#include "mqtt_config.h"
#include "mqtt_wrap.h"
#include "ca_crt.h"
#include "cli_key.h"
#include "cli_crt.h"

typedef enum {zone1=1, zone2, zone3, zone4} Zone;
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

static mqtt_client_t client;
static struct mqtt_connect_client_info_t ci;
static ip_addr_t broker_ip;

static char inbox[4][16];
static int inbox_id = 0;

extern const void * sh_buff1_start; extern const void * sh_buff1_end;
extern const void * sh_buff2_start; extern const void * sh_buff2_end;

struct sh_buffer {
	const char * start;
	const char * end;
	char * pos;
};

static struct sh_buffer buffers[] = {
		{NULL, NULL, NULL},
		{NULL, NULL, NULL},
		{(char *)&sh_buff1_start, (char *)&sh_buff1_end, (char *)&sh_buff1_start},
		{(char *)&sh_buff2_start, (char *)&sh_buff2_end, (char *)&sh_buff2_start},
};

char * mqtt_get_msg(const int i){ return (i>=0 && i<4) ? inbox[i] : NULL;}
void mqtt_clear_msg(const int i){if (i>=0 && i<4) inbox[i][0]='\0';}

static void mqtt_incoming_publish_cb(void *arg, const char *topic, u32_t tot_len) {

	//printf("Incoming publish at topic %s with total length %u\n", topic, (unsigned int) tot_len);

	/* Decode topic string into a user defined reference */
	inbox_id =  strlen(topic)>=6 && strncmp(topic+strlen(topic)-6, "/zone1", 6) == 0 ? 1 :
				strlen(topic)>=6 && strncmp(topic+strlen(topic)-6, "/zone2", 6) == 0 ? 2 :
				strlen(topic)>=6 && strncmp(topic+strlen(topic)-6, "/zone3", 6) == 0 ? 3 :
				strlen(topic)>=6 && strncmp(topic+strlen(topic)-6, "/zone4", 6) == 0 ? 4 :
				0;

	if (inbox_id>=1 && inbox_id<=4)
		buffers[inbox_id-1].pos = (char *)buffers[inbox_id-1].start;

}

static void mqtt_incoming_data_cb(void *arg, const u8_t *data, u16_t len, u8_t flags) {

	//printf("Incoming publish payload with length %d, flags %u\n", len, (unsigned int) flags);

	if (buffers[inbox_id-1].pos != NULL)
		for(int i=0; i<len && buffers[inbox_id-1].pos < buffers[inbox_id-1].end; i++)
			*(buffers[inbox_id-1].pos++) = *(data+i);

	if (flags & MQTT_DATA_FLAG_LAST) {

		/* Last fragment of payload received (or whole part if payload fits receive buffer
		 See MQTT_VAR_HEADER_BUFFER_LEN)  */

	  //printf("mqtt_incoming_data_cb:  %.*s \n", len, data); // no \0 term

		if (inbox_id && strlen(inbox[inbox_id-1])==0){
			strncpy(inbox[inbox_id-1], data, MIN(len,16));
			inbox[inbox_id-1][MIN(len,16-1)]='\0';
		}

		if ( buffers[inbox_id-1].pos - buffers[inbox_id-1].start > 16)
			strcpy(inbox[inbox_id-1], "payload");

	}

}

/*static void mqtt_pub_request_cb(void *arg, err_t result) {

	 Called when publish is complete either with sucess or failure

	if(result != ERR_OK)
		printf("Publish result: %d\n", result);

}*/

/*static void mqtt_sub_request_cb(void *arg, err_t result) {

   Just print the result code here for simplicity,
     normal behaviour would be to take some action if subscribe fails like
     notifying user, retry subscribe or disconnect from server

  printf("Subscribe result: %d\n", result);

}*/

static void mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status){

	if (status == MQTT_CONNECT_ACCEPTED) {

		//printf("mqtt_connection_cb: Successfully connected\n");

		/* Setup callback for incoming publish requests */
		mqtt_set_inpub_callback(client, mqtt_incoming_publish_cb, mqtt_incoming_data_cb, arg);

		/* Subscribe to all subtopics with QoS level x, call mqtt_sub_request_cb with result */
		char topic[strlen(ci.client_id)+strlen("/#")+1];
		strcpy(topic, ci.client_id); strcat(topic, "/#");

		//err_t err = mqtt_subscribe(client, topic, MQTT_QOS, mqtt_sub_request_cb, arg);
		err_t err = mqtt_subscribe(client, topic, MQTT_QOS, NULL, arg);

		//if (err != ERR_OK) printf("mqtt_subscribe return: %d\n", err);

	} else {

		//printf("mqtt_connection_cb: Disconnected, reason: %d\n", status);

		/* try to reconnect */
		//mqtt_gateway_connect();
	}

}

// ----------------------------- PUBLIC -----------------------------------------------------------

void mqtt_init(const ip_addr_t broker, const char *client_id) {

	/* Setup client info structure */
	memset(&ci, 0, sizeof(ci));

	ci.client_id = client_id;
	ci.keep_alive = MQTT_KEEP_ALIVE;
	ci.will_msg = MQTT_WILL_MSG;
	ci.will_qos = MQTT_WILL_QOS;
	ci.will_retain = MQTT_WILL_RTN;
#if defined(MQTT_CLIENT_ID) && defined(MQTT_WILL_TOPIC)
	ci.will_topic = MQTT_CLIENT_ID MQTT_WILL_TOPIC;
#else
	char *will_topic = (char *) malloc(strlen(client_id)+strlen(MQTT_WILL_TOPIC)+1); // do not free()
	strcpy(will_topic, client_id); strcat(will_topic, MQTT_WILL_TOPIC);
	ci.will_topic = (const char*)will_topic;
	//printf(">>> %s (%d) \n", ci.will_topic, strlen(ci.will_topic));
#endif
	ci.tls_config = altcp_tls_create_config_client_2wayauth(
		ca_crt, sizeof(ca_crt),
		cli_key, sizeof(cli_key), NULL, 0,
		cli_crt, sizeof(cli_crt)
	);

	broker_ip = broker;

}

u8_t mqtt_is_mqtt_connected(){

	return mqtt_client_is_connected(&client);

}

u8_t mqtt_is_tcp_disconnected(){


	return client.conn_state == 0; //TCP_DISCONNECTED;

}

void mqtt_connect() {

	//printf("Connecting ...\n");

#ifndef MQTT_BROKER_PORT
	#define MQTT_BROKER_PORT MQTT_TLS_PORT
#endif

	/* Initiate client and connect to server, if this fails immediately an error code is returned
	 otherwise mqtt_connection_cb will be called with connection result after attempting
	 to establish a connection with the server. */
	err_t err = mqtt_client_connect(&client, &broker_ip, MQTT_BROKER_PORT, mqtt_connection_cb, NULL, &ci);

	/* For now just print the result code if something goes wrong */
	//if (err != ERR_OK)	printf("mqtt_connect return %d\n", err);

}

void mqtt_wrap_publish(char *pub_topic, char *pub_payload) {

	err_t err = mqtt_publish(&client, pub_topic, pub_payload, strlen(pub_payload),
							 MQTT_QOS, MQTT_RTN,
							 NULL, //mqtt_pub_request_cb,
							 NULL);

	//if (err != ERR_OK) printf("Publish err: %d\n", err);

}




